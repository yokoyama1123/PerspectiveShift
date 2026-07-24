#include"pch.h"
#include "TitleStage.h"

using namespace DirectX;

Yokoyama::TitleStage::TitleStage()
	:m_rotate{0}
{
	//ビュー行列を設定する
	m_view = SimpleMath::Matrix::CreateLookAt(EYE, TARGET, SimpleMath::Vector3::Up);
}

void Yokoyama::TitleStage::Update(float elapsedTime)
{
	//回転する
	m_rotate += ROTATE_STAGE * elapsedTime;
	//一周したら角度をリセット
	if (m_rotate >= 360.0f) m_rotate = 0;

	//ワールド行列の後進
	m_world = SimpleMath::Matrix::CreateScale(SCALE) *
		SimpleMath::Matrix::CreateTranslation(STAGEPOSITION) *
		SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_rotate));
}

void Yokoyama::TitleStage::Rnder(GameContext& gameContext)
{
	//ステージのモデルの描画
	gameContext.stages[gameContext.selectStage]->Draw(
		gameContext.deviceResources.GetD3DDeviceContext(),
		gameContext.commonStates,
		m_world,
		m_view,
		gameContext.projection
	);
}
