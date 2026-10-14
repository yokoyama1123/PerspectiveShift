#include"pch.h"
#include "Collision.h"
#include <algorithm>

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
    // 当たり判定を表示するかどうか(F3+B)
    if ((gameContext.keyboardTracker.pressed.F3 && Keyboard::Get().GetState().B) ||
        (gameContext.keyboardTracker.pressed.B && Keyboard::Get().GetState().F3))
    {
        m_showCollision = !m_showCollision;
    }
    // デバッグモードなら常に当たり判定を表示する
    if (gameContext.isDebugMode)
    {
        m_showCollision = true;
    }

    // プレイヤーの当たり判定の登録
    if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox());


    // プレイヤーとステージ外枠の当たり判定と位置修正
    PlayerStageCollision();

    // プレイヤーとブロックの当たり判定と位置修正
    PlayerBlockCollision();

    // プレイヤーとゴールの当たり判定(プレイヤーの当たり判定の位置修正がすべて終了してから行う)
    PlayerGoalCollision(gameContext.isDebugMode);

    // カメラとステージ外枠の当たり判定と位置修正
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

    // 登録した当たり判定をまとめて描画
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
    //各軸が当たっているかどうか
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
/// 線分と面の当たり判定
/// </summary>
/// <param name="a">線分の始まり</param>
/// <param name="b">線分の終わり</param>
/// <param name="p">面</param>
/// <param name="q">当たっている場所の座標</param>
/// <returns>当たっていたらtrue</returns>
bool Yokoyama::Collision::IntersectSegmentPlane(const DirectX::SimpleMath::Vector3& a, const DirectX::SimpleMath::Vector3& b, const DirectX::SimpleMath::Plane& p, DirectX::SimpleMath::Vector3* q)
{
    float t;
    DirectX::SimpleMath::Vector3 ab = b - a;
    t = (p.D() - p.Normal().Dot(a)) / p.Normal().Dot(ab);
    if (t>= 0 && t < 1.0f)
    {
        (*q) = a + t * ab;
        return true;
    }
    return false;
}

/// <summary>
/// ステージのブロックとプレイヤーの当たり判定と修正
/// </summary>
void Yokoyama::Collision::PlayerBlockCollision()
{
    // ソートしたステージデータ
    std::vector<BoundingBox> sortBoxes{};
    // ソートのための仮ボックス
    BoundingBox tmp{};

    // ステージデータをコピー
    for (size_t i = 0; i < m_pStage->GetCellDatas().size(); i++)
    {
        // ブロックの情報のみをコピーする
        if ((m_pStage->GetCellDatas()[i].type != Yokoyama::CellType::Player) && (m_pStage->GetCellDatas()[i].type != Yokoyama::CellType::Goal))
        {
            sortBoxes.push_back(m_pStage->GetCellDatas()[i].boundingBox);
        }
    }

    // ブロックが無かったら何もしない
    if (sortBoxes.size() == 0)
    {
        return;
    }

    // プレイヤーから近い順にソート
    std::sort(sortBoxes.begin(), sortBoxes.end(),
        [this](const auto& a, const auto& b) {
            return SimpleMath::Vector3::Distance(a.Center, m_pPlayer->GetCenterPosition())
                < SimpleMath::Vector3::Distance(b.Center, m_pPlayer->GetCenterPosition());
        }
    );

    // 修正する移動量
    SimpleMath::Vector3 addpos{};

    // ステージデータの当たり判定の登録&当たっているこの判定&位置修正
    for (size_t i = 0; i < sortBoxes.size(); i++)
    {
        // プレイヤーとブロックが当たっているか
        if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
        {
            // プレイヤーの位置修正(X座標)
            addpos = m_pPlayer->GetPosition();
            addpos.x -= m_pPlayer->GetVelocity().x;
            m_pPlayer->SetPosition(addpos);

            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
            {
                // プレイヤーの位置修正(Z座標)
                addpos.x += m_pPlayer->GetVelocity().x;
                addpos.z -= m_pPlayer->GetVelocity().z;
                m_pPlayer->SetPosition(addpos);

                if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), sortBoxes[i]))
                {
                    // プレイヤーの位置修正(Y座標)
                    addpos.z += m_pPlayer->GetVelocity().z;
                    addpos.y -= m_pPlayer->GetVelocity().y;
                    m_pPlayer->SetPosition(addpos);

                    // 地面に接している場合ジャンプができるようにする
                    if (m_pPlayer->GetVelocity().y < 0) m_pPlayer->SetCanJump(true);
                    // 垂直抗力
                    m_pPlayer->SetVelocity(SimpleMath::Vector3{ m_pPlayer->GetVelocity().x, 0.0f, m_pPlayer->GetVelocity().z });
                }
            }

            // 当たっているブロックとプレイヤーの当たり判定のを赤色にする
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox(), Colors::Red);
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(sortBoxes[i], Colors::Red);
        }
        else
        {
            // ブロックの当たり判定の登録
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
        // プレイヤーとステージの壁が当たっているか
        if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
        {
            // プレイヤーの位置修正(X座標)
            addpos = m_pPlayer->GetPosition();
            addpos.x -= m_pPlayer->GetVelocity().x;
            m_pPlayer->SetPosition(addpos);

            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
            {
                // プレイヤーの位置修正(Z座標)
                addpos.x += m_pPlayer->GetVelocity().x;
                addpos.z -= m_pPlayer->GetVelocity().z;
                m_pPlayer->SetPosition(addpos);

                //床との当たり判定は取らない
                if (m_pStage->GetWallData()[i].type != Yokoyama::WallType::Ym && HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetWallData()[i].boundingBox))
                {
                    // プレイヤーの位置修正(Y座標)
                    addpos.z += m_pPlayer->GetVelocity().z;
                    addpos.y -= m_pPlayer->GetVelocity().y;
                    m_pPlayer->SetPosition(addpos);

                    // 地面に接している場合ジャンプができるようにする
                    if (m_pPlayer->GetVelocity().y < 0) m_pPlayer->SetCanJump(true);
                    // 垂直抗力
                    m_pPlayer->SetVelocity(SimpleMath::Vector3{ m_pPlayer->GetVelocity().x, 0.0f, m_pPlayer->GetVelocity().z });

                }
            }

            // 当たっているステージの壁とプレイヤーの当たり判定のを青色にする
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox(), Colors::Blue);
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox, Colors::Blue);
        }
        else
        {
            // ステージの壁の当たり判定の登録
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetWallData()[i].boundingBox);
        }
    }
}

