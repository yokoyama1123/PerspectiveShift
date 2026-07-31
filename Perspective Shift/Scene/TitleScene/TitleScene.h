#pragma once
#include "../../ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include"Scene/FadeInOut.h"

#include"TitleLogo.h"
#include"TitleStage.h"

class TitleScene: public Imase::SceneBase<SceneId, GameContext>
{
public:
	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:
	// フェードインをしてよい
    bool m_canFadeIn = false;

	// フェードインアウト
    std::unique_ptr<Yokoyama::FadeInOut> m_fadeInOut;

	// タイトルロゴ
	std::unique_ptr<Yokoyama::TitleLogo> m_titleLogo;

	// 背景のステージ
	std::unique_ptr<Yokoyama::TitleStage> m_titleStage;

	// BGMのインスタンス
	std::unique_ptr<DirectX::SoundEffectInstance> m_titleBGMInstance;
};

