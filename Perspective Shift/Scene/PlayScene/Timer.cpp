#include "pch.h"
#include "Timer.h"
#include <cmath>

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="gameContext">gameContext</param>
Yokoyama::Timer::Timer(GameContext& gameContext)
	: m_time{}
{
	//数字全てを描いた画像のRECT
	RECT numbersRECT = Getrect(gameContext.numbersTexture.Get());
	//一つの数字のRECTの横幅を記録
	float singleNumberSize = numbersRECT.right / 10.0f;
	//一つの数字のRECT(数字全てを描いた画像のRECTで初期化)
	RECT singleNumberRECT{numbersRECT};

	//数字一つずつのRECTを設定
	for (size_t i = 0; i < 10; i++)
	{
		//一つの数字のRECTを修正
		singleNumberRECT.left = numbersRECT.left + singleNumberSize * i;
		singleNumberRECT.right = singleNumberRECT.left + singleNumberSize;
		//RECTの登録
		m_numberRECTs.push_back(singleNumberRECT);
	}

	//タイムを記録する桁数を設定
	m_timeDigits.resize(4);

}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">elapsedTime</param>
void Yokoyama::Timer::Update(float elapsedTime)
{
	// 経過時間を加算
	m_time += elapsedTime;

	// ミリ秒単位に変換
	int totalMilliseconds = static_cast<int>(m_time * 1000.0f);

	// 分・秒に分解
	int totalSeconds = totalMilliseconds / 1000;
	int seconds = totalSeconds % 60;
	int minutes = (totalSeconds / 60) % 60;

	// 桁数を確認して代入
	int digitIndex = 0;

	// 分の桁を記録
	if (digitIndex < m_timeDigits.size()) m_timeDigits[digitIndex++] = (minutes / 10) % 10;  // 分10の位
	if (digitIndex < m_timeDigits.size()) m_timeDigits[digitIndex++] = minutes % 10;         // 分1の位

	// 秒の桁を記録
	if (digitIndex < m_timeDigits.size()) m_timeDigits[digitIndex++] = (seconds / 10) % 10;  // 秒10の位
	if (digitIndex < m_timeDigits.size()) m_timeDigits[digitIndex++] = seconds % 10;         // 秒1の位

}

/// <summary>
/// 描画
/// </summary>
/// <param name="gameContext">gameContext</param>
void Yokoyama::Timer::Render(GameContext& gameContext)
{
	//描画開始
	gameContext.spriteBatch.Begin(
		DirectX::DX11::SpriteSortMode::SpriteSortMode_Deferred,// 通常のソートモード
		gameContext.commonStates.NonPremultiplied(),           // 半透明
		gameContext.commonStates.PointWrap()                   // テクスチャサンプラー
	);

	// タイムの描画
	SimpleMath::Vector2 numberPosition{};
	for (size_t i = 0; i < m_timeDigits.size(); i++)
	{
		numberPosition.x = POSITION.x + (i * m_numberRECTs[0].right * SCALE * 1.1);

		gameContext.spriteBatch.Draw(
			gameContext.numbersTexture.Get(),
			numberPosition,
			&m_numberRECTs[m_timeDigits[i]],
			Colors::White,
			0.0f,
			SimpleMath::Vector2{ 0.0f,0.0f },
			SCALE
		);
		
	}

	// 描画終了
	gameContext.spriteBatch.End();
}

/// <summary>
/// 現在の経過時間を返す
/// </summary>
/// <returns>経過時間</returns>
float Yokoyama::Timer::GetTime()
{
	return m_time;
}

/// <summary>
/// その画像のアスペクト比を返す(RECT)
/// </summary>
/// <param name="srv">シェーダーリソースビュー</param>
/// <returns>アスペクト比(RECT)</returns>
RECT Yokoyama::Timer::Getrect(ID3D11ShaderResourceView* srv)
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
