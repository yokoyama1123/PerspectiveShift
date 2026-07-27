#include "pch.h"
#include "Camera.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Yokoyama::Camera::Camera(int windowWidth, int windowHeight)
    : m_yAngle{0.0f}
    , m_xAngle{0.0f}
    , m_isCameraMode{false}
    , m_view{}
    , m_eye{0.0f, 0.0f, 0.0f}
    , m_target{}
    , m_up{0.0f, 1.0f, 0.0f}
    , m_velocity{}
    , m_prevControlPressed{false}
{
    // マウスを相対モードに設定（カーソル非表示＆中央固定）
    Mouse::Get().SetMode(Mouse::MODE_RELATIVE);
    Mouse::Get().ResetScrollWheelValue();
}

/// <summary>
/// 更新関数
/// </summary>
void Yokoyama::Camera::Update(GameContext& gameContext, float elapsedTime, const DirectX::SimpleMath::Vector3& target)
{
    // 速度の初期化
    m_velocity = SimpleMath::Vector3::Zero;

    // カメラモードの切り替え（左コントロールキー）
    if (gameContext.keyboardTracker.pressed.LeftControl)
    {
        m_isCameraMode = !m_isCameraMode;
    }
    //デバッグモードならカメラモードである
    if (gameContext.isDebugMode) m_isCameraMode = true;

    // マウスのステートを取得
    auto state = Mouse::Get().GetState();

    // トラッカー更新
    m_tracker.Update(state);

    // カメラの回転を更新（相対モード：state.x, state.y は移動量）
    Motion(state.x, state.y);

    // 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_yAngle);
    SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_xAngle);
    SimpleMath::Matrix rt = rotY * rotX;

    // 上方向ベクトルの設定
    m_up = SimpleMath::Vector3{ 0.0f, 1.0f, 0.0f };
    m_up = SimpleMath::Vector3::Transform(m_up, rt.Invert());


    // カメラモードによるそれぞれの動き
    if (!m_isCameraMode)
    {
        // プレイヤー追従モード
        // カメラの位置の変数作成
        SimpleMath::Vector3 eye(0.0f, 0.0f, 1.0f);
        // カメラの位置を設定
        eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
        // プレイヤーから一定の距離離す
        eye *= DEFAULT_CAMERA_DISTANCE;

        // 最終的なカメラの位置に反映
        m_eye = target + eye;

        // ターゲットの位置の記録更新
        m_target = target;
    }
    else
    {
        // 自由移動モード
        SimpleMath::Vector3 lookDir(0.0f, 0.0f, -1.0f);
        lookDir = SimpleMath::Vector3::Transform(lookDir, rt.Invert());

        // 見ている方向ベクトル（Y方向無視）
        SimpleMath::Vector3 dir = lookDir;
        dir.y = 0;
        dir.Normalize();

        // カメラの位置更新（WASD + Space/Shift）
        MoveCamera(elapsedTime, dir);

        // ターゲットの更新
        m_target = m_eye + lookDir;
    }
}

/// <summary>
/// カメラの位置を返す関数
/// </summary>
DirectX::SimpleMath::Vector3 Yokoyama::Camera::GetEyePosition() const
{
    return m_eye;
}

/// <summary>
/// 注視点を返す関数
/// </summary>
DirectX::SimpleMath::Vector3 Yokoyama::Camera::GetTargetPosition() const
{
    return m_target;
}

/// <summary>
/// 速さ取得
/// </summary>
DirectX::SimpleMath::Vector3 Yokoyama::Camera::GetVelocity() const
{
    return m_velocity;
}

/// <summary>
/// ビュー行列を返す関数
/// </summary>
DirectX::SimpleMath::Matrix Yokoyama::Camera::GetCameraMatrix() const
{
    return m_view;
}

/// <summary>
/// カメラモードを返す関数
/// </summary>
bool Yokoyama::Camera::GetCameraMode() const
{
    return m_isCameraMode;
}

/// <summary>
/// 位置を設定
/// </summary>
/// <param name="position">設定したいカメラの位置</param>
void Yokoyama::Camera::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
    m_eye = position;
}

/// <summary>
/// ビュー行列を設定
/// </summary>
void Yokoyama::Camera::SetCameraMatrix()
{
    if(m_eye != SimpleMath::Vector3::Zero)m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

/// <summary>
/// マウスによる回転処理（相対モード対応）
/// </summary>
void Yokoyama::Camera::Motion(float x, float y)
{
    // 相対モードでは x, y が移動量
    float dx = static_cast<float>(x);
    float dy = static_cast<float>(y);

    if (dx != 0.0f || dy != 0.0f)
    {
        // Y軸回転（左右）は制限なし
        m_yAngle += dx * SENSITIVITY;

        // X軸回転（上下）は角度制限をかける
        m_xAngle += dy * SENSITIVITY;

        // 角度制限
        constexpr float radLimit = XMConvertToRadians(LIMIT);
        if (m_xAngle > radLimit)
        {
            m_xAngle = radLimit;
        }
        else if (m_xAngle < -radLimit)
        {
            m_xAngle = -radLimit;
        }
    }
}

/// <summary>
/// カメラモード時のカメラ自身の移動
/// </summary>
void Yokoyama::Camera::MoveCamera(float elapsedTime, DirectX::SimpleMath::Vector3 dir)
{


    // 横方向ベクトル
    SimpleMath::Vector3 hor = dir.Cross(SimpleMath::Vector3::UnitY);

    // キー入力を取得
    auto kb = Keyboard::Get().GetState();

    // WASDで移動,Spaceで上昇,左Shiftで下降
    if (kb.W)m_velocity += dir;
    if (kb.S)m_velocity -= dir;
    if (kb.D)m_velocity += hor;
    if (kb.A)m_velocity -= hor;
    if (kb.Space)m_velocity += SimpleMath::Vector3::UnitY;
    if (kb.LeftShift)m_velocity -= SimpleMath::Vector3::UnitY;

    // 正規化してスピードをかける
    if (m_velocity.LengthSquared() > 0.0f)
    {
        m_velocity.Normalize();
        m_velocity *= SPEED;
    }

    // 位置に速度を加算
    m_eye += m_velocity * elapsedTime;
}