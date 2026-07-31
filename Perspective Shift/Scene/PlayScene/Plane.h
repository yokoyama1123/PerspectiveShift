#pragma once
#include"CellData.h"
#include<vector>

namespace Yokoyama
{
    // 板ポリゴンクラス
	class Plane
	{
    public:
        // コンストラクタ
        Plane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, DirectX::CommonStates* pStates, const wchar_t* textureHandle);

        // デストラクタ
        ~Plane() = default;

        // 板ポリゴンの描画
        void Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, const std::vector<Yokoyama::CellData>& cellDatas);

    private:
        //-----メンバー変数-----//
        // コモンステート
        DirectX::CommonStates* m_pStates;
        // ベーシックエフェクト
        std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
        // プリミティブバッチ
        std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
        // テクスチャー
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
        // レイアウト
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
        //----------//
	};

}