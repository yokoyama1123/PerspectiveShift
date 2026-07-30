#include"pch.h"
#include "Collision.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pDevice">device</param>
/// <param name="pContext">context</param>
Yokoyama::Collision::Collision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Yokoyama::Player* pPlayer, Yokoyama::Stage* pStage, Yokoyama::Camera* pCamera)
    : m_showCollision{false}
    , m_stageClear{false}
    , m_pPlayer{pPlayer}
    , m_pStage{pStage}
    , m_pCamera{pCamera}
{
    // コリジョン情報表示オブジェクトの作成
    m_collisionRenderer = std::make_unique<Imase::CollisionRenderer>(pDevice, pContext);
}

/// <summary>
/// 更新関数
/// </summary>
/// <param name="gameContext">gameContext</param>
/// <param name="pPlayer">プレイヤーオブジェクトのポインタ</param>
/// <param name="pStage">ステージオブジェクトのポインタ</param>
void Yokoyama::Collision::Update(const GameContext& gameContext)
{
    //当たり判定を表示するかどうか(F3+B)
    if ((gameContext.keyboardTracker.pressed.F3 && Keyboard::Get().GetState().B) ||
        (gameContext.keyboardTracker.pressed.B && Keyboard::Get().GetState().F3))
    {
        m_showCollision = !m_showCollision;
    }
    //デバッグモードなら常に当たり判定を表示する
    if (gameContext.isDebugMode)
    {
        m_showCollision = true;
    }

    //プレイヤーの当たり判定の登録
    if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox());


    //プレイヤーとステージ外枠の当たり判定と位置修正
    PlayerStageCollision();

    //プレイヤーとブロックの当たり判定と位置修正
    PlayerBlockCollision();

    //プレイヤーとゴールの当たり判定(プレイヤーの当たり判定の位置修正がすべて終了してから行う)
    PlayerGoalCollision(gameContext.isDebugMode);

    //カメラとステージ外枠の当たり判定と位置修正
    CameraStageCollision();
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="gameContext">gameContext</param>
/// <param name="view">view</param>
/// <param name="projection">projection</param>
/// <param name="pPlayer">プレイヤーオブジェクトのポインタ</param>
/// <param name="pStage">ステージオブジェクトのポインタ</param>
void Yokoyama::Collision::Render(const GameContext& gameContext, SimpleMath::Matrix view, SimpleMath::Matrix projection)
{
    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    //登録した当たり判定をまとめて描画
    m_collisionRenderer->DrawCollision(context, gameContext.commonStates, view, projection, Colors::White, Colors::White, 0.0f);
}

/// <summary>
/// ステージをクリアしたか
/// </summary>
/// <returns>ゴールに触れたらtrue</returns>
bool Yokoyama::Collision::GetStageClear() const
{
    return m_stageClear;
}

/// <summary>
/// AABBの当たり判定
/// </summary>
/// <param name="box1">Box1</param>
/// <param name="box2">Box2</param>
/// <returns>当たっていたらtrue</returns>
bool Yokoyama::Collision::HitCheckAABB2AABB(const BoundingBox& box1, const BoundingBox& box2)
{
    if (fabsf(box1.Center.x - box2.Center.x) > (box1.Extents.x + box2.Extents.x))
    {
        return false;
    }
    if (fabsf(box1.Center.y - box2.Center.y) > (box1.Extents.y + box2.Extents.y))
    {
        return false;
    }
    if (fabsf(box1.Center.z - box2.Center.z) > (box1.Extents.z + box2.Extents.z))
    {
        return false;
    }
    return true;
}

