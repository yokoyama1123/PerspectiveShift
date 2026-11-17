#include "pch.h"
#include "StageClearText.h"
#include"Scene/TextureUtil.h"

using namespace DirectX;

Yokoyama::StageClearText::StageClearText(GameContext& gameContext)
	:m_time{}
	,m_position{}
	,m_rect{}
{
	m_rect = Yokoyama::Getrect(gameContext.stageClearTexture.Get());

	// 画面中央に移動
	POSITION.x = Yokoyama::Screen::CENTER_X - m_rect.right / 2.0f * SCALE;
	POSITION.y = Yokoyama::Screen::CENTER_Y - m_rect.bottom / 2.0f * SCALE;
}

void Yokoyama::StageClearText::Update(float elapsedTime)
{
	// 時間を更新
	m_time += elapsedTime;
	if (m_time >= CYCLE)m_time = 0;

	// サイン波で上下に移動（振幅20ピクセル、周期2秒）
	float offsetY = AMPLITUDE * sinf(m_time * 2.0f * PI / CYCLE);

	// 移動を反映
	m_position.x = POSITION.x;
	m_position.y = POSITION.y + offsetY;
}

void Yokoyama::StageClearText::Render(GameContext& gameContext)
{
	//描画開始
	gameContext.spriteBatch.Begin(
		DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
		gameContext.commonStates.NonPremultiplied(),           // 半透明
		gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
	);

	// ステージクリアの描画
	gameContext.spriteBatch.Draw(
		gameContext.stageClearTexture.Get(),
		m_position,
		&m_rect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2{ 0.0f,0.0f },
		SCALE
	);

	// 描画終了
	gameContext.spriteBatch.End();
}