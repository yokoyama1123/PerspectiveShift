#pragma once
#include"pch.h"

//ステージ内のブロックの情報
namespace Yokoyama
{
    // それが何か
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
    
    // どこに何があるか(ステージ座標)
    struct CellData
    {
        // ステージの座標
        DirectX::SimpleMath::Vector3 stagePosition;
        // そこにあるもの
        Yokoyama::CellType type;

        // AABB（衝突判定用）
        DirectX::BoundingBox boundingBox;

        // マス目の大きさ(n * n)
        static constexpr float SIZE = 3.0f;

        // 中心から各軸までの距離(当たり判定)
        static constexpr DirectX::SimpleMath::Vector3 DISTANCE = {SIZE / 2, SIZE / 2, SIZE / 2};
    };
}
