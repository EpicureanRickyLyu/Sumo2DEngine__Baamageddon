#pragma once
#include "../MainGame.h"
float alertDistance = 400;
float attackDistance = 300;
float KillDistance = 100;
float AnimSpeed = 0.1f;

//need a wolf class maybe
void isInLeftViewCone(GameObject& player, GameObject& wolf,bool &isLeftWolfAttack, bool isDead)
{
	Vector2D dis = player.pos - wolf.pos;
	Vector2D LeftDir = { -1.0f, 0 };
	Vector2D temp = dis;
	temp.Normalize();
	//Play::DrawFontText("64px", std::to_string(dis.Length()), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);
	//will be bigger and bigger number
	if(LeftDir.Dot(temp) > 0)
	if (dis.Length() < alertDistance && dis.Length() > attackDistance)
	{
		Play::DrawSprite(wolf.spriteId, wolf.pos, 1);
	}
	else if (dis.Length() < alertDistance && dis.Length() < attackDistance)
	{
		//attack
		isLeftWolfAttack = true;
		Play::DrawSprite(wolf.spriteId, wolf.pos, 2);
	}
	else
	{
		Play::DrawSprite(wolf.spriteId, wolf.pos, 0);
	}
	else
	{
		if (dis.Length() < KillDistance)
		{
			//wolf dead
			wolf.acceleration = { -0.3f , 0.5f };

			isDead = true;
			Play::DrawSprite(wolf.spriteId, wolf.pos, 2);
			Play::PlayAudio("munch");
			//Play::DestroyGameObject(wolf.GetId());
		}
		else
		Play::DrawSprite(wolf.spriteId, wolf.pos, 0);
	}
}

void UpdateLeftWolf()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_wolfs = Play::CollectGameObjectIDsByType(TYPE_WOLFLEFT);
	if (gameState.playState == PlayState::STATE_WAIT)
		return;

	for (int id_lwolf : obj_wolfs)
	{
		GameObject& obj_lwolf = Play::GetGameObject(id_lwolf);
		//
		bool isLeftWolfAttack = false;
		bool isDead = false;
		//
		if(!isLeftWolfAttack && !isDead)
		isInLeftViewCone(obj_sheep, obj_lwolf, isLeftWolfAttack, isDead);
		bool hasCollided = false;

		if (isLeftWolfAttack)
		{
			obj_lwolf.velocity = { -10.0f,2.5f };
			if (Play::IsColliding(obj_lwolf, obj_sheep))
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
		Play::UpdateGameObject(obj_lwolf);
		//if (isLeftWolfAttack)
		//{
		//	if(!Play::IsAnimationComplete(obj_lwolf))
		//	//Play::DrawObjectRotated(obj_lwolf);
		//	else
		//	Play::DestroyGameObject(obj_lwolf.GetId());
		//}

		if (isDead)
		{
			obj_lwolf.rotation += 0.25f;
			//Play::DrawObjectRotated(obj_lwolf);
			Play::DrawObjectRotated(obj_lwolf);
			Play::DestroyGameObject(obj_lwolf.GetId());
		}
		if (Play::IsLeavingDisplayArea(obj_lwolf))
		{
			Play::DestroyGameObject(obj_lwolf.GetId());
		}

	}
}

//--------------------------RightWolf--------------------------------------------
void isInRighttViewCone(GameObject& player, GameObject& wolf, bool& isLeftWolfAttack, bool isDead)
{
	Vector2D dis = player.pos - wolf.pos;
	Vector2D LeftDir = { 1.0f, 0 };
	Vector2D temp = dis;
	temp.Normalize();
	//Play::DrawFontText("64px", std::to_string(dis.Length()), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);
	//will be bigger and bigger number
	if (LeftDir.Dot(temp) > 0)
		if (dis.Length() < alertDistance && dis.Length() > attackDistance)
		{
			Play::DrawSprite(wolf.spriteId, wolf.pos, 1);
		}
		else if (dis.Length() < alertDistance && dis.Length() < attackDistance)
		{
			//attack
			isLeftWolfAttack = true;
			Play::DrawSprite(wolf.spriteId, wolf.pos, 2);
		}
		else
		{
			Play::DrawSprite(wolf.spriteId, wolf.pos, 0);
		}
	else
	{
		if (dis.Length() < KillDistance)
		{
			//wolf dead
			wolf.acceleration = { 0.7f , 0.5f };
			wolf.rotation += 0.25f;
			Play::DrawSprite(wolf.spriteId, wolf.pos, 2);
			isDead = true;
			Play::PlayAudio("munch");
			//Play::DestroyGameObject(wolf.GetId());
		}
		else
			Play::DrawSprite(wolf.spriteId, wolf.pos, 0);
	}
}

void UpdateRighttWolf()
{
	GameObject& obj_sheep = Play::GetGameObjectByType(TYPE_SHEEP);
	std::vector<int> obj_wolfs = Play::CollectGameObjectIDsByType(TYPE_WOLFRIGHT);
	if (gameState.playState == PlayState::STATE_WAIT)
		return;

	for (int id_lwolf : obj_wolfs)
	{
		GameObject& obj_lwolf = Play::GetGameObject(id_lwolf);
		//
		bool isLeftWolfAttack = false;
		bool isDead = false;
		//
		isInRighttViewCone(obj_sheep, obj_lwolf, isLeftWolfAttack, isDead);
		bool hasCollided = false;

		if (isLeftWolfAttack)
		{
			obj_lwolf.velocity = { 10.0f,2.5f };
			if (Play::IsColliding(obj_lwolf, obj_sheep))
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
		Play::UpdateGameObject(obj_lwolf);
	/*	if (isLeftWolfAttack)
			Play::DrawObjectRotated(obj_lwolf);*/

		if (isDead)
		{
			//Play::DrawObjectRotated(obj_lwolf);
			Play::DestroyGameObject(obj_lwolf.GetId());
		}
		if (Play::IsLeavingDisplayArea(obj_lwolf))
		{
			Play::DestroyGameObject(obj_lwolf.GetId());
		}

	}
}