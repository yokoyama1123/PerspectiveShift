#include "pch.h"
#include "ResultScene.h"

using namespace DirectX;

// 更新
void ResultScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
    // 経過時間を取得する
    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    // デバッグモードをOFFにする
    gameContext.isDebugMode = false;

    //ステージクリアの更新
    m_stageClear->Update(elapsedTime);

	// フェードアウトが終わったかつフェードインしない
    if (m_fadeInOut->GetFedeOutEnd() && !m_canFadeIn)
    {
        // いろいろアプデ

        // 何かしらの条件
        if (gameContext.keyboardTracker.pressed.Space)
        {
            // フェードインしてよい
            m_canFadeIn = true;
        }
    }
    else
    {
        // フェードアウト更新
        m_fadeInOut->FedeOutUpdate(elapsedTime, 1.0f, m_titleBGMInstance.get());
    }

    // フェードインしてよい
    if (m_canFadeIn)
    {
        // フェードイン更新
        m_fadeInOut->FedeInUpdate(elapsedTime, 1.0f, m_titleBGMInstance.get());
    }

    // フェードインし終わった
    if (m_fadeInOut->GetFedeInEnd())
    {
        // 次のシーンへ
        sceneController.RequestSwitch(SceneId::Title);
    }
}

// 描画
void ResultScene::Render(GameContext& gameContext)
{
    //背景の描画
    m_resultBackground->Render(gameContext);

    //ステージクリアの描画
    m_stageClear->Render(gameContext);

    // フェードイン描画
    m_fadeInOut->FedeInRender(gameContext);
    // フェードアウト描画
    m_fadeInOut->FedeOutRnder(gameContext);
}

// シーン切り替え時に呼び出される関数
void ResultScene::OnEnter(GameContext& gameContext)
{
	gameContext;
    // フェードインをしてはいけない
    m_canFadeIn = false;

    //ステージクリアの作成
    m_stageClear = std::make_unique<Yokoyama::StageClearText>(gameContext);

    // 背景の作成
    m_resultBackground = std::make_unique<Yokoyama::ResultBackground>(gameContext);

    // フェードインアウトを作成
    m_fadeInOut = std::make_unique<Yokoyama::FadeInOut>();

    // タイトルBGMのインスタンス作成
    m_titleBGMInstance = gameContext.titleBGM->CreateInstance();

    // BGM再生(ループ)
    m_titleBGMInstance->Play(true);
}
