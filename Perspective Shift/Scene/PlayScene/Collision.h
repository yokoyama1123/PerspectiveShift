#pragma once

#include"GameContext.h"
#include"ImaseLib/CollisionRenderer.h"
#include"Player.h"
#include"Stage.h"
#include"Camera.h"


namespace Yokoyama
{
    // 当たり判定の検出と位置修正をするクラス
    class Collision
    {
    public:
        // コンストラクタ
        Collision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Yokoyama::Player* pPlayer,
                  Yokoyama::Stage* pStage, Yokoyama::Camera* pCamera);

        // デストラクタ
        ~Collision() = default;

        // 当たり判定の更新
        void Update(const GameContext& gameContext);

        // 当たり判定の描画
        void Render(const GameContext& gameContext, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
        
        // ステージをクリアしたか
        bool GetStageClear() const;

    private:
        //----- メンバー変数 -----//
        // コリジョン情報表示オブジェクト
        std::unique_ptr<Imase::CollisionRenderer> m_collisionRenderer;
        // プレイヤーのポインタ
        Yokoyama::Player* m_pPlayer;
        // ステージのポインタ
        Yokoyama::Stage* m_pStage;
        // カメラのポインタ
        Yokoyama::Camera* m_pCamera;
        // 当たり判定を表示するか
        bool m_showCollision;
        // ステージクリア
        bool m_stageClear;
        //----------//

        //----- メンバー関数 -----//
        // AABBの当たり判定
        bool HitCheckAABB2AABB(const DirectX::BoundingBox& box1, const DirectX::BoundingBox& box2);
        // ステージのブロックとプレイヤーの当たり判定と修正
        void PlayerBlockCollision();
        // プレイヤーとステージ外枠の当たり判定と位置修正
        void PlayerStageCollision();
        // プレイヤーとゴールの当たり判定
        void PlayerGoalCollision(bool isDebugMode);
        // カメラとステージ外枠の当たり判定と位置修正
        void CameraStageCollision();
        //----------//
    };
}