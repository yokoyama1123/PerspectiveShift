#pragma once
#include "GameContext.h"
#include "Plane.h"
#include"CellData.h"
#include <vector>

namespace Yokoyama
{
    // ステージ外枠のタイプ
    enum class WallType
    {
        Xm,
        Xp,
        Ym,
        Yp,
        Zm,
        Zp,
    };

    // ステージ外枠のデータ
    struct WallData
    {
        // 当たり判定
        DirectX::BoundingBox boundingBox;

        // タイプ
        WallType type;
    };


	// ステージクラス
    class Stage
    {
    public:
        // コンストラクタ
        Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const GameContext& gameContext);
        // デストラクタ
        ~Stage() = default;
        // ステージの更新
        void Update(const GameContext& gameContext, float elapsedTime);
        // ステージの描画
        void Render(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, ID3D11DeviceContext* pContext);

        // プレイヤーの初期位置を返す関数
        DirectX::SimpleMath::Vector3 GetPlayerSetPosition();

        // ステージデータを返す関数
        std::vector<Yokoyama::CellData> GetCellDatas();

        // ステージ外枠の当たり判定を返す関数
        std::vector<Yokoyama::WallData> GetWallData();

        // セルの場所を変更する
        void SetCellPosition(int cellnumber, DirectX::SimpleMath::Vector3 stagePosition);

        // セルのタイプを変更する
        void SetCellType(int cellnumber, Yokoyama::CellType type);

        // そのセルを削除する
        void DeleteCell(int cellnumber);

        // セルの追加
        void AddCell();

    private:
        //----- 定数 -----//
        // ステージの大きさ(n倍)
        static constexpr float STAGESCALE = 10.0f;
        // ゴールの大きさ(n倍)
        static constexpr float GOALSCALE = 0.8f;
        // ステージ外枠の当たり判定
        std::vector<Yokoyama::WallData> WALL_DATA{
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{-1, 3 * STAGESCALE / 2, -(3 * STAGESCALE / 2)},DirectX::SimpleMath::Vector3{1, 3 * STAGESCALE / 2, 3 * STAGESCALE / 2}}, WallType::Xm},
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{ 3 * STAGESCALE + 1, 3 * STAGESCALE / 2, -(3 * STAGESCALE / 2)}, DirectX::SimpleMath::Vector3{ 1, 3 * STAGESCALE / 2, 3 * STAGESCALE / 2}}, WallType::Xp},
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 3 * STAGESCALE / 2, -(3 * STAGESCALE + 1)}, DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 3 * STAGESCALE / 2, 1}}, WallType::Zm},
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 3 * STAGESCALE / 2, 1}, DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 3 * STAGESCALE / 2, 1}}, WallType::Zp},
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, -1, -(3 * STAGESCALE / 2)}, DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 1, 3 * STAGESCALE / 2}}, WallType::Ym},
            {DirectX::BoundingBox{DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 3 * STAGESCALE + 1, -(3 * STAGESCALE / 2)}, DirectX::SimpleMath::Vector3{ 3 * STAGESCALE / 2, 1, 3 * STAGESCALE / 2}}, WallType::Yp}
        };
        //----------//

        //----- メンバー変数 -----//
        // ステージのワールド行列
        DirectX::SimpleMath::Matrix m_worldStage;
        // ゴールのワールド行列
        DirectX::SimpleMath::Matrix m_worldGoal;
        // どこに何があるかをまとめたやつ
        std::vector<CellData> m_cellDatas;
        // 板ポリゴン
        std::unique_ptr<Yokoyama::Plane> m_plane;
        //----------//

        //----- メンバー関数 -----//
        // ステージデータを登録
        void RegistingStageData(const GameContext& gameContext);
        // 当たり判定の登録
        void SetBoundingBox();
        //----------//
	};
}