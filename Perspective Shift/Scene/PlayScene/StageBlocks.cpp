#include"pch.h"
#include "StageBlocks.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pContext">コンテキスト</param>
/// <param name="stage">ステージデータ</param>
Yokoyama::StageBlocks::StageBlocks(ID3D11DeviceContext* pContext, const std::vector<Yokoyama::CellData>& stage)
{
	//立方体の作成
	m_cube = DirectX::GeometricPrimitive::CreateCube(pContext, LENGTH);

	//色の初期化
	m_color = Colors::White;

	//ブロックのある場所のワールド行列を作成
	for (size_t i = 0; i < stage.size(); i++)
	{
		if (stage[i].type != Yokoyama::CellType::Goal && stage[i].type != Yokoyama::CellType::Player)
		{
			m_worlds.push_back(SimpleMath::Matrix::CreateTranslation(stage[i].boundingBox.Center));
		}
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">elapsedTime</param>
/// <param name="eyePos">カメラの場所</param>
void Yokoyama::StageBlocks::Update(float elapsedTime, DirectX::SimpleMath::Vector3 eyePos)
{
	//カメラから遠い順にソート
	std::sort(m_worlds.begin(), m_worlds.end(),
		[&](const SimpleMath::Matrix& a, const SimpleMath::Matrix& b) {
			return SimpleMath::Vector3::Distance(a.Translation(), eyePos)
				> SimpleMath::Vector3::Distance(b.Translation(), eyePos);
		}
	);

	//α値を下げる
	m_color.w -= ALPHA * elapsedTime;
}

/// <summary>
/// 描画
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Yokoyama::StageBlocks::Render(const SimpleMath::Matrix& view, const SimpleMath::Matrix& projection)
{
	//ワールド行列の数だけ描画
	for (size_t i = 0; i < m_worlds.size(); i++)
	{
		m_cube->Draw(
			m_worlds[i],
			view,
			projection,
			m_color,
			nullptr,
			false
		);
	}
}
