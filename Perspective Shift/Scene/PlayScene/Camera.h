#pragma once
#include "GameContext.h"

namespace Yokoyama
{
    // カメラクラス
    class Camera
    {
    public:
        // コンストラクタ
        Camera(int windowWidth, int windowHeight);
        // デストラクタ
        ~Camera() = default;
    
        // 更新
        void Update(GameContext& gameContext, float elapsedTime, const DirectX::SimpleMath::Vector3& target);
    
        // 視点取得
        DirectX::SimpleMath::Vector3 GetEyePosition() const;
    
        // 注視点取得
        DirectX::SimpleMath::Vector3 GetTargetPosition() const;
    
        // 速さ取得
        DirectX::SimpleMath::Vector3 GetVelocity()const;

        // ビュー行列の取得関数
        DirectX::SimpleMath::Matrix GetCameraMatrix() const;

        // カメラモードかどうか
        bool GetCameraMode() const;

        // 位置を設定
        void SetPosition(const DirectX::SimpleMath::Vector3& position);

        // ビュー行列を設定
        void SetCameraMatrix();
    
    private:
        //-----定数集-----//
        // カメラの距離
        static constexpr float DEFAULT_CAMERA_DISTANCE = 6.0f;
        // カメラの距離(デバッグモード時)
        static constexpr float DEBUG_CAMERA_DISTANCE = 10.0f;
        // カメラが移動できる角度
        static constexpr float LIMIT = 89.9f;
        // 速さ
        static constexpr float SPEED = 13.0f;
        // 相対モード用の感度(1ピクセルあたりの回転量(ラジアン))
        static constexpr float SENSITIVITY = 0.003f;
        //----------//

        //-----メンバー変数-----//
        // マウスのX座標
        int m_x;
        // マウスのY座標
        int m_y;
        // 画面サイズに対する相対的なスケールX
        float m_sx;
        // 画面サイズに対する相対的なスケールY
        float m_sy;
        // 横回転量
        float m_xTmp;
        // 縦回転量
        float m_yTmp;
        // 横回転
        float m_yAngle;
        // 縦回転
        float m_xAngle;
        // カメラモードである
        bool m_isCameraMode;
        // 前回のコントロールキー状態（トグル用）
        bool m_prevControlPressed;
        // 生成されたビュー行列
        DirectX::SimpleMath::Matrix m_view;
        // 視点
        DirectX::SimpleMath::Vector3 m_eye;
        // 速度
        DirectX::SimpleMath::Vector3 m_velocity;
        // 注視点
        DirectX::SimpleMath::Vector3 m_target;
        // 上方向ベクトル
        DirectX::SimpleMath::Vector3 m_up;
        // マウストラッカー
        DirectX::Mouse::ButtonStateTracker m_tracker;
        //----------//

        //-----メンバー関数-----//
        // カメラ移動
        void Motion(float x, float y);
        // カメラの移動
        void DebugMotion(int x, int y);
        // カメラモードの時の移動
        void MoveCamera(float elapsedTime, DirectX::SimpleMath::Vector3 dir);
        // 画面サイズに対する相対的なスケールを記録
        void SetWindowSize(int& windowWidth, int& windowHeight);
        //----------//

    };
}