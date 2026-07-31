#include "pch.h"
#include "Timer.h"
#include "imgui/imgui.h"

/// <summary>
/// コンストラクタ
/// </summary>
Yokoyama::Timer::Timer()
	:m_time{}
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">elapsedTime</param>
void Yokoyama::Timer::Update(float elapsedTime)
{
	//経過時間を加算
	m_time += elapsedTime;
}

/// <summary>
/// 描画
/// </summary>
/// <param name="gameContext">gameContext</param>
void Yokoyama::Timer::Render(GameContext& gameContext)
{
	int oneSecont{};
	oneSecont = static_cast<int>(m_time);
	int tenSecont{};
	if (oneSecont > 0)tenSecont = 10 % oneSecont;

	ImGui::DragInt("one", &oneSecont);
	ImGui::DragInt("ten", &tenSecont);
}

/// <summary>
/// 現在の経過時間を返す
/// </summary>
/// <returns>経過時間</returns>
float Yokoyama::Timer::GetTime()
{
	return m_time;
}
