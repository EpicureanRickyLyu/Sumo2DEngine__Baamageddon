#pragma once
#include "../MainGame.h"
void UpdateSpikes()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_Spikes = Play::CollectGameObjectIDsByType(TYPE_SPIKES);
	if (gameState.playState == PlayState::STATE_WAIT)
		return;
	for (int id_spike : obj_Spikes)
	{
		GameObject& obj_Spike = Play::GetGameObject(id_spike);
		bool hasCollided = false;
		if (Play::IsColliding(obj_Spike, obj_sheep))
		{
			if (!hasCollided)
			{
				obj_sheep.velocity.y = -SHEEP_JUMP_IMPULSE;
				hasCollided = true;
				gameState.playState = PlayState::STATE_DEAD;
				Play::PlayAudio("munch");
			}
		}
	}
}