/// <summary>
/// プレイヤーとゴールの当たり判定
/// </summary>
/// <param name="isDebugMode">デバッグモードかどうか</param>
void Yokoyama::Collision::PlayerGoalCollision(bool isDebugMode)
{
    // デバッグモードならクリアしない
    if (isDebugMode) return;

    for (size_t i = 0; i < m_pStage->GetCellDatas().size(); i++)
    {
        if (m_pStage->GetCellDatas()[i].type == CellType::Goal)
        {
            // プレイヤーとゴールが当たっているか
            if (HitCheckAABB2AABB(m_pPlayer->GetBoundingBox(), m_pStage->GetCellDatas()[i].boundingBox))
            {
                // ステージクリア
                m_stageClear = true;
                // 当たっているゴールとプレイヤーの当たり判定のをピンク色にする
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pPlayer->GetBoundingBox(), Colors::Pink);
                if (m_showCollision) m_collisionRenderer->AddBoundingVolume(m_pStage->GetCellDatas()[i].boundingBox, Colors::Pink);
            }
            else
            {
                // ゴールの当たり判定の登録
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
    bool useLineClip = (m_pCamera->GetCameraMode() == Yokoyama::Camera::CameraMode::Tracking);
    constexpr float margin = 0.3f;   // line clip 方式の余白(クランプ方式は0)
    const auto& walls = m_pStage->GetWallData();

    auto eye = m_pCamera->GetEyePosition();
    auto pivot = m_pCamera->GetTargetPosition();
    auto d = eye - pivot;

    auto at = [](SimpleMath::Vector3& v, int axis) -> float&
        {
            return axis == 0 ? v.x : axis == 1 ? v.y : v.z;
        };

    // axis: 0=X 1=Y 2=Z / sign: -1=マイナス側 +1=プラス側 / face: 内側の面の座標
    auto info = [](const auto& w, int& axis, float& sign, float& face)
        {
            const auto& c = w.boundingBox.Center;
            const auto& e = w.boundingBox.Extents;
            switch (w.type)
            {
            case WallType::Xm: axis = 0; sign = -1; face = c.x + e.x; break;
            case WallType::Xp: axis = 0; sign = +1; face = c.x - e.x; break;
            case WallType::Ym: axis = 1; sign = -1; face = c.y + e.y; break;
            case WallType::Yp: axis = 1; sign = +1; face = c.y - e.y; break;
            case WallType::Zm: axis = 2; sign = -1; face = c.z + e.z; break;
            case WallType::Zp: axis = 2; sign = +1; face = c.z - e.z; break;
            default:           axis = -1; break;
            }
        };

    // ---- ① 線分クリップ(Ym以外)----
    if (useLineClip)
    {
        float t = 1.0f;
        int hitIndex = -1;

        for (size_t i = 0; i < walls.size(); i++)
        {
            if (walls[i].type == WallType::Ym) continue;   // Ymは②で処理

            int axis; float sign, face;
            info(walls[i], axis, sign, face);
            if (axis < 0) continue;

            face -= sign * margin;                          // 余白を内側へ
            float p = at(pivot, axis);
            float dd = at(d, axis);

            if (dd * sign > 0.0f && (p + dd - face) * sign > 0.0f)
            {
                float newT = (face - p) / dd;
                if (newT < t) { t = newT; hitIndex = static_cast<int>(i); }
            }
        }

        if (hitIndex >= 0)
        {
            eye = pivot + d * std::max(t, 0.0f);
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(walls[hitIndex].boundingBox, Colors::Green);
        }
    }

    // ---- ② 従来のクランプ(useLineClip=falseなら全面、trueならYmだけ)----
    for (const auto& w : walls)
    {
        if (useLineClip && w.type != WallType::Ym) continue;

        int axis; float sign, face;
        info(w, axis, sign, face);
        if (axis < 0) continue;

        float& v = at(eye, axis);
        if ((v - face) * sign > 0.0f)
        {
            v = face;
            if (m_showCollision) m_collisionRenderer->AddBoundingVolume(w.boundingBox, Colors::Green);
        }
    }

    //位置の反映
    m_pCamera->SetPosition(eye);
}