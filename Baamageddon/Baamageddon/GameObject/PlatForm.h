#pragma once

#include "../MainGame.h"
bool OnJumpAheadToPlatForm = false;

void UpdatePlatForm()
{
	std::vector<int> vPlatforms = Play::CollectGameObjectIDsByType(TYPE_ISLAND);
	if (!OnJumpAheadToPlatForm)
		return;

	//for (auto &_platform : gameState.vPlatforms)
	//{
	//	int id  = _platform.platform_id;

	//	GameObject& obj_island = Play::GetGameObject(id);

	//	Play::UpdateGameObject(obj_island);
	//	Play::DrawObjectRotated(obj_island);
	//}

	for (int id_platform : vPlatforms)
	{
	
		GameObject& obj_island = Play::GetGameObject(id_platform);
		if (id_platform == gameState.BouncePlatformid)
		{
			Play::DrawFontText("64px", std::to_string(obj_island.GetId()), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);
			//continue;
			Play::UpdateGameObject(obj_island);
			Play::DrawObjectRotated(obj_island);
		}
		//Play::DrawFontText("64px", std::to_string(obj_island.GetId()), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);

	}
	//OnJumpAheadToPlatForm = true;
}