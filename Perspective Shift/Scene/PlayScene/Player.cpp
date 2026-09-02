#include"pch.h"
#include "Player.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Yokoyama::Player::Player(DirectX::SimpleMath::Vector3 position)
    : m_position{ position }
    , m_velocity{}
    , m_scale{SCALE}
    , m_angle{}
    , m_time{}
    , m_elapsedTime{}
    , m_canJump{false}
{
    //中心からの距離を設定(当たり判定)
    m_boundingBox.Extents = DISTANCE;
}

/// <summary>
/// プレイヤーの更新
/// </summary>
/// <param name="context">gameContext</param>
/// <param name="elapsedTime">elapsedTime</param>
/// <param name="eyePosition">eyePosition</param>
void Yokoyama::Player::Update(const GameContext& gameContext, float elapsedTime, const DirectX::SimpleMath::Vector3& eyePosition)
{
    m_elapsedTime = elapsedTime;

    // ワールド行列を作成する
    m_world = SimpleMath::Matrix::CreateTranslation(m_position);

    // カメラからプレイヤーまでのベクトルを計算
    DirectX::SimpleMath::Vector3 direction = eyePosition - m_position;
    // y方向を無視
    direction.y = 0.0f;
    // ベクトルを正規化
    direction.Normalize();

    // 速さを初期化
    m_velocity.x = 0.0f;
    m_velocity.z = 0.0f;


    // プレイヤーの方向ベクトルを計算
    SimpleMath::Vector3 forward(m_world.m[2][0], m_world.m[2][1], m_world.m[2][2]); // 前方向
    SimpleMath::Vector3 right(m_world.m[0][0], m_world.m[0][1], m_world.m[0][2]);   // 右方向
    SimpleMath::Vector3 backward = -forward;                                        // 後ろ
    SimpleMath::Vector3 left = -right;                                              // 左

    // 斜め4方向（forward と right を足して正規化）
    SimpleMath::Vector3 forwardRight  = forward + right;    // 右前
    SimpleMath::Vector3 forwardLeft   = forward - right;    // 左前
    SimpleMath::Vector3 backwardRight = backward + right;   // 右後ろ
    SimpleMath::Vector3 backwardLeft  = backward - right;   // 左後ろ

    // ベクトルを正規化
    forward.Normalize();
    right.Normalize();
    backward.Normalize();
    left.Normalize();
    forwardRight.Normalize();
    forwardLeft.Normalize();
    backwardRight.Normalize();
    backwardLeft.Normalize();

    // キーボードによるプレイヤーの動き
    if (Keyboard::Get().GetState().W && Keyboard::Get().GetState().D)       // WDキー(右前方向)
    {
        Move(forwardRight, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().W && Keyboard::Get().GetState().A)  // WAキー(左前方向)
    {
        Move(forwardLeft, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().S && Keyboard::Get().GetState().D)  // SDキー(右後ろ方向)
    {
        Move(backwardRight, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().S && Keyboard::Get().GetState().A)  // SAキー(左後ろ方向)
    {
        Move(backwardLeft, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().W)                                  // Wキー(前方向)
    {
        Move(forward, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().S)                                  // Sキー(後ろ方向)
    {
        Move(backward, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().D)                                  // Dキー(右方向)
    {   
        Move(right, direction, forward, elapsedTime);
    }
    else if (Keyboard::Get().GetState().A)                                  // Aキー(左方向)
    {   
        Move(left, direction, forward, elapsedTime);
    }

    // スペースキーでジャンプ
    if (Keyboard::Get().GetState().Space && m_canJump)
    {
        m_canJump = false;
        m_velocity.y += JUMP;

        // ジャンプの効果音のインスタンス作成
        m_jumpSoundInstance = gameContext.jumpSound->CreateInstance();

        // 効果音の再生
        m_jumpSoundInstance->Play(false);
    }

    // 呼吸
    BreathingMove(elapsedTime);

    // 重力を加算
    m_velocity.y += -GRAVITY * elapsedTime;

    // 位置に速さを加算
    m_position += m_velocity * elapsedTime;

    // 中心座標の更新
    m_boundingBox.Center = GetCenterPosition();
}

/// <summary>
/// プレイヤーの描画
/// </summary>
/// <param name="context">gameContext</param>
void Yokoyama::Player::Render(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
    // 行列を反映
    m_world = SimpleMath::Matrix::CreateScale(m_scale) *          // 大きさ
              SimpleMath::Matrix::CreateRotationY(m_angle) *    // 回転
              SimpleMath::Matrix::CreateTranslation(m_position);// 移動

    // プレイヤーのモデルの描画
    gameContext.playerModel->Draw(
        gameContext.deviceResources.GetD3DDeviceContext(),
        gameContext.commonStates, 
        m_world,                     
        view, 
        projection);
}

/// <summary>
/// 重力の取得
/// </summary>
/// <returns>重力</returns>
float Yokoyama::Player::GetGravity() const
{
    return GRAVITY;
}

/// <summary>
/// プレイヤーのy0の位置を取得
/// </summary>
/// <returns>プレイヤーのy0の位置</returns>
DirectX::SimpleMath::Vector3 Yokoyama::Player::GetPosition() const
{
    return m_position;
}

/// <summary>
/// プレイヤーの中心の位置を取得
/// </summary>
/// <returns>プレイヤーの中心の位置</returns>
DirectX::SimpleMath::Vector3 Yokoyama::Player::GetCenterPosition() const
{
    return DirectX::SimpleMath::Vector3(m_position.x, m_position.y + SCALE, m_position.z);
}

/// <summary>
/// 速さを取得
/// </summary>
/// <returns>速さ</returns>
DirectX::SimpleMath::Vector3 Yokoyama::Player::GetVelocity() const
{
    return m_velocity * m_elapsedTime;
}

/// <summary>
/// AABBを取得
/// </summary>
/// <returns>当たり判定</returns>
const DirectX::BoundingBox& Yokoyama::Player::GetBoundingBox()
{
    // 中心座標の更新
    m_boundingBox.Center = GetCenterPosition();
    return m_boundingBox;
}

/// <summary>
/// 位置を設定
/// </summary>
/// <param name="position">設定したい位置</param>
void Yokoyama::Player::SetPosition(const SimpleMath::Vector3& position)
{
    m_position = position;
}

/// <summary>
/// 速さを設定
/// </summary>
/// <param name="velocity">設定したい速さ</param>
void Yokoyama::Player::SetVelocity(const DirectX::SimpleMath::Vector3& velocity)
{
    m_velocity = velocity / m_elapsedTime;
}

/// <summary>
/// ジャンプできるかの設定
/// </summary>
/// <param name="canJump">ジャンプできるか</param>
void Yokoyama::Player::SetCanJump(bool canJump)
{
    m_canJump = canJump;
}

/// <summary>
/// プレイヤーの移動
/// </summary>
/// <param name="orientation">向きたい方向の単位ベクトル</param>
/// <param name="direction">カメラからプレイヤー方向の単位ベクトル</param>
/// <param name="forward">プレイヤーの正面方向の単位ベクトル</param>
/// <param name="elapsedTime">elapsedTime</param>
void Yokoyama::Player::Move(SimpleMath::Vector3 orientation, SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 forward, float elapsedTime)
{
    float dot{};
    // 回転軸（y軸）
    DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::UnitY;
    dot = orientation.Dot(direction);
    dot = std::clamp(dot, -1.0f, 1.0f); // 数値誤差対策
    m_angle = std::acos(dot);

    // 外積と axis の内積で符号を決める（回転方向）
    DirectX::SimpleMath::Vector3 cross = orientation.Cross(direction);
    if (cross.Dot(axis) < 0.0f)
    {
        m_angle = -m_angle;
    }

    // 正面方向ベクトル(正規化済み)にスピードとエラペストタイムをかけたものをポジションに加算
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_angle);
    
    // 速さの計算
    m_velocity.x = -(SimpleMath::Vector3::Transform(forward, rotY) * SPEED).x;
    m_velocity.z = -(SimpleMath::Vector3::Transform(forward, rotY) * SPEED).z;

}

void Yokoyama::Player::BreathingMove(float elapsedTime)
{
    //空中にいる
    if (m_velocity.y != 0)
    {
        m_time = CYCLE * 0.75;

        //縦に伸ばす
        m_scale.x = SCALE - AMPLITUDE / 2;
        m_scale.z = SCALE - AMPLITUDE / 2;
        m_scale.y = SCALE + AMPLITUDE;
    }
    else
    {
        // 時間を更新
        m_time += elapsedTime;

        if (m_time >= CYCLE)m_time = 0;

        // サイン波により大きさを調節
        float offset = AMPLITUDE * sinf(m_time * 2.0f * PI / CYCLE);

        m_scale.x = SCALE + offset / 2;
        m_scale.z = SCALE + offset / 2;
        m_scale.y = SCALE - offset;
    }
}
