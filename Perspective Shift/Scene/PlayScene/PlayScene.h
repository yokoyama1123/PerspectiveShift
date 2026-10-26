#pragma once

#include "../../ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"
#include"Scene/FadeInOut.h"
#include "Collision.h"
#include "Camera.h"
#include "Player.h"
#include "Stage.h"
#include "Timer.h"
#include "StageBlocks.h"

class PlayScene : public Imase::SceneBase<SceneId, GameContext>
{
public:
	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:

    //ゲームオーバーとなるY座標
    static constexpr float DEATH_POSITION = -30.0f;

    //ゲームオーバー時のフェードインの時間
    static constexpr float DEATH_FADE_TIME = 2.0f;

    //開始時の目の位置
    static constexpr DirectX::SimpleMath::Vector3 START_EYE = { 30, 30, -30 };

    //開始時のターゲットの位置
    static constexpr DirectX::SimpleMath::Vector3 START_TARGET = { 15, 0, -15 };

    //フェードインにかける時間
    float m_FadeInTime = 1.0f;

    //フェードアウトにかける時間
    float m_FadeOutTime = 1.0f;

    // フェードインをしてよい
    bool m_canFadeIn = false;

    // リトライする
    bool m_retry = false;

    // 前のシーンへいく
    bool m_backScene = false;

    // ビュー行列
    DirectX::SimpleMath::Matrix m_view;

    // フェードインアウト
    std::unique_ptr<Yokoyama::FadeInOut> m_fadeInOut;

    // カメラ
    std::unique_ptr<Yokoyama::Camera> m_camera;

    // プレイヤー
    std::unique_ptr<Yokoyama::Player> m_player;

    // ステージ
    std::unique_ptr<Yokoyama::Stage> m_stage;

    // 当たり判定
    std::unique_ptr<Yokoyama::Collision> m_collision;

    //タイマー
    std::unique_ptr<Yokoyama::Timer> m_timer;

    // BGMのインスタンス
    std::unique_ptr<DirectX::SoundEffectInstance> m_playBGMInstance;

    //ステージ開始時にあるブロック
    std::unique_ptr<Yokoyama::StageBlocks> m_stageBlocks;
};

