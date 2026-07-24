#include "pch.h"
#include "PlayScene.h"

using namespace DirectX;

// 更新
void PlayScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
    // 経過時間を取得する
    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    //カメラ更新
    m_camera->Update(gameContext, elapsedTime, m_player->GetCenterPosition());

    //ステージの更新
    m_stage->Update(gameContext, elapsedTime);

	// フェードアウトが終わったかつフェードインしない
    if (m_fadeInOut->GetFedeOutEnd() && !m_canFadeIn)
    {

        // カメラモードでないならプレイヤー更新
        if(!m_camera->GetCameraMode()) m_player->Update(gameContext, elapsedTime, m_camera->GetEyePosition());

        //当たり判定の更新
        m_collision->Update(gameContext);

        // 何かしらの条件
        if (m_collision->GetStageClear())
        {
            // フェードインしてよい
            m_canFadeIn = true;
        }
    }
    else
    {
        // フェードアウト更新
        m_fadeInOut->FedeOutUpdate(elapsedTime, 1.0f, m_playBGMInstance.get());
    }

    // フェードインしてよい
    if (m_canFadeIn)
    {
        // フェードイン更新
        m_fadeInOut->FedeInUpdate(elapsedTime, 1.0f, m_playBGMInstance.get());
    }

    // フェードインし終わった
    if (m_fadeInOut->GetFedeInEnd())
    {
        // 次のシーンへ
        sceneController.RequestSwitch(SceneId::Result);
    }
}

// 描画
void PlayScene::Render(GameContext& gameContext)
{
	gameContext;

    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    //ビュー行列を設定
    m_camera->SetCameraMatrix();
    // ビュー行列を取得
    m_view = m_camera->GetCameraMatrix();

    // プレイヤーの描画
    m_player->Render(gameContext, m_view, gameContext.projection);

    //ステージの描画
    m_stage->Render(gameContext, m_view, gameContext.projection, context);

    //当たり判定の描画
    m_collision->Render(gameContext, m_view, gameContext.projection);

    Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

    debugRenderer.DrawText({ 0.0f, 0.0f }, L"PlayScene");

	// フェードイン描画
    m_fadeInOut->FedeInRender(gameContext);
    // フェードアウト描画
    m_fadeInOut->FedeOutRnder(gameContext);
}

// シーン切り替え時に呼び出される関数
void PlayScene::OnEnter(GameContext& gameContext)
{
   	// DirectX3Dのデバイスを取得する
    auto device = gameContext.deviceResources.GetD3DDevice();

    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    // フェードインをしてはいけない
    m_canFadeIn = false;

    // フェードインアウトを作成
    m_fadeInOut = std::make_unique<Yokoyama::FadeInOut>();

    // 画面のサイズを取得する
    RECT rect = gameContext.deviceResources.GetOutputSize();

    // カメラを作成
    m_camera = std::make_unique<Yokoyama::Camera>(static_cast<float>(rect.right), static_cast<float>(rect.bottom));

    // ステージの作成
    m_stage = std::make_unique<Yokoyama::Stage>(device, context, gameContext);

    // プレイヤーを作成
    m_player = std::make_unique<Yokoyama::Player>(m_stage->GetPlayerSetPosition());

    //当たり判定の作成
    m_collision = std::make_unique<Yokoyama::Collision>(device, context, m_player.get(), m_stage.get(), m_camera.get());

    //タイトルBGMのインスタンス作成
    m_playBGMInstance = gameContext.playBGM->CreateInstance();

    //BGM再生(ループ)
    m_playBGMInstance->Play(true);
}
