#pragma once

#include "../../ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include"Scene/FadeInOut.h"
#include"Scene/PlayScene/Stage.h"

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
	// カメラの位置
	static constexpr DirectX::SimpleMath::Vector3 EYE = { 0,30,0 };

	// ターゲットの位置
	static constexpr DirectX::SimpleMath::Vector3 TARGET = { 15, 15, -15 };

	//フェードインにかける時間
	float m_FadeInTime = 1.0f;

	//フェードアウトにかける時間
	float m_FadeOutTime = 1.0f;

	// フェードインをしてよい
    bool m_canFadeIn = false;

	// 前のシーンへいく
	bool m_backScene = false;

	// ステージ数
	int m_stageCount = 0;

	// 現在選択しているステージ
	int m_selectStage = 0;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// ステージ
	std::unique_ptr<Yokoyama::Stage> m_stage;

	// フェードインアウト
    std::unique_ptr<Yokoyama::FadeInOut> m_fadeInOut;

	// BGMのインスタンス
	std::unique_ptr<DirectX::SoundEffectInstance> m_titleBGMInstance;
};

