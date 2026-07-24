#pragma once
#include"GameContext.h"

namespace Yokoyama
{
	class TitleLogo
	{
	public:
		//コンストラクタ
		TitleLogo(GameContext& gameContext);
		//デストラクタ
		~TitleLogo() = default;
	
		//更新
		void Update(float elapsedTime);
		//描画
		void Render(GameContext& gameContext);
	
	private:
		//タイトルロゴの大きさ(倍率)
		static constexpr float SCALELOGO = 0.8f;

		// 振幅（移動幅）
		static constexpr float AMPLITUDE = 20.0f;   

		//周期(秒数)
		static constexpr float CYCLE = 4.0f;

		//円周率
		static constexpr float PI = 3.14159265358979f;

		//タイトルロゴの初期位置
		DirectX::SimpleMath::Vector2 LOGOPOSITION;

		
		// 経過時間
		float m_time = 0.0f;         

		//ロゴの場所
		DirectX::SimpleMath::Vector2 m_position;
	
		//タイトルロゴのRECT
		RECT m_titleRect;
	
		//その画像のアスペクト比を返す(RECT)
		RECT Getrect(ID3D11ShaderResourceView* srv);
	};
}

