#pragma once
#include"pch.h"

namespace Yokoyama
{
	/// <summary>
	/// その画像のアスペクト比を返す(RECT)
	/// </summary>
	/// <param name="srv">シェーダーリソースビュー</param>
	/// <returns>アスペクト比(RECT)</returns>
	RECT Getrect(ID3D11ShaderResourceView* srv)
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
}