#pragma once
#include<string>
#include<vector>
#include"Scene/PlayScene/CellData.h"
#include<nlohmann/json.hpp>

using json = nlohmann::json;

namespace Yokoyama
{
	//ステージのセーブロードをするクラス
	class SaveLoad
	{
	public:
		//コンストラクタ
		SaveLoad();
		//デストラクタ
		~SaveLoad() = default;
		//ステージデータをセーブする
		void SaveData(const std::string& filename, const std::vector<Yokoyama::CellData>& cellDatas);



		//ステージデータを返す
		json* GetStagesJson(int stageNumber);

		//josnのステージデータを渡すとCellDatasを返す
		std::vector<Yokoyama::CellData> GetCellDatas(json stage);
		//josnのステージデータを渡すとBestTimeを返す
		float GetBestTime(json stage);
	private:
		//ステージデータ
		std::vector<json> m_stages;
		//ロードする
		void LoadData();
	};
}