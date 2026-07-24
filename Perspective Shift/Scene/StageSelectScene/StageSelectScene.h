#pragma once

#include "../../ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include"Scene/FadeInOut.h"

class StageSelectScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:
	//フェードインをしてよい
    bool m_canFadeIn;

	//ステージ数
	int m_stageCount;

	//現在選択しているステージ
	int m_selectStage;

	//フェードインアウト
    std::unique_ptr<Yokoyama::FadeInOut> m_fadeInOut;

	//BGMのインスタンス
	std::unique_ptr<DirectX::SoundEffectInstance> m_titleBGMInstance;
};

