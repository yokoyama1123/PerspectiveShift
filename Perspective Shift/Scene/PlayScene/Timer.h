#pragma once
#include "GameContext.h"

namespace Yokoyama
{
	// タイマー
	class Timer
	{
	public:
		// コンストラクタ
		Timer();

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

        //----------//

        //-----メンバー変数-----//
		// 現在の経過時間
		float m_time;
        //----------//

        //-----メンバー関数-----//
		
        //----------//
	};
}