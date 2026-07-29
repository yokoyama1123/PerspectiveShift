#include "pch.h"
#include "StageSelectScene.h"

using namespace DirectX;

// 更新
void StageSelectScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;
	debugRenderer.DrawText({ 0.0f, 0.0f }, L"StageSelectScene");

	//フェードアウトが終わったかつフェードインしない
	if (m_fadeInOut->GetFedeOutEnd() && !m_canFadeIn)
	{
		//いろいろアプデ
		// Aキーが押されたら選択しているステージ番号を-1
		if (gameContext.keyboardTracker.pressed.A)
		{
			m_selectStage--;
			//ステージ0の時に押したらステージ最大に行く
			if (m_selectStage < 0)m_selectStage = m_stageCount - 1;
		}
		// Dキーが押されたら選択しているステージ番号を+1
		if (gameContext.keyboardTracker.pressed.D)
		{
			m_selectStage++;
			//ステージ最大の時に押したらステージ0に行く
			if (m_selectStage >= m_stageCount)m_selectStage = 0;
		}
		//現在選択しているステージの表示
		std::wstring stage = L"Stage:" + std::to_wstring(m_selectStage);
		debugRenderer.DrawText({ Yokoyama::Screen::CENTER_X, Yokoyama::Screen::CENTER_Y }, stage);

		// スペースキーが押されたら
        if (gameContext.keyboardTracker.pressed.Space)
		{
			//Ctrlキーを押していたらデバッグモードON
			if (Keyboard::Get().GetState().LeftControl)
			{
				gameContext.isDebugMode = true;
			}

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
	//ESCキーが押されたら
	if (gameContext.keyboardTracker.pressed.Escape)
	{
		//フェードインしてよい
		m_canFadeIn = true;
		//前のシーンへいく
		m_backScene = true;
	}

	//フェードインし終わった
	if (m_fadeInOut->GetFedeInEnd())
	{
		//選んだステージを記録
		gameContext.selectStage = m_selectStage;
		//次のシーンへ
		if (!m_backScene) sceneController.RequestSwitch(SceneId::Play);
		//前のシーン
		else sceneController.RequestSwitch(SceneId::Title);
	}
}

// 描画
void StageSelectScene::Render(GameContext& gameContext)
{
	gameContext;



	//フェードイン描画
    m_fadeInOut->FedeInRender(gameContext);
	//フェードアウト描画
    m_fadeInOut->FedeOutRnder(gameContext);
}

// シーン切り替え時に呼び出される関数
void StageSelectScene::OnEnter(GameContext& gameContext)
{
	gameContext;
	//フェードインをしてはいけない
    m_canFadeIn = false;

	//ステージ数を記録
	m_stageCount = static_cast<int>(gameContext.stages.size());

	//現在選択しているステージ
	m_selectStage = gameContext.selectStage;

	//フェードインアウトを作成
    m_fadeInOut = std::make_unique<Yokoyama::FadeInOut>();

	//タイトルBGMのインスタンス作成
	m_titleBGMInstance = gameContext.titleBGM->CreateInstance();

	//BGM再生(ループ)
	m_titleBGMInstance->Play(true);
}
