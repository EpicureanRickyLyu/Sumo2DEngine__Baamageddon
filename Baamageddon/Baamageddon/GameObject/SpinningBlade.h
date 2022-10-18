#pragma once

#include "../MainGame.h"
Vector2D BladeSpeed = { -1.0f ,0 };
void InitializeBlade()
{
	std::vector<int> obj_SpinningBlades = Play::CollectGameObjectIDsByType(TYPE_SPINNINGBLADE);
	for (int id_balde : obj_SpinningBlades)
	{
		GameObject& obj_blade = Play::GetGameObject(id_balde);
		//init speed
		obj_blade.velocity = BladeSpeed;
		obj_blade.rotSpeed = 3.0f;
	}

}
void UpdateSpinningBlade()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_SpinningBlades = Play::CollectGameObjectIDsByType(TYPE_SPINNINGBLADE);
	//std::vector<int> obj_Hazards = Play::CollectGameObjectIDsByType(TYPE_);
	std::vector<int> obj_Markers = Play::CollectGameObjectIDsByType(TYPE_MARKER);
	/*if (gameState.playState == PlayState::STATE_WAIT)
		return;*/

	for (int id_balde : obj_SpinningBlades)
	{
		GameObject& obj_blade = Play::GetGameObject(id_balde);
		bool hasCollided = false;
		if (Play::IsColliding(obj_blade, obj_sheep))
		{
			if (!hasCollided)
			{
				obj_sheep.velocity.y = -SHEEP_JUMP_IMPULSE;
				hasCollided = true;
				gameState.playState = PlayState::STATE_DEAD;
				Play::PlayAudio("munch");
			}
		}
		//collider with marker
		for (int id_marker : obj_Markers)
		{
			GameObject& obj_Marker = Play::GetGameObject(id_marker);

			if (Play::IsColliding(obj_blade, obj_Marker))
			{
					obj_blade.velocity = -obj_blade.velocity;
					obj_blade.rotSpeed = -obj_blade.rotSpeed;
			}
		}
		Play::UpdateGameObject(obj_blade);
		Play::DrawObjectRotated(obj_blade);
	}

}