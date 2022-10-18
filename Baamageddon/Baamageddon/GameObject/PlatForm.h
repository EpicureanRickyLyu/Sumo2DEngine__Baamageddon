#pragma once
#include "../MainGame.h"
bool OnJumpAheadToPlatForm = false;

void UpdatePlatForm()
{
	std::vector<int> vPlatforms = Play::CollectGameObjectIDsByType(TYPE_ISLAND);
	//if (!OnJumpAheadToPlatForm)
	//	return;
	
	

	for (int id_platform : vPlatforms)
	{
		GameObject& obj_platform = Play::GetGameObject(id_platform);
		if(obj_platform.pos == gameState.BouncePlatformPos)
		{
			//obj_platform.velocity = { 0,1.0f };
			//obj_platform.acceleration = { 0, -0.3f };
			/*if (obj_platform.pos.y <= obj_platform.oldPos.y)
			{
				obj_platform.pos = obj_platform.oldPos;
			}*/

			Play::UpdateGameObject(obj_platform);
			Play::DrawObjectRotated(obj_platform);
		}
	}
	//OnJumpAheadToPlatForm = true;
}