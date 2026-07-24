//--------------------------------------------------------------------------------------
// File: GameContext.h
//
// シーンへ渡すゲームコンテキストクラス
//
// Date: 2026.3.3
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Common/StepTimer.h"
#include "Common/DeviceResources.h"
#include "ImaseLib/DebugRenderer.h"
#include "Save&Load.h"

// 各シーンに渡す共通リソースを記述してください
struct GameContext
{
	// ステップタイマー
	DX::StepTimer& timer;

	// デバイスリソース
	DX::DeviceResources& deviceResources;

	// キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker& keyboardTracker;

	// コモンステート
	DirectX::CommonStates& commonStates;

	// デバッグ用の描画セット
	Imase::DebugRenderer& debugRenderer;

    //プロゼクション行列
    DirectX::SimpleMath::Matrix& projection;

    //スプライトバッチ
    DirectX::SpriteBatch& spriteBatch;

    //現在選択しているステージ
    int selectStage;

	//----- モデル -----//
	//プレイヤーのモデル
    DirectX::Model* playerModel;
    //ステージ外枠のモデル
    std::vector<std::unique_ptr<DirectX::Model>>& stages;
    //ゴールのモデル
    DirectX::Model* goalModel;
    // ---------- //

    //-----テクスチャ-----//
    //板ポリゴンのテクスチャ
    const wchar_t* textureHandle;
    //タイトルロゴ
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& titlelogo;
    //フェードインアウト
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& fadeInOutTexture;
    //----------//

    //-----サウンド-----//
    //タイトルシーンのBGM
    DirectX::SoundEffect* titleBGM;
    //プレイシーンのBGM
    DirectX::SoundEffect* playBGM;
    //----------//

    //-----セーブロードクラス-----//
    std::unique_ptr<Yokoyama::SaveLoad> saveLoad;
    //----------//
};

