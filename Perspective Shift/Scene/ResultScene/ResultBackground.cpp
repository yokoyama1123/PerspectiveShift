#include "pch.h"
#include "ResultBackground.h"

using namespace DirectX;

Yokoyama::ResultBackground::ResultBackground(GameContext& gameContext)
	:m_rect{}
{
	m_rect = Getrect(gameContext.resultBackgroundTexture.Get());
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

RECT Yokoyama::ResultBackground::Getrect(ID3D11ShaderResourceView* srv)
{
	// シェーダーリソースビューから元になるリソースを取得
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	srv->GetResource(resource.GetAddressOf());

	// リソースを2Dテクスチャとして扱う
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resource.As(&texture);

	// テクスチャの幅や高さなどを取得
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	// 取得した幅と高さをlongに変換
	long Width = static_cast<long>(desc.Width);
	long Height = static_cast<long>(desc.Height);

	// RECT を作成して返す
	RECT r{ 0, 0, Width, Height };
	return r;
}
