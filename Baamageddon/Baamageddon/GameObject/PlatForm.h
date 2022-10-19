#pragma once
#include "../MainGame.h"
bool OnJumpAheadToPlatForm = false;
float BounceStrength = 10.0f;
//why the ID first created is not the same later ? ? ? ? ?
//This cause me for 3 hours;) I thought my logic was wrong
void UpdatePlatformID()
{
	std::vector<int> vPlatforms = Play::CollectGameObjectIDsByType(TYPE_ISLAND);

	//return;
	for (int id_platform : vPlatforms)
	{
		GameObject& obj_platform = Play::GetGameObject(id_platform);
		for (Platform& platform : gameState.vPlatforms)
		{
			if (platform.Pos == obj_platform.pos)
			{
				platform.platform_id = id_platform;
			}
		}
	}

}

void UpdatePlatForm()
{
	UpdatePlatformID();
	std::vector<int> vPlatforms = Play::CollectGameObjectIDsByType(TYPE_ISLAND);

	for (int id_platform : vPlatforms)
	{
		GameObject& obj_platform = Play::GetGameObject(id_platform);
		
		if(id_platform == gameState.BouncePlatformID)
		{
			if (OnJumpAheadToPlatForm)
			{
				//Play::DrawFontText("64px", std::to_string(id_platform), { obj_platform.pos.x, obj_platform.pos.y - 300 }, Play::CENTRE);
				obj_platform.velocity = { 0, BounceStrength };
				OnJumpAheadToPlatForm = false;
				//Play::DrawFontText("64px", "Hit", { obj_platform.pos.x, obj_platform.pos.y - 400 }, Play::CENTRE);
			}
			else
			{
				if (obj_platform.pos.y <= gameState.BouncePlatformPos.y)
				{
					obj_platform.acceleration = { 0, 0 };
					obj_platform.velocity = { 0, 0 };
					obj_platform.pos = gameState.BouncePlatformPos;
				}
				else
					obj_platform.acceleration = { 0, -2.0f };
			}
			/*if (obj_platform.pos.y <= obj_platform.oldPos.y)
			{
				obj_platform.pos = obj_platform.oldPos;
			}*/
			//Matrix2D move{ {1,0,0},{0,1,0},{0,0,1} };
			//::DrawSprite(obj_platform.spriteId, {obj_platform.pos.x, obj_platform.pos.y + 50}, 0);
			//Play::DrawSpriteTransformed(obj_platform.spriteId, move, 0.1f);
			Play::UpdateGameObject(obj_platform);
			Play::DrawObjectRotated(obj_platform);
		}
		else
		{
			Play::DrawSprite(obj_platform.spriteId, { obj_platform.pos.x, obj_platform.pos.y }, 0);
		}
		
	}
	//OnJumpAheadToPlatForm = true;
}