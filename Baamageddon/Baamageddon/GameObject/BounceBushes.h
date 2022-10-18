#pragma once
#include "../MainGame.h"

void UpdateBouceBushes()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_bushes = Play::CollectGameObjectIDsByType(TYPE_BOUNCEBUSHES);
	if (gameState.playState == PlayState::STATE_WAIT)
		return;
	for (int id_bush : obj_bushes)
	{
		GameObject& obj_bush = Play::GetGameObject(id_bush);
		bool hasCollided = false;
		if (Play::IsColliding(obj_bush, obj_sheep) && gameState.sheepState == SheepState::STATE_AIRBORNE)
		{
			if (!hasCollided)
			{
				obj_sheep.velocity.y = -SHEEP_JUMP_IMPULSE;
				Play::SetSprite(obj_bush, BOUNCYBUSH_SPRITE_NAME, 8.0f);
				Play::PlayAudio("munch");
			}
		}
		else
		{
			Play::SetSprite(obj_bush, BOUNCYBUSH_SPRITE_NAME, 0);
		}
		Play::UpdateGameObject(obj_bush);
		Play::DrawObjectRotated(obj_bush);
	}

}