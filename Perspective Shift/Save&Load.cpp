#include"pch.h"
#include "Save&Load.h"
#include<fstream>
#include<filesystem>

using namespace DirectX;

Yokoyama::SaveLoad::SaveLoad()
	:m_stages{}
{
    LoadData();
}


void Yokoyama::SaveLoad::SaveData(const std::string& filename, const std::vector<Yokoyama::CellData>& cellDatas)
{
    json idata =
    {
        {"bestTime",0},
        {
            "blocks",
            {
            }
        },
    };

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
    std::string fileAddress = "Resources/StageData/" + filename;

    std::ofstream output_file(fileAddress);
    if (output_file.is_open())
    {
        output_file << idata.dump(4);  // 4スペースでインデント
        output_file.close();
    }
}

json* Yokoyama::SaveLoad::GetStagesJson(int stageNumber)
{
    return &m_stages[stageNumber];
}

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

float Yokoyama::SaveLoad::GetBestTime(json stage)
{
    return stage["bestTime"];
}

void Yokoyama::SaveLoad::LoadData()
{
    std::string path = "Resources/StageData/";
    
    size_t count{};

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        //フォルダが見つかるたびにカウント
        ++count;
    }

    std::string filename{};

    for (size_t i = 0; i < count; i++)
    {
        filename = path + "Stage" + std::to_string(i) + ".json";

        //ファイルから読み込み
        std::ifstream satage0(filename);
        if (satage0.is_open())
        {
            m_stages.push_back(json::parse(satage0));
            satage0.close();
        }
    }
}