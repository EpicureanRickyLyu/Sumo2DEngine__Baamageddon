#pragma once

///////////////////////////////////////////////////////////////////////////
// Baamageddon - A simple platform game using the PlayBuffer framework.
// Copyright 2020 Sumo Digital Limited
///////////////////////////////////////////////////////////////////////////
// 
//-------------------------------------------------------------------------

#include "Play.h"
#include "AABB.h"

//-------------------------------------------------------------------------
constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

constexpr float SHEEP_WALK_SPEED = 5.0f;
constexpr float SHEEP_JUMP_IMPULSE = 14.f;

constexpr const char* SHEEP_IDLE_LEFT_SPRITE_NAME = "spr_sheep1_idle_left";
constexpr const char* SHEEP_IDLE_RIGHT_SPRITE_NAME = "spr_sheep1_idle_right";
constexpr const char* SHEEP_WALK_LEFT_SPRITE_NAME = "spr_sheep1_walk_left";
constexpr const char* SHEEP_WALK_RIGHT_SPRITE_NAME = "spr_sheep1_walk_right";
constexpr const char* SHEEP_JUMP_LEFT_SPRITE_NAME = "spr_sheep1_jump_left";
constexpr const char* SHEEP_JUMP_RIGHT_SPRITE_NAME = "spr_sheep1_jump_right";

constexpr const char* ISLAND_A_SPRITE_NAME = "spr_island_A";
constexpr const char* ISLAND_B_SPRITE_NAME = "spr_island_B";
constexpr const char* ISLAND_C_SPRITE_NAME = "spr_island_C";
constexpr const char* ISLAND_D_SPRITE_NAME = "spr_island_D";

const Point2f SHEEP_COLLISION_HALFSIZE = { 40,40 };

constexpr const char* DOUGHNUT_SPRITE_NAME = "spr_doughnut_12";
constexpr const char* SPRINKLE_SPRITE_NAME = "spr_sprinkle";
constexpr const char* SCORE_TAB_SPRITE_NAME = "spr_score_tab";

constexpr int LEFT_SCREEN_BOUND = 100;
constexpr int RIGHT_SCREEN_BOUND = DISPLAY_WIDTH - LEFT_SCREEN_BOUND;

constexpr int FLOOR_BOUND = DISPLAY_HEIGHT * 2;


//*****************New GameObject Image*****************
constexpr const char* SPIKES_SPRITE_NAME = "spr_spikes";
constexpr const char* SPNNINGBLADE_SPRITE_NAME = "spr_spikes";

//-------------------------------------------------------------------------

enum PlayState
{
	STATE_START = 0,
	STATE_APPEAR,
	STATE_PLAY,
	STATE_DEAD,
	STATE_WAIT
};

//-------------------------------------------------------------------------

enum SheepState
{
	STATE_IDLE = 0,
	STATE_WALKING,
	STATE_AIRBORNE
};

//-------------------------------------------------------------------------

enum SheepDirection
{
	DIRECTION_LEFT = 0,
	DIRECTION_RIGHT = 1
}; 

//-------------------------------------------------------------------------

enum GameObjectType
{
	TYPE_NOONE = -1,
	TYPE_SHEEP,
	TYPE_ISLAND,
	TYPE_CLOUD,
	TYPE_DOUGHNUT,
	TYPE_SPRINKLE,
	//***********NEW TYPE********
	TYPE_SPIKES,
	TYPE_SPINNINGBLADE,
	TYPE_MARKER,
}; 

//-------------------------------------------------------------------------

struct Platform
{
	AABB box;
	int platform_id;
};

//-------------------------------------------------------------------------

struct GameState
{
	int score = 0;
	PlayState playState = STATE_START;
	SheepState sheepState = STATE_IDLE;
	SheepDirection sheepDirection = DIRECTION_RIGHT;
	std::vector< Platform > vPlatforms;
	Point2f cameraTarget{ 0.0f, 0.0f };
}; 

//-------------------------------------------------------------------------

static GameState gameState;

//-------------------------------------------------------------------------

void CreatePlatforms();

void DrawScene();

void UpdateDoughnuts();

void UpdateSprinkles();

void UpdateDestroyed();

void UpdateGamePlayState();

void SetAirborne(GameObject& obj_sheep);

void DrawCollisionBounds(GameObject& obj_sheep);

void DisplayDebugInfo(GameObject& obj_sheep);

void PrintMouseCoordinateList();

void TestAABBSegmentTest();

void RandomBaa();

void LoadLevel();



