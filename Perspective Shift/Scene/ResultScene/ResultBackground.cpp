#include "pch.h"
#include "ResultBackground.h"
#include"Scene/TextureUtil.h"

using namespace DirectX;

Yokoyama::ResultBackground::ResultBackground(GameContext& gameContext)
	:m_rect{}
{
	m_rect = Yokoyama::Getrect(gameContext.resultBackgroundTexture.Get());
}

void Yokoyama::ResultBackground::Render(GameContext& gameContext)
{
	//描画開始
	gameContext.spriteBatch.Begin(
		DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
		gameContext.commonStates.NonPremultiplied(),           // 半透明
		gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
	);

	// ステージクリアの描画
	gameContext.spriteBatch.Draw(
		gameContext.resultBackgroundTexture.Get(),
		POSITION,
		&m_rect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2{ 0.0f,0.0f },
		SCALE
	);

	// 描画終了
	gameContext.spriteBatch.End();
}