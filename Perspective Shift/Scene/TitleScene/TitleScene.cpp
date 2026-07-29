#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;

// 更新
void TitleScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	//ESCキーでプログラムを終了
	if (gameContext.keyboardTracker.pressed.Escape) PostQuitMessage(0);

	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"TitleScene");
	
	//タイトルロゴの更新
	m_titleLogo->Update(elapsedTime);

	//背景ステージの更新
	m_titleStage->Update(elapsedTime);

	//フェードアウトが終わったかつフェードインしない
	if (m_fadeInOut->GetFedeOutEnd() && !m_canFadeIn)
	{
		//いろいろアプデ

		//スペースキーが押されたら
		if (gameContext.keyboardTracker.pressed.Space)
		{
			//フェードインしてよい
            m_canFadeIn = true;
		}
	}
	else
	{
		//フェードアウト更新
        m_fadeInOut->FedeOutUpdate(elapsedTime, 1.0f, m_titleBGMInstance.get());
	}

	//フェードインしてよい
	if (m_canFadeIn)
	{
		//フェードイン更新
        m_fadeInOut->FedeInUpdate(elapsedTime, 1.0f, m_titleBGMInstance.get());
	}

	//フェードインし終わった
	if (m_fadeInOut->GetFedeInEnd())
	{
		//次のシーンへ
        sceneController.RequestSwitch(SceneId::StageSelect);
	}
}

// 描画
void TitleScene::Render(GameContext& gameContext)
{
	//背景ステージの描画
	m_titleStage->Rnder(gameContext);

	//タイトルロゴの描画
	m_titleLogo->Render(gameContext);

	//フェードイン描画
    m_fadeInOut->FedeInRender(gameContext);
	//フェードアウト描画
    m_fadeInOut->FedeOutRnder(gameContext);
}

// シーン切り替え時に呼び出される関数
void TitleScene::OnEnter(GameContext& gameContext)
{
	gameContext;

	//フェードインをしてはいけない
    m_canFadeIn = false;

	//フェードインアウトを作成
    m_fadeInOut = std::make_unique<Yokoyama::FadeInOut>();

	//タイトルロゴの作成
	m_titleLogo = std::make_unique<Yokoyama::TitleLogo>(gameContext);

	//背景ステージの作成
	m_titleStage = std::make_unique<Yokoyama::TitleStage>();

	//タイトルBGMのインスタンス作成
	m_titleBGMInstance = gameContext.titleBGM->CreateInstance();

	//BGM再生(ループ)
	m_titleBGMInstance->Play(true);
}
