#pragma once
#include "../MainGame.h"
float SwingSpeed = 0.02f;
void IntilizeSwingBlade()
{
	Play::MoveSpriteOrigin(SWINGINGBLADE_SPRITE_NAME, 0, -146);
}
bool SwingBladeCollider(GameObject& player, GameObject& SwingBlade)
{
	//Based on Radian Customise Collider
	float radius = 70;
	Vector2D ColliderCenter = { SwingBlade.pos.x - sin(SwingBlade.rotation) * 280,SwingBlade.pos.y + cos(SwingBlade.rotation) * 280 };
	//Easy to see, could comment in the future
	Play::DrawCircle(ColliderCenter, 70, Play::cRed);
	if ((player.pos - ColliderCenter).Length() < radius + 20)
	{
		return true;
	}
	return false;
}
void UpdateSwingBlade()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_Swings = Play::CollectGameObjectIDsByType(TYPE_SWINGBLADE);
	/*if (gameState.playState == PlayState::STATE_WAIT)
		return;*/
	
	for (int id_balde : obj_Swings)
	{
		GameObject& obj_blade = Play::GetGameObject(id_balde);
		bool hasCollided = false;
		//draw collider box
		if(SwingBladeCollider(obj_sheep, obj_blade))
		{
			if (!hasCollided)
			{
				obj_sheep.velocity.y = -SHEEP_JUMP_IMPULSE;
				hasCollided = true;
				gameState.playState = PlayState::STATE_DEAD;
				Play::PlayAudio("munch");
				hasCollided = true;
			}
		}
		obj_blade.rotSpeed = SwingSpeed;


	
		Play::UpdateGameObject(obj_blade);
		Play::DrawObjectRotated(obj_blade);
	}
}