#pragma once
#include "../MainGame.h"
float CradleSpeed = 0.2f;
void InitializeCradle()
{

}
bool isDamaged(GameObject& blade, GameObject& obj_sheep)
{
	Vector2D dis = obj_sheep.pos - blade.pos;
	Vector2D LeftDir = { -1.0f, 0 };
	Vector2D RightDir = { 1.0f, 0 };
	Vector2D temp = dis;
	temp.Normalize();
	//if left right angle < 60
	if (LeftDir.Dot(temp) > 0.6 || RightDir.Dot(temp) > 0.6)
	{
		return true;
	}
	else
		return false;

}
//bool OnBlade = false;
void UpdateCradle()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	for (auto& cradle : gameState.cradles)
	{
		GameObject& blade = Play::GetGameObject(cradle.id_CenterObj);
		GameObject& rope = Play::GetGameObject(cradle.Rope.first);

		//rope.rotSpeed = SwingSpeed;
		if (gameState.playState == PlayState::STATE_DEAD)
			return;
		if (Play::IsColliding(obj_sheep, blade))
		{
			if (obj_sheep.velocity.y >= 0.0f)
			{
				obj_sheep.pos.y -= 2;
				gameState.sheepState = STATE_IDLE;
				obj_sheep.velocity.y = 0.f;
				obj_sheep.acceleration.y = 0.f;
				//OnBlade = true;
			}
			else if (isDamaged(blade, obj_sheep))
			{
				obj_sheep.velocity.y = -SHEEP_JUMP_IMPULSE;
				gameState.playState = PlayState::STATE_DEAD;
				Play::PlayAudio("munch");
			}
			else
			{
				obj_sheep.pos.y = blade.pos.y + 100;
				obj_sheep.velocity.y = 0.f;
			}

		}

		Play::UpdateGameObject(blade);
		Play::UpdateGameObject(rope);
		Play::DrawObjectRotated(blade);
		Play::DrawObjectRotated(rope);
	}
}