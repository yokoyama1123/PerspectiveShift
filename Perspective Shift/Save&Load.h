#pragma once
#include<string>
#include<vector>
#include"Scene/PlayScene/CellData.h"
#include<nlohmann/json.hpp>

using json = nlohmann::json;

namespace Yokoyama
{
	// ステージのセーブロードをするクラス
	class SaveLoad
	{
	public:
		// コンストラクタ
		SaveLoad();
		// デストラクタ
		~SaveLoad() = default;
		// ステージデータを保存する
		void SaveData(int stageNumber, const std::vector<Yokoyama::CellData>& cellDatas);

		// ベストタイムを保存する
		void SaveBestTime(int stageNumber, float time);

		// ステージデータを返す
		json* GetStagesJson(int stageNumber);

		// josnのステージデータを渡すとCellDatasを返す
		std::vector<Yokoyama::CellData> GetCellDatas(json stage);

		// josnのステージデータを渡すとBestTimeを返す
		float GetBestTime(json stage);
	private:
		// ステージデータ
		std::vector<json> m_stages;
		// 全てのステージデータを読み込む
		void LoadData();
	};
}