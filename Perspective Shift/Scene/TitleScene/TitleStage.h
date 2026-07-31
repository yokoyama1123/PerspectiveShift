#pragma once
#include "GameContext.h"

// タイトルシーンの背景で描画されているステージ
namespace Yokoyama
{
	class TitleStage
	{
	public:
		// コンストラクタ
		TitleStage();
		
		// デストラクタ
		~TitleStage() = default;

		// 更新
		void Update(float elapsedTime);

		// 描画
		void Rnder(GameContext& gameContext);

	private:
		//----- 定数 -----//
		// ステージのサイズ
		static constexpr float SCALE = 10.0f;
		// ステージの回転角度(度/s)
		static constexpr float ROTATE_STAGE = 20.0f;
		// ステージの位置
		static constexpr DirectX::SimpleMath::Vector3 STAGEPOSITION = { -15,0,15 };
		// カメラの位置
		static constexpr DirectX::SimpleMath::Vector3 EYE = { 0,18,15 };
		// ターゲットの位置
		static constexpr DirectX::SimpleMath::Vector3 TARGET = { EYE.x, EYE.y - 0.3f, EYE.z - 1.0f };
		//----------//

		//----- メンバー変数 -----//
		// 現在の回転角度
		float m_rotate;
		// ワールド行列
		DirectX::SimpleMath::Matrix m_world;
		// ビュー行列
		DirectX::SimpleMath::Matrix m_view;
		//----------//
	};
}