/// <summary>
/// ステージのブロックとプレイヤーの当たり判定と修正
/// </summary>
void Yokoyama::Collision::PlayerBlockCollision()
{
    //ソートしたステージデータ
    std::vector<BoundingBox> sortBoxes{};
    //ソートのための仮ボックス
    BoundingBox tmp{};

    //ステージデータをコピー
    for (size_t i = 0; i < m_pStage->GetCellDatas().size(); i++)
    {
        if ((m_pStage->GetCellDatas()[i].type != Yokoyama::CellType::Player) && (m_pStage->GetCellDatas()[i].type != Yokoyama::CellType::Goal))
        {
            sortBoxes.push_back(m_pStage->GetCellDatas()[i].boundingBox);
        }
    }

    //プレイヤーから近い順にソート
    for (int i = 0; i < sortBoxes.size() - 1; i++)
    {
        for (int j = static_cast<int>(sortBoxes.size() - 2); j >= 0; j--)
        {
            if (SimpleMath::Vector3::Distance(sortBoxes[j].Center, m_pPlayer->GetCenterPosition())
            > SimpleMath::Vector3::Distance(sortBoxes[j + 1].Center, m_pPlayer->GetCenterPosition()))
            {
                tmp = sortBoxes[j];
                sortBoxes[j] = sortBoxes[j + 1];
                sortBoxes[j + 1] = tmp;
            }
        }
    }

    // 修正する移動量
    SimpleMath::Vector3 addpos{};

    //ステージデータの当たり判定の登録&当たっているこの判定&位置修正
    for (size_t i = 0; i < sortBoxes.size(); i++)
    {
        //プレイヤーとブロックが当たっているか
        if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
        {
            //プレイヤーの位置修正(X座標)
            addpos = m_pPlayer->GetPosition();
            addpos.x -= m_pPlayer->GetVelocity().x;
            m_pPlayer->SetPosition(addpos);

            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
            {
                //プレイヤーの位置修正(Z座標)
                addpos.x += m_pPlayer->GetVelocity().x;
                addpos.z -= m_pPlayer->GetVelocity().z;
                m_pPlayer->SetPosition(addpos);

                if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
                {
                    //プレイヤーの位置修正(Y座標)
                    addpos.z += m_pPlayer->GetVelocity().z;
                    addpos.y -= m_pPlayer->GetVelocity().y;
                    m_pPlayer->SetPosition(addpos);

                    //地面に接している場合ジャンプができるようにする
                    if (m_pPlayer->GetVelocity().y < 0) m_pPlayer->SetCanJump(true);
                    //垂直抗力
                    m_pPlayer->SetVelocity(SimpleMath::Vector3{ m_pPlayer->GetVelocity().x, 0.0f, m_pPlayer->GetVelocity().z });
                }
            }

            //当たっているブロックとプレイヤーの当たり判定のを赤色にする
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox(), Colors::Red);
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(sortBoxes[i], Colors::Red);
        }
        else
        {
            //ブロックの当たり判定の登録
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(sortBoxes[i]);
        }
    }
}

/// <summary>
/// プレイヤーとステージ外枠の当たり判定と位置修正
/// </summary>
void Yokoyama::Collision::PlayerStageCollision()
{
    // 修正する移動量
    SimpleMath::Vector3 addpos{};
    for (size_t i = 0; i < m_pStage->GetWallData().size(); i++)
    {
        //プレイヤーとステージの壁が当たっているか
        if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
        {
            //プレイヤーの位置修正(X座標)
            addpos = m_pPlayer->GetPosition();
            addpos.x -= m_pPlayer->GetVelocity().x;
            m_pPlayer->SetPosition(addpos);

            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
            {
                //プレイヤーの位置修正(Z座標)
                addpos.x += m_pPlayer->GetVelocity().x;
                addpos.z -= m_pPlayer->GetVelocity().z;
                m_pPlayer->SetPosition(addpos);

                if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
                {
                    //プレイヤーの位置修正(Y座標)
                    addpos.z += m_pPlayer->GetVelocity().z;
                    addpos.y -= m_pPlayer->GetVelocity().y;
                    m_pPlayer->SetPosition(addpos);

                    //地面に接している場合ジャンプができるようにする
                    if (m_pPlayer->GetVelocity().y < 0) m_pPlayer->SetCanJump(true);
                    //垂直抗力
                    m_pPlayer->SetVelocity(SimpleMath::Vector3{ m_pPlayer->GetVelocity().x, 0.0f, m_pPlayer->GetVelocity().z });

                }
            }

            //当たっているステージの壁とプレイヤーの当たり判定のを青色にする
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox(), Colors::Blue);
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Blue);
        }
        else
        {
            //ステージの壁の当たり判定の登録
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox);
        }
    }
}

