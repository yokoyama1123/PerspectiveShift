#include "pch.h"
#include "Plane.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pDevice">デバイス</param>
/// <param name="pContext">デバイスコンテキスト</param>
/// <param name="pStates">コモンステート</param>
/// <param name="textureHandle">板ポリゴンのテクスチャ</param>
Yokoyama::Plane::Plane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CommonStates* pStates, const wchar_t* textureHandle)
    : m_pStates(pStates)
{
    // プリミティブバッチの作成
    m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pContext);

    // テクスチャ
    DX::ThrowIfFailed(CreateDDSTextureFromFile(
        pDevice,
        textureHandle,
        nullptr,
        m_texture.ReleaseAndGetAddressOf())
    );

    // ベーシックエフェクト
    m_basicEffect = std::make_unique<BasicEffect>(pDevice);
    m_basicEffect->SetVertexColorEnabled(false);
    m_basicEffect->SetLightingEnabled(false);
    m_basicEffect->SetTextureEnabled(true);
    m_basicEffect->SetTexture(m_texture.Get());

    // 入力レイアウト
    DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionTexture>(pDevice, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf()));
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="pContext">デバイスコンテキスト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロゼクション行列</param>
/// <param name="cellDatas">ステージデータ</param>
void Yokoyama::Plane::Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, const std::vector<Yokoyama::CellData>& cellDatas)
{
    // ステート設定
    pContext->OMSetBlendState(m_pStates->Opaque(), nullptr, 0xFFFFFFFF);
    pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);
    pContext->RSSetState(m_pStates->CullCounterClockwise());

    // ワールド行列（固定）
    m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
    m_basicEffect->SetView(view);
    m_basicEffect->SetProjection(proj);
    m_basicEffect->Apply(pContext);

    // サンプラー
    auto sampler = m_pStates->PointWrap();
    pContext->PSSetSamplers(0, 1, &sampler);

    // 入力レイアウト
    pContext->IASetInputLayout(m_inputLayout.Get());

    // 板ポリゴン
    VertexPositionTexture v[4]{};

    //ステージ座標からワールド座標へ変更するときの加算量
    SimpleMath::Vector3 addPosition{};

    //描画開始
    m_primitiveBatch->Begin();

    //ステージデータの数だけ回す
    for (size_t i = 0; i < cellDatas.size(); i++)
    {
        //ステージ座標を取得
        addPosition = cellDatas[i].stagePosition;
        
        //ステージ座標をワールド座標変更するために乗算
        addPosition.x *=  CellData::SIZE;
        addPosition.y *=  CellData::SIZE;
        addPosition.z *= -CellData::SIZE;

        //板ポリゴンを描画する方向による板ポリゴンの設定
        switch (cellDatas[i].type)
        {
            case Yokoyama::CellType::Xp:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            case Yokoyama::CellType::Xm:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(0.0f, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(0.0f, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            case Yokoyama::CellType::Yp:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            case Yokoyama::CellType::Ym:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(0.0f, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(0.0f, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            case Yokoyama::CellType::Zp:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(0.0f, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -0.0f) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            case Yokoyama::CellType::Zm:
                // 板ポリゴンの設定
                v[0] = {SimpleMath::Vector3(CellData::SIZE, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 1.0f)};
                v[1] = {SimpleMath::Vector3(CellData::SIZE, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(0.0f, 0.0f)};
                v[2] = {SimpleMath::Vector3(0.0f, 0.0f, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 1.0f)};
                v[3] = {SimpleMath::Vector3(0.0f, CellData::SIZE, -CellData::SIZE) + addPosition, SimpleMath::Vector2(1.0f, 0.0f)};
                break;
            default:
                break;
        }

        //板ポリゴンの描画
        m_primitiveBatch->Draw(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, v, 4);
     
    }
    //描画終了
    m_primitiveBatch->End();
}