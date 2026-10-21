#pragma once
#include "GeometricPrimitive.h"
#include "GameContext.h"
namespace Yokoyama
{
	//プレイシーン開始時に出現しているブロッククラス
	class StageBlocks
	{
	public:
		//コンストラクタ
		StageBlocks(ID3D11DeviceContext* pContext, const std::vector<Yokoyama::CellData>& stage);
		//デストラクタ
		~StageBlocks() = default;
		//更新
		void Update(float elapsedTime, DirectX::SimpleMath::Vector3 eyePos);
		//描画
		void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

	private:
        //----- 定数 -----//
		//ブロックの一辺の長さ
		static constexpr float LENGTH = 2.9f;
		//1秒に減るα値
		static constexpr float ALPHA = 0.5;
        //----------//

        //----- メンバー変数 -----//
		//ブロック
		std::unique_ptr<DirectX::GeometricPrimitive> m_cube;
		//ブロックの色
		DirectX::SimpleMath::Vector4 m_color;
		//ブロックの位置
		std::vector<DirectX::SimpleMath::Matrix>m_worlds;
        //----------//

        //----- メンバー関数 -----//

        //----------//
	};
}