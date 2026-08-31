#pragma once
#include"GameContext.h"

//リザルトシーンの背景を描画するクラス
namespace Yokoyama
{
	class ResultBackground
	{
	public:
		//コンストラクタ
		ResultBackground(GameContext& gameContext);
		//デストラクタ
		~ResultBackground() = default;
		//描画
		void Render(GameContext& gameContext);

	private:
		//----- 定数 -----//
		// ステージクリアの大きさ(倍率)
		static constexpr float SCALE = 0.8f;
		// ステージクリアの初期位置
		static constexpr DirectX::SimpleMath::Vector2 POSITION{0, 0};
		//----------//

		//----- メンバー変数 -----//
		// ステージクリアのRECT
		RECT m_rect;
		//----------//

		//----- メンバー関数 -----//
		// その画像のアスペクト比を返す(RECT)
		RECT Getrect(ID3D11ShaderResourceView* srv);
		//----------//
	};
}