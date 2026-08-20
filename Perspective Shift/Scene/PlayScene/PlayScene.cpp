#include "pch.h"
#include "PlayScene.h"
#include"imgui/imgui.h"
#include "Windows.h"
#include <memory>
#include "SimpleMath.h"
#include "../../GameContext.h"
#include "ImaseLib/SceneManager.h"
#include "Scene/FadeInOut.h"
#include "Camera.h"
#include "Collision.h"
#include "Player.h"
#include "Stage.h"
#include "Scene/SceneId.h"

using namespace DirectX;

// 更新
void PlayScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
    // 経過時間を取得する
    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    // ステージの更新
    m_stage->Update(gameContext, elapsedTime);

	// フェードアウトが終わったかつフェードインしない
    if (m_fadeInOut->GetFedeOutEnd() && !m_canFadeIn)
    {
        //タイマーの後進
        m_timer->Update(elapsedTime);

        // カメラ更新
        m_camera->Update(gameContext, elapsedTime, m_player->GetCenterPosition());

        // カメラモードでないかつデバッグモードでないならプレイヤー更新
        if (!m_camera->GetCameraMode() && !gameContext.isDebugMode) m_player->Update(gameContext, elapsedTime, m_camera->GetEyePosition());

        // 当たり判定の更新
        m_collision->Update(gameContext);

        // ゲームクリアしたら
        if (m_collision->GetStageClear())
        {
            // フェードインしてよい
            m_canFadeIn = true;
        }
        // Rキーが押されたら
        if (gameContext.keyboardTracker.pressed.R)
        {
            // フェードインしてよい
            m_canFadeIn = true;
            // リトライする
            m_retry = true;
        }
        // ESCキーが押されたら
        if (gameContext.keyboardTracker.pressed.Escape)
        {
            // フェードインしてよい
            m_canFadeIn = true;
            // 前のシーンへいく
            m_backScene = true;
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
        // リトライでないかつ前のシーンに行かなければ次のシーンへ
        if(!m_retry && !m_backScene)sceneController.RequestSwitch(SceneId::Result);
        // リトライ
        else if(m_retry) sceneController.RequestSwitch(SceneId::Play);
        // 前のシーン
        else if(m_backScene) sceneController.RequestSwitch(SceneId::StageSelect);
    }
}

// 描画
void PlayScene::Render(GameContext& gameContext)
{
	gameContext;

    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    // ビュー行列を設定
    m_camera->SetCameraMatrix();

    // ビュー行列を取得
    m_view = m_camera->GetCameraMatrix();

    // デバッグモードならIMGUの表示
    if (gameContext.isDebugMode)
    {
        // GUIの作成
        std::string GuiName = "Stage" + std::to_string(gameContext.selectStage) + "Data";
        ImGui::Begin(GuiName.c_str());
        // セルデータを表示
        for (size_t i = 0; i < m_stage->GetCellDatas().size(); i++)
        {
            SimpleMath::Vector3 cellposition = m_stage->GetCellDatas()[i].stagePosition;
            std::string cellnumber = "Cell" + std::to_string(i);
            if (ImGui::TreeNodeEx(cellnumber.c_str()))
            {
                ImGui::DragFloat3("pos", &cellposition.x);
                // GUIでの変更を反映
                m_stage->SetCellPosition(i, cellposition);
                // このセルを消すボタン
                if (ImGui::Button("Delete"))
                {
                    m_stage->DeleteCell(i);
                }

                ImGui::TreePop();
            }
        }
        // セルを追加するボタン
        if (ImGui::Button("AddCell"))
        {
            m_stage->AddCell();
        }
        ImGui::End();

        // ステージデータの変更をセーブするボタン
        if (ImGui::Button("Save"))
        {
            gameContext.saveLoad->SaveData(gameContext.selectStage, m_stage->GetCellDatas());
        }

        // GUIでの変更を反映
        m_player->SetPosition(m_stage->GetPlayerSetPosition());
    }

    // プレイヤーの描画
    m_player->Render(gameContext, m_view, gameContext.projection);

    // ステージの描画
    m_stage->Render(gameContext, m_view, gameContext.projection, context);

    // 当たり判定の描画
    m_collision->Render(gameContext, m_view, gameContext.projection);

    //タイマーの描画
    m_timer->Render(gameContext);

    //カメラの描画
    m_camera->Render(gameContext);

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

    // 当たり判定の作成
    m_collision = std::make_unique<Yokoyama::Collision>(device, context, m_player.get(), m_stage.get(), m_camera.get());

    //タイマーの作成
    m_timer = std::make_unique<Yokoyama::Timer>(gameContext);

    // タイトルBGMのインスタンス作成
    m_playBGMInstance = gameContext.playBGM->CreateInstance();

    // BGM再生(ループ)
    m_playBGMInstance->Play(true);
}
