#include "pch.h"
#include "TitleLogo.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="gameContext">ゲームコンテキスト</param>
Yokoyama::TitleLogo::TitleLogo(GameContext& gameContext)
	: m_time{}
	, m_position{}
	, m_startTextColor{1,1,1,1}
	, m_titleRect{}
	, m_startTextRect{}
{
	// タイトルロゴのRectを設定
	m_titleRect = Getrect(gameContext.titlelogo.Get());
	// スタートテキストのRectを設定
	m_startTextRect = Getrect(gameContext.startTextTexture.Get());

	// 画面中央にロゴを移動
	LOGOPOSITION.x = Yokoyama::Screen::CENTER_X - m_titleRect.right / 2.0f * SCALELOGO;
	LOGOPOSITION.y = Yokoyama::Screen::CENTER_Y - m_titleRect.bottom / 2.0f * SCALELOGO;

	// スタートテキストを移動
	STARTTEXTPOSITION.x = Yokoyama::Screen::CENTER_X - m_startTextRect.right / 2.0f * SCALESTARTTEXT;
	STARTTEXTPOSITION.y = Yokoyama::Screen::CENTER_X - m_startTextRect.bottom / 2.0f * SCALESTARTTEXT;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">elapsedTime</param>
void Yokoyama::TitleLogo::Update(float elapsedTime)
{
	// 時間を更新
	m_time += elapsedTime;

	if (m_time >= CYCLE)m_time = 0;

	// サイン波で上下に移動（振幅20ピクセル、周期2秒）
	float offsetY = AMPLITUDE * sinf(m_time * 2.0f * PI / CYCLE);

	// 移動を反映
	m_position.x = LOGOPOSITION.x;
	m_position.y = LOGOPOSITION.y + offsetY;

	// スタートテキストのα値を変更
	m_startTextColor.w = sinf(m_time * 2.0f * PI / CYCLE);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="gameContext">ゲームコンテキスト</param>
void Yokoyama::TitleLogo::Render(GameContext& gameContext)
{
	//描画開始
	gameContext.spriteBatch.Begin(
		DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
		gameContext.commonStates.NonPremultiplied(),           // 半透明
		gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
	);

	// タイトルロゴの描画
	gameContext.spriteBatch.Draw(
		gameContext.titlelogo.Get(),
		m_position,
		&m_titleRect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2{ 0.0f,0.0f },
		SCALELOGO
	);

	// スタートテキストの描画
	gameContext.spriteBatch.Draw(
		gameContext.startTextTexture.Get(),
		STARTTEXTPOSITION,
		&m_startTextRect,
		m_startTextColor,
		0.0f,
		SimpleMath::Vector2{ 0.0f,0.0f },
		SCALESTARTTEXT
	);

	// 描画終了
	gameContext.spriteBatch.End();
}

/// <summary>
/// その画像のアスペクト比を返す(RECT)
/// </summary>
/// <param name="srv">シェーダーリソースビュー</param>
/// <returns>アスペクト比(RECT)</returns>
RECT Yokoyama::TitleLogo::Getrect(ID3D11ShaderResourceView* srv)
{
	// シェーダーリソースビューから元になるリソースを取得
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	srv->GetResource(resource.GetAddressOf());

	// リソースを2Dテクスチャとして扱う
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = resource.As(&texture);

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
