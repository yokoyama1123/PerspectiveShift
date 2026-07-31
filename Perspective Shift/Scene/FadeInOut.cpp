#include "pch.h"
#include "FadeInOut.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Yokoyama::FadeInOut::FadeInOut() 
    : m_fedeInEnd{false}
    , m_fedeOutEnd{false}
    , m_fadeInCount{0.0f}
    , m_fadeOutCount{0.0f}
    , m_fadeInPercent{0.0f}
    , m_fadeOutPercent{0.0f}
{
}

/// <summary>
/// フェードインの更新関数
/// </summary>
/// <param name="fadeInSecond">フェードインにかけたい時間</param>
/// <param name="bgm">流れているBGM</param>
void Yokoyama::FadeInOut::FedeInUpdate(float elapsedTime, float fadeInSecond, SoundEffectInstance* bgm)
{
    // カウントを加算
    m_fadeInCount += 1.0f * elapsedTime;

    // 今の進行割合を計算
    m_fadeInPercent = m_fadeInCount / fadeInSecond;

    // 音量の調整
    if(bgm != nullptr) bgm->SetVolume(1.0f - m_fadeInPercent);

    // 進行度が100%以上なら終了
    if(m_fadeInPercent >= 1.0f) m_fedeInEnd = true;
}

/// <summary>
/// フェードアウトの更新関数
/// </summary>
/// <param name="fadeOutSecond">フェードアウトにかけたい時間</param>
/// <param name="bgm">流れているBGM</param>
void Yokoyama::FadeInOut::FedeOutUpdate(float elapsedTime, float fadeOutSecond, SoundEffectInstance* bgm)
{
    // カウントを加算
    m_fadeOutCount += 1.0f * elapsedTime;

    // 今の進行割合を計算
    m_fadeOutPercent = m_fadeOutCount / fadeOutSecond;

    // 音量の調整
    if (bgm != nullptr) bgm->SetVolume(m_fadeOutPercent);

    // 進行度が100%以上なら終了
    if(m_fadeOutPercent >= 1.0f) m_fedeOutEnd = true;
}

/// <summary>
/// フェードインの描画関数
/// </summary>
void Yokoyama::FadeInOut::FedeInRender(GameContext& gameContext)
{
    gameContext.spriteBatch.Begin(
        DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
        gameContext.commonStates.NonPremultiplied(),           // 半透明
        gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
    );

    // 画面全体を覆う黒いスプライト
    RECT rect = { 0, 0, Yokoyama::Screen::WIDTH, Yokoyama::Screen::HEIGHT};
    gameContext.spriteBatch.Draw(
        gameContext.fadeInOutTexture.Get(),
        rect,
        nullptr,
        SimpleMath::Color{ 0.0f, 0.0f, 0.0f, m_fadeInPercent }  // 黒、alphaで透明度
    );

    gameContext.spriteBatch.End();
}

/// <summary>
/// フェードアウトの描画関数
/// </summary>
void Yokoyama::FadeInOut::FedeOutRnder(GameContext& gameContext)
{
    gameContext.spriteBatch.Begin(
        DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
        gameContext.commonStates.NonPremultiplied(),           // 半透明
        gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
    );

    // 画面全体を覆う黒いスプライト
    RECT rect = { 0, 0, Yokoyama::Screen::WIDTH, Yokoyama::Screen::HEIGHT };
    gameContext.spriteBatch.Draw(
        gameContext.fadeInOutTexture.Get(),
        rect,
        nullptr,
        SimpleMath::Color{ 0.0f, 0.0f, 0.0f, 1.0f - m_fadeOutPercent }  // 黒、alphaで透明度
    );

    gameContext.spriteBatch.End();
}

/// <summary>
/// シーン切り替え時に呼び出される関数
/// </summary>
void Yokoyama::FadeInOut::OnEnter()
{
    m_fedeInEnd = false;
    m_fedeOutEnd = false;
}

/// <summary>
/// フェードインし終わったかを返す関数
/// </summary>
/// <returns>フェードインし終わった</returns>
bool Yokoyama::FadeInOut::GetFedeInEnd()
{
    return m_fedeInEnd;
}

bool Yokoyama::FadeInOut::GetFedeOutEnd()
{
    return m_fedeOutEnd;
}
