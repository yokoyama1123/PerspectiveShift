#pragma once
#include"pch.h"

// ステージ内のブロックの情報
namespace Yokoyama
{
    // ブロックの属性
    enum class CellType
    {
        Player, // プレイヤーの初期位置
        Goal,   // ゴールの位置
        Xp,     // 板ポリゴン(Xプラス方向)
        Xm,     // 板ポリゴン(Xマイナス方向)
        Yp,     // 板ポリゴン(Yプラス方向)
        Ym,     // 板ポリゴン(Yマイナス方向)
        Zp,     // 板ポリゴン(Zプラス方向)
        Zm,     // 板ポリゴン(Zマイナス方向)
        None,   // 何でもないよ
    };

    //回転軸
    enum class CubeEdgeAxis
    {
        //回転しない
        None,
        //X軸回転
        EdgeX_y0z0, EdgeX_y0z1, EdgeX_y1z0, EdgeX_y1z1,
        //Y軸回転
        EdgeY_x0z0, EdgeY_x0z1, EdgeY_x1z0, EdgeY_x1z1,
        //Z軸回転
        EdgeZ_x0y0, EdgeZ_x0y1, EdgeZ_x1y0, EdgeZ_x1y1,
    };
    
    // どこに何があるか(ステージ座標)
    struct CellData
    {
        // ステージの座標
        DirectX::SimpleMath::Vector3 stagePosition;

        // そこにあるもの
        Yokoyama::CellType type;

        //回転ギミック
        CubeEdgeAxis cubeEdgeAxis;

        // AABB（衝突判定用）
        DirectX::BoundingBox boundingBox;

        // マス目の大きさ(n * n)
        static constexpr float SIZE = 3.0f;

        // 中心から各軸までの距離(当たり判定)
        static constexpr DirectX::SimpleMath::Vector3 DISTANCE = {SIZE / 2, SIZE / 2, SIZE / 2};
    };
}