/// <summary>
/// プレイヤーとゴールの当たり判定
/// </summary>
void Yokoyama::Collision::PlayerGoalCollision(bool isDebugMode)
{
    for (size_t i = 0; i < m_pStage->GetCellDatas().size(); i++)
    {
        if (m_pStage->GetCellDatas()[i].type == CellType::Goal)
        {
            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetCellDatas()[i].boundingBox))
            {
                //デバッグモードならクリアしない
                if(!isDebugMode) m_stageClear = true;

                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetCellDatas()[i].boundingBox, Colors::Pink);
            }
            else
            {
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetCellDatas()[i].boundingBox);
            }
        }
    }
}

/// <summary>
/// カメラとステージ外枠の当たり判定と位置修正
/// </summary>
void Yokoyama::Collision::CameraStageCollision()
{
    //ステージ外枠の六面の当たり判定を確認する
    for (size_t i = 0; i < m_pStage->GetWallData().size(); i++)
    {
        //どの面に当たったか
        switch (m_pStage->GetWallData()[i].type)
        {
        case Yokoyama::WallType::Xm://Xマイナス方向の面
            if (m_pCamera->GetEyePosition().x < m_pStage->GetWallData()[i].boundingBox.Center.x + m_pStage->GetWallData()[i].boundingBox.Extents.x)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.x = m_pStage->GetWallData()[i].boundingBox.Center.x + m_pStage->GetWallData()[i].boundingBox.Extents.x;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        case Yokoyama::WallType::Xp://Xプラス方向の面
            if (m_pCamera->GetEyePosition().x > m_pStage->GetWallData()[i].boundingBox.Center.x - m_pStage->GetWallData()[i].boundingBox.Extents.x)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.x = m_pStage->GetWallData()[i].boundingBox.Center.x - m_pStage->GetWallData()[i].boundingBox.Extents.x;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        case Yokoyama::WallType::Ym://Yマイナス方向の面
            if (m_pCamera->GetEyePosition().y < m_pStage->GetWallData()[i].boundingBox.Center.y + m_pStage->GetWallData()[i].boundingBox.Extents.y)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.y = m_pStage->GetWallData()[i].boundingBox.Center.y + m_pStage->GetWallData()[i].boundingBox.Extents.y;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        case Yokoyama::WallType::Yp://Yプラス方向の面
            if (m_pCamera->GetEyePosition().y > m_pStage->GetWallData()[i].boundingBox.Center.y - m_pStage->GetWallData()[i].boundingBox.Extents.y)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.y = m_pStage->GetWallData()[i].boundingBox.Center.y - m_pStage->GetWallData()[i].boundingBox.Extents.y;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        case Yokoyama::WallType::Zm://Zマイナス方向の面
            if (m_pCamera->GetEyePosition().z < m_pStage->GetWallData()[i].boundingBox.Center.z + m_pStage->GetWallData()[i].boundingBox.Extents.z)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.z = m_pStage->GetWallData()[i].boundingBox.Center.z + m_pStage->GetWallData()[i].boundingBox.Extents.z;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        case Yokoyama::WallType::Zp://Zプラス方向の面
            if (m_pCamera->GetEyePosition().z > m_pStage->GetWallData()[i].boundingBox.Center.z - m_pStage->GetWallData()[i].boundingBox.Extents.z)
            {
                auto pos = m_pCamera->GetEyePosition();
                pos.z = m_pStage->GetWallData()[i].boundingBox.Center.z - m_pStage->GetWallData()[i].boundingBox.Extents.z;
                m_pCamera->SetPosition(pos);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Green);
            }
            break;
        default:
            break;
        }
    }
}