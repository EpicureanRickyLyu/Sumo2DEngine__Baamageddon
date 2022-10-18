#pragma once
#include "../MainGame.h"

void UpdateExitDoughnut()
{
	if (!gameState.DoughnutsNum)
	{
		GameObject& obj_Doughnut = Play::GetGameObjectByType(TYPE_EXIT);
		GameObject& obj_Sheep = Play::GetGameObjectByType(TYPE_SHEEP);

		if (Play::IsColliding(obj_Doughnut, obj_Sheep))
		{
			Play::DrawFontText("64px", "You Win", { obj_Doughnut.pos.x, obj_Doughnut.pos.y - 200}, Play::CENTRE);
		}
		//Draw Sparkle
		float ran[5] = {0.15f,0.25f,0.35f,0.45f,0.55f};

		std::vector<Play::Colour> color{ {70,10,10},{10,80,10} ,{10,30,70} };
	
		for (float rad = ran[Play::RandomRollRange(0, 5)]; rad < 2.0f; rad += ran[Play::RandomRollRange(0, 5)])
		{
			int id = Play::CreateGameObject(TYPE_SPRINKLE, obj_Doughnut.pos, 0, SPRINKLE_SPRITE_NAME);
			Play::ColourSprite(SPRINKLE_SPRITE_NAME, color[Play::RandomRollRange(0, 3)]);
			GameObject& obj_sprinkle = Play::GetGameObject(id);
			obj_sprinkle.rotSpeed = 0.5f;
			obj_sprinkle.acceleration = { 0.0f, 0.2f };
			Play::SetGameObjectDirection(obj_sprinkle, 16, rad * PLAY_PI);
		}

		Play::UpdateGameObject(obj_Doughnut);
		Play::DrawObjectRotated(obj_Doughnut);
	}
}