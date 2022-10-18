#pragma once
#include "../MainGame.h"

//lack animation and singleton 
void UpdateExitDoughnut()
{
	if (!gameState.DoughnutsNum)
	{
		GameObject& obj_Doughnut = Play::GetGameObjectByType(TYPE_EXIT);
		GameObject& obj_Sheep = Play::GetGameObjectByType(TYPE_SHEEP);

		if (Play::IsColliding(obj_Doughnut, obj_Sheep))
		{
			Play::DrawFontText("64px", "SCORE: You Win", {DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2}, Play::CENTRE);
		}

		Play::UpdateGameObject(obj_Doughnut);
		Play::DrawObjectRotated(obj_Doughnut);
	}
}