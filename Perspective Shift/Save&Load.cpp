#include"pch.h"
#include "Save&Load.h"
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
        switch (cellDatas[i].type)
        {
        case Yokoyama::CellType::Player:
            idata["blocks"].push_back({ {"type","Player"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Goal:
            idata["blocks"].push_back({ {"type","Goal"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Xm:
            idata["blocks"].push_back({ {"type","Xm"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Xp:
            idata["blocks"].push_back({ {"type","Xp"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Ym:
            idata["blocks"].push_back({ {"type","Ym"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Yp:
            idata["blocks"].push_back({ {"type","Yp"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Zm:
            idata["blocks"].push_back({ {"type","Zm"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        case Yokoyama::CellType::Zp:
            idata["blocks"].push_back({ {"type","Zp"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        default:
            idata["blocks"].push_back({ {"type","None"},{"X",cellDatas[i].stagePosition.x}, {"Y",cellDatas[i].stagePosition.y}, {"Z",cellDatas[i].stagePosition.z} });
            break;
        }
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

    for (const auto& data : stage["blocks"])
    {
        SimpleMath::Vector3 position = { data["X"], data["Y"], data["Z"] };
        Yokoyama::CellType type{};

        if (data["type"] == "Xm")           type = Yokoyama::CellType::Xm;
        else if (data["type"] == "Xp")      type = Yokoyama::CellType::Xp;
        else if (data["type"] == "Ym")      type = Yokoyama::CellType::Ym;
        else if (data["type"] == "Yp")      type = Yokoyama::CellType::Yp;
        else if (data["type"] == "Zm")      type = Yokoyama::CellType::Zm;
        else if (data["type"] == "Zp")      type = Yokoyama::CellType::Zp;
        else if (data["type"] == "Player")  type = Yokoyama::CellType::Player;
        else if (data["type"] == "Goal")    type = Yokoyama::CellType::Goal;

        celldatas.push_back(CellData{ position, type, BoundingBox{{},Yokoyama::CellData::DISTANCE} });
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