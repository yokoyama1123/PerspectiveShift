#include"pch.h"
#include "Save&Load.h"
#include"MagicEnum/magic_enum.hpp"
#include<fstream>
#include<filesystem>

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Yokoyama::SaveLoad::SaveLoad()
	:m_stages{}
{
    LoadData();
}

/// <summary>
/// ステージデータを保存する
/// </summary>
/// <param name="stageNumber">ステージナンバー</param>
/// <param name="cellDatas">ステージデータ</param>
void Yokoyama::SaveLoad::SaveData(int stageNumber, const std::vector<Yokoyama::CellData>& cellDatas)
{
    // jsonを作成
    json idata =
    {
        {"bestTime",0},
        {
            "blocks",
            {
            }
        },
    };

    // セルのタイプと座標を保存
    for (size_t i = 0; i < cellDatas.size(); i++)
    {
        //タイプをstring型に修正して取得
        auto type = magic_enum::enum_name<Yokoyama::CellType>(cellDatas[i].type);
        //セルを保存
        idata["blocks"].push_back({ { "type",type },{"Rotate",cellDatas[i].isRotate},{"X",cellDatas[i].stagePosition.x},{"Y",cellDatas[i].stagePosition.y},{"Z",cellDatas[i].stagePosition.z}});
    }

    // ファイルに保存
    std::string fileAddress = "Resources/StageData/Stage" + std::to_string(stageNumber) + ".json";
    std::ofstream output_file(fileAddress);
    if (output_file.is_open())
    {
        output_file << idata.dump(4);  // 4スペースでインデント
        output_file.close();
    }

    //ゲーム内のセルデータも更新
    m_stages[stageNumber] = idata;
}

/// <summary>
/// ベストタイムを保存する
/// </summary>
/// <param name="stageNumber">ステージナンバー<</param>
/// <param name="time">クリアタイム</param>
void Yokoyama::SaveLoad::SaveBestTime(int stageNumber, float time)
{
    //タイムを記録
    if ( m_stages[stageNumber]["bestTime"] == 0 || m_stages[stageNumber]["bestTime"] >= time)
    {
        m_stages[stageNumber]["bestTime"] = time;
    }

    // ファイルに保存
    std::string fileAddress = "Resources/StageData/Stage" + std::to_string(stageNumber) + ".json";
    std::ofstream output_file(fileAddress);
    if (output_file.is_open())
    {
        output_file << m_stages[stageNumber].dump(4);  // 4スペースでインデント
        output_file.close();
    }
}

/// <summary>
/// ステージデータを返す
/// </summary>
/// <param name="stageNumber">ステージナンバー</param>
/// <returns>ステージデータ(json)</returns>
json* Yokoyama::SaveLoad::GetStagesJson(int stageNumber)
{
    return &m_stages[stageNumber];
}

/// <summary>
/// josnのステージデータを渡すとCellDatasを返す
/// </summary>
/// <param name="stage">ステージデータ(json)</param>
/// <returns>ステージデータ</returns>
std::vector<Yokoyama::CellData> Yokoyama::SaveLoad::GetCellDatas(json stage)
{
    std::vector<Yokoyama::CellData> celldatas{};
    bool isRotate{ false };

    for (const auto& data : stage["blocks"])
    {
        //位置を取得
        SimpleMath::Vector3 position = { data["X"], data["Y"], data["Z"] };

        //タイプを取得
        Yokoyama::CellType type;//タイプを入れる変数
        //タイプをstring型で取得
        std::string a = data["type"];
        //string型からCellTypeに変換して保存
        type = magic_enum::enum_cast<Yokoyama::CellType>(a).value_or(Yokoyama::CellType::Xm);
        
        //回転可かを取得
        isRotate = data["Rotate"];

        //上記を保存
        celldatas.push_back(CellData{ position, type, isRotate, BoundingBox{{},Yokoyama::CellData::DISTANCE} });
    }
   
	return celldatas;
}

/// <summary>
/// josnのステージデータを渡すとBestTimeを返す
/// </summary>
/// <param name="stage">ステージデータ(json)</param>
/// <returns>ベストタイム</returns>
float Yokoyama::SaveLoad::GetBestTime(json stage)
{
    return stage["bestTime"];
}

/// <summary>
/// 全てのステージデータを読み込む
/// </summary>
void Yokoyama::SaveLoad::LoadData()
{
    // ステージデータがある場所
    std::string path = "Resources/StageData/";
    
    // ステージデータの数
    size_t count{};

    // フォルダが見つかるたびにカウント
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        ++count;
    }

    // ステージデータのファイル名
    std::string filename{};

    for (size_t i = 0; i < count; i++)
    {
        filename = path + "Stage" + std::to_string(i) + ".json";

        // ファイルから読み込み
        std::ifstream satage(filename);
        if (satage.is_open())
        {
            // ステージデータの読み込み
            m_stages.push_back(json::parse(satage));
            satage.close();
        }
    }
}