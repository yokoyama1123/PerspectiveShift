#pragma once
#include "GameContext.h"

namespace Yokoyama
{
	// カウントアップタイマー
	class Timer
	{
	public:
		// コンストラクタ
		Timer(GameContext& gameContext, int numberDigits = 4);

		// デストラクタ
		~Timer() = default;

		// 更新
		void Update(float elapsedTime);

		// 描画
		void Render(GameContext& gameContext);

		// 現在の経過時間
		float GetTime();

	private:
        //-----定数集-----//
		//数字一つの大きさ(n倍)
		static constexpr float SCALE = 2.0f;
		//タイマー左上のスクリーン座標
		static constexpr DirectX::SimpleMath::Vector2 POSITION{0, 0};
        //----------//

        //-----メンバー変数-----//
		// 現在の経過時間
		float m_time;
		//時間それぞれの桁の数字
		std::vector<int> m_timeDigits;
		// 数字それぞれのRECT
		std::vector<RECT> m_numberRECTs;
        //----------//

        //-----メンバー関数-----//
		// その画像のアスペクト比を返す(RECT)
		RECT Getrect(ID3D11ShaderResourceView* srv);
        //----------//
	};
}