//
// Game.h
//

#pragma once

#include "Common/DeviceResources.h"
#include "Common/StepTimer.h"

#include <memory>
#include <optional>

#include "ImaseLib/DebugRenderer.h"

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "Scene/SceneId.h"
#include "Save&Load.h"




// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // プロジェクション行列を作成する関数
    DirectX::SimpleMath::Matrix CreateProjectionMatrix();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // プロゼクション行列
    DirectX::SimpleMath::Matrix m_projection;

    // ----- モデル ----- //
    // プレイヤーのモデル
    std::unique_ptr<DirectX::Model> m_mPlayer;
    // ステージの外枠モデル
    std::vector<std::unique_ptr<DirectX::Model>> m_mStages;
    // ゴール
    std::unique_ptr<DirectX::Model> m_mGoal;
    //----------//

    //-----テクスチャハンドル-----//
    // 板ポリゴンのテクスチャ
    std::wstring m_textureHandle{};
    // タイトルロゴのテクスチャ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titlelogo;
    // フェードインアウトのテクスチャ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fadeInOutTexture;
    // スタートを促すテキストのテクスチャ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_startTextTexture;
    //数字のテクスチャ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numbersTexture;
    //----------//

    //-----サウンドハンドル-----//
    // タイトルシーンのBGM
    std::unique_ptr<DirectX::SoundEffect> m_titleBGM;
    // プレイシーンのBGM
    std::unique_ptr<DirectX::SoundEffect> m_palyBGM;
    //----------//

    // セーブロード
    std::unique_ptr<Yokoyama::SaveLoad> m_saveLoad;

    // --------------------------------------------------------------------- //

private:
    // 音量(1~0)
    static constexpr float SOUND_VOLUME = 0.2f;

    // imguiのフォントサイズ
    static constexpr float IMGUI_FONTSIZE = 20.0f;

    // キーボードトラッカー
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
    
    // オーディオエンジン
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

    // コモンステート
    std::unique_ptr<DirectX::CommonStates> m_states;

    // デバッグ用の描画セット
    std::unique_ptr<Imase::DebugRenderer> m_debugRenderer;

    // ゲームコンテキスト
    std::optional<GameContext> m_gameContext;

    // スプライトバッチ
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

    // シーンマネージャー
    Imase::SceneManager<SceneId, GameContext> m_sceneManager;
};
