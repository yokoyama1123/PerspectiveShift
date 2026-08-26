#pragma once
#include"GameContext.h"

//ステージクリアテクスチャを描画するクラス
namespace Yokoyama
{
	class StageClearText
	{
	public:
		//コンストラクタ
		StageClearText(GameContext& gameContext);
		//デストラクタ
		~StageClearText() = default;

		//更新
		void Update(float elapsedTime);
		//描画
		void Render(GameContext& gameContext);

	private:
		//----- 定数 -----//
		// ステージクリアの大きさ(倍率)
		static constexpr float SCALE = 0.8f;
		// 振幅（移動幅）
		static constexpr float AMPLITUDE = 20.0f;
		// 周期(秒数)
		static constexpr float CYCLE = 4.0f;
		// 円周率
		static constexpr float PI = 3.14159265358979f;
		// ステージクリアの初期位置
		DirectX::SimpleMath::Vector2 POSITION{};
		//----------//

		//----- メンバー変数 -----//
		// 経過時間
		float m_time = 0.0f;
		// ステージクリアの場所
		DirectX::SimpleMath::Vector2 m_position;
		// ステージクリアのRECT
		RECT m_rect;
		//----------//

		//----- メンバー関数 -----//
		// その画像のアスペクト比を返す(RECT)
		RECT Getrect(ID3D11ShaderResourceView* srv);
		//----------//
	};
}