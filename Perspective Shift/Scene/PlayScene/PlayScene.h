#pragma once

#include "../../ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include"Scene/FadeInOut.h"

#include "Collision.h"
#include "Camera.h"
#include "Player.h"
#include "Stage.h"

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
    // ビュー行列
    DirectX::SimpleMath::Matrix m_view;

    // フェードインをしてよい
    bool m_canFadeIn = false;

    //リトライする
    bool m_retry = false;

    //前のシーンへいく
    bool m_backScene = false;

    // フェードインアウト
    std::unique_ptr<Yokoyama::FadeInOut> m_fadeInOut;

    //カメラ
    std::unique_ptr<Yokoyama::Camera> m_camera;

    // プレイヤー
    std::unique_ptr<Yokoyama::Player> m_player;

    //ステージ
    std::unique_ptr<Yokoyama::Stage> m_stage;

    //当たり判定
    std::unique_ptr<Yokoyama::Collision> m_collision;

    //BGMのインスタンス
    std::unique_ptr<DirectX::SoundEffectInstance> m_playBGMInstance;
};

