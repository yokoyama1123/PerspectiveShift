#include"pch.h"
#include "Stage.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pDevice">デバイス</param>
/// <param name="pContext">デバイスコンテキスト</param>
/// <param name="pStates">コモンステート</param>
/// <param name="textureHandle">板ポリゴンのテクスチャ</param>
Yokoyama::Stage::Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const GameContext& gameContext)
{
    //板ポリゴンの登録
    RegistingStageData(gameContext);

    //板ポリゴンの作成
    m_plane = std::make_unique<Yokoyama::Plane>(pDevice, pContext, &gameContext.commonStates, gameContext.textureHandle);
}

/// <summary>
/// 更新関数
/// </summary>
/// <param name="gameContext">gameContext</param>
/// <param name="elapsedTime">elapsedTime</param>
void Yokoyama::Stage::Update(const GameContext& gameContext, float elapsedTime)
{
    SimpleMath::Vector3 goalPosition{};

    for (size_t i = 0; i < m_cellDatas.size(); i++)
    {
        if (m_cellDatas[i].type == CellType::Goal)
        {
            goalPosition = m_cellDatas[i].boundingBox.Center;
        }
    }

    //ワールド行列への反映
    m_worldStage = SimpleMath::Matrix::CreateScale(STAGESCALE);//大きさ

    m_worldGoal = SimpleMath::Matrix::CreateScale(GOALSCALE) * //大きさ
                  SimpleMath::Matrix::CreateTranslation(goalPosition);//位置
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="gameContext">gameContext</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロゼクション行列</param>
/// <param name="pContext">コンテキスト</param>
void Yokoyama::Stage::Render(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, ID3D11DeviceContext* pContext)
{
     //ステージのモデルの描画
    gameContext.stages[gameContext.selectStage]->Draw(
        gameContext.deviceResources.GetD3DDeviceContext(),
        gameContext.commonStates,
        m_worldStage,
        view,
        projection
    );

    //ゴールのモデルの描画
    gameContext.goalModel->Draw(
        gameContext.deviceResources.GetD3DDeviceContext(),
        gameContext.commonStates,
        m_worldGoal,
        view,
        projection
    );

    //板ポリゴンの描画
    m_plane->Render(pContext, view, projection, m_cellDatas);
}

/// <summary>
/// プレイヤーの初期位置を返す関数
/// </summary>
/// <returns>PlayerSetPosition</returns>
DirectX::SimpleMath::Vector3 Yokoyama::Stage::GetPlayerSetPosition()
{
    DirectX::SimpleMath::Vector3 playerPosition{};

    //セルデータの中からプレイヤーの初期位置を探す
    for (size_t i = 0; i < m_cellDatas.size(); i++)
    {
        if (m_cellDatas[i].type == CellType::Player)
        {
            //そのマスの中心にプレイヤーを配置
            playerPosition.x =   CellData::SIZE * m_cellDatas[i].stagePosition.x + CellData::SIZE / 2;
            playerPosition.y =   CellData::SIZE * m_cellDatas[i].stagePosition.y + CellData::SIZE / 2;
            playerPosition.z = -(CellData::SIZE * m_cellDatas[i].stagePosition.z + CellData::SIZE / 2);
            return playerPosition;
        }
    }

    return playerPosition;
}

/// <summary>
/// ステージデータを返す関数
/// </summary>
/// <returns>cellDatas</returns>
std::vector<Yokoyama::CellData> Yokoyama::Stage::GetCellDatas()
{
    return m_cellDatas;
}

/// <summary>
/// ステージ外枠の当たり判定を返す関数
/// </summary>
/// <returns>Yokoyama::WallData</returns>
std::vector<Yokoyama::WallData> Yokoyama::Stage::GetWallData()
{
    return WALL_DATA;
}

/// <summary>
/// セルの場所を変更する
/// </summary>
/// <param name="cellnumber">どのセルか</param>
/// <param name="stagePosition">変更する場所</param>
void Yokoyama::Stage::SetCellPosition(int cellnumber, DirectX::SimpleMath::Vector3 stagePosition)
{
    m_cellDatas[cellnumber].stagePosition = stagePosition;
    SetBoundingBox();
}

void Yokoyama::Stage::DeleteCell(int cellnumber)
{
    auto it = m_cellDatas.begin() + cellnumber;
    m_cellDatas.erase(it);
}

void Yokoyama::Stage::AddCell()
{
    m_cellDatas.push_back
    (
        Yokoyama::CellData
        { 
            {0,0,0},
            Yokoyama::CellType::Xm,
            BoundingBox{{CellData::SIZE / 2, CellData::SIZE / 2, -CellData::SIZE / 2}, Yokoyama::CellData::DISTANCE}
        }
    );
}

/// <summary>
/// ステージデータを登録
/// </summary>
void Yokoyama::Stage::RegistingStageData(const GameContext& gameContext)
{
    m_cellDatas = gameContext.saveLoad->GetCellDatas(*gameContext.saveLoad->GetStagesJson(gameContext.selectStage));

    //当たり判定の登録
    SetBoundingBox();
}

/// <summary>
/// 当たり判定の登録
/// </summary>
void Yokoyama::Stage::SetBoundingBox()
{
    for (size_t i = 0; i < m_cellDatas.size(); i++)
    {
        //プレイヤーの初期位置以外に当たり判定をつける
        //ゴールの当たり判定
        if (m_cellDatas[i].type == CellType::Goal)
        {
            m_cellDatas[i].boundingBox.Center =
                SimpleMath::Vector3{ CellData::SIZE * m_cellDatas[i].stagePosition.x + CellData::SIZE / 2,
                                     CellData::SIZE * m_cellDatas[i].stagePosition.y + CellData::SIZE / 2,
                                   -(CellData::SIZE * m_cellDatas[i].stagePosition.z + CellData::SIZE / 2) };
        }
        //板ポリゴンの当たり判定
        else if (m_cellDatas[i].type != CellType::Player)
        {
            //ステージ座標にあわせたワールド座標の当たり判定
            m_cellDatas[i].boundingBox.Center =
                SimpleMath::Vector3{ CellData::SIZE * m_cellDatas[i].stagePosition.x + CellData::SIZE / 2,
                                     CellData::SIZE * m_cellDatas[i].stagePosition.y + CellData::SIZE / 2,
                                   -(CellData::SIZE * m_cellDatas[i].stagePosition.z + CellData::SIZE / 2) };
        }
    }
}
