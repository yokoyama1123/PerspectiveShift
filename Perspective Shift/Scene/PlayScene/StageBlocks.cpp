#include"pch.h"
#include "StageBlocks.h"

using namespace DirectX;

Yokoyama::StageBlocks::StageBlocks(ID3D11DeviceContext* pContext, const std::vector<Yokoyama::CellData>& stage)
{
	m_cube = DirectX::GeometricPrimitive::CreateCube(pContext, LENGTH);

	m_color = Colors::White;

	for (size_t i = 0; i < stage.size(); i++)
	{
		if (stage[i].type != Yokoyama::CellType::Goal && stage[i].type != Yokoyama::CellType::Player)
		{
			m_worlds.push_back(SimpleMath::Matrix::CreateTranslation(stage[i].boundingBox.Center));
		}
	}
}

void Yokoyama::StageBlocks::Update(float elapsedTime, DirectX::SimpleMath::Vector3 eyePos)
{
	//カメラから遠い順にソート
	std::sort(m_worlds.begin(), m_worlds.end(),
		[&](const SimpleMath::Matrix& a, const SimpleMath::Matrix& b) {
			return SimpleMath::Vector3::Distance(a.Translation(), eyePos)
				> SimpleMath::Vector3::Distance(b.Translation(), eyePos);
		}
	);

	m_color.w -= 0.1 * elapsedTime;
}

void Yokoyama::StageBlocks::Render(const SimpleMath::Matrix& view, const SimpleMath::Matrix& projection)
{
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
