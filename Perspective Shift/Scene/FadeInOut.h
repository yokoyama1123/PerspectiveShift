#pragma once
#include "GameContext.h"

namespace Yokoyama
{
	// フェードイン・アウトを行うクラス
	class FadeInOut
	{
    public:
		// コンストラクタ
        FadeInOut();
		// デストラクタ
        ~FadeInOut() = default;

		// フェードイン(Update)
		void FedeInUpdate(float elapsedTime, float fadeInSecond = 0.0f, DirectX::SoundEffectInstance* bgm = nullptr);
		// フェードアウト(Update)
        void FedeOutUpdate(float elapsedTime, float fadeOutSecond = 0.0f, DirectX::SoundEffectInstance* bgm = nullptr);
		// フェードイン(Render)
        void FedeInRender(GameContext& gameContext);
		// フェードアウト(Render)
        void FedeOutRnder(GameContext& gameContext);
        // シーン切り替え時に呼び出される関数
        void OnEnter();

		// フェードインし終わったか
        bool GetFedeInEnd();
		// フェードアウトし終わったか
        bool GetFedeOutEnd();

	private:
		//----- メンバー変数 -----//
		// フェードインし終わった
        bool m_fedeInEnd;
		// フェードアウトし終わった
        bool m_fedeOutEnd;
		// フェードイン中のカウント
		float m_fadeInCount;
		// フェードアウト中のカウント
		float m_fadeOutCount;
		// フェードインの進行割合(0~1)
		float m_fadeInPercent;
		// フェードアウトの進行割合(0~1)
		float m_fadeOutPercent;
		//----------//
	};
}