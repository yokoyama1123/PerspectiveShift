#pragma once
#include "GameContext.h"

namespace Yokoyama
{
    class Player
    {
    public:
        // コンストラクタ
        Player(DirectX::SimpleMath::Vector3 position);
        // デストラクタ
        ~Player() = default;
    
        // プレイヤーの更新
        void Update(const GameContext& gameContext, float elapsedTime, const DirectX::SimpleMath::Vector3& eyePosition);
        // プレイヤーの描画
        void Render(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
        
        //重力の取得
        float GetGravity() const;
        // プレイヤーのy0の位置を取得
        DirectX::SimpleMath::Vector3 GetPosition() const;
        //プレイヤーの中心の位置を取得
        DirectX::SimpleMath::Vector3 GetCenterPosition() const;
        //プレイヤーの速さを取得
        DirectX::SimpleMath::Vector3 GetVelocity() const;
        // AABBを取得
        const DirectX::BoundingBox& GetBoundingBox();

        //位置を設定
        void SetPosition(const DirectX::SimpleMath::Vector3& position);
        //速さを設定
        void SetVelocity(const DirectX::SimpleMath::Vector3& velocity);
        //ジャンプできるかの設定
        void SetCanJump(bool canJump);

    private:
        //----- 定数 -----//
        // プレイヤーのスピード
        static constexpr float SPEED = 13.0f;

        //大きさ(n倍)
        static constexpr float SCALE = 1.0f;

        //重力
        static constexpr float GRAVITY = 50.0f;

        //ジャンプ力
        static constexpr float JUMP = 23.0f;
        
        //中心から各軸までの距離(当たり判定)
        static constexpr DirectX::SimpleMath::Vector3 DISTANCE = {1.0f, 1.0f, 1.0f};

        //----------//
        
        //----- メンバー変数 -----//
        //ワールド行列
        DirectX::SimpleMath::Matrix m_world;

        // プレイヤーの位置
        DirectX::SimpleMath::Vector3 m_position;

        //速さ
        DirectX::SimpleMath::Vector3 m_velocity;

        // AABB（衝突判定用）
        DirectX::BoundingBox m_boundingBox;

        // プレイヤーの向き
        float m_angle;

        //elapsedTime
        float m_elapsedTime;

        //ジャンプできる
        bool m_canJump;

        //----------//

        //----- メンバー関数 -----//
        //プレイヤーの移動
        void Move(DirectX::SimpleMath::Vector3 orientation, DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 forward, float elapsedTime);
        //----------//
    };
}