////////////////////////////////////////////////////////////////////////////////////
// A simple platform game editor for Baamageddon using the PlayBuffer framework.
// Copyright 2020 Sumo Digital Limited
///////////////////////////////////////////////////////////////////////////////////
//
// Note that this editor loads all its sprites from the Baamageddon project 
// directory and modifies the data in Baamageddon\Level.lev to change the level.
// Debugging->Working Directory should be to be set to ..\Baamageddon
//
///////////////////////////////////////////////////////////////////////////////////
#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

#include "Play.h"

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

constexpr int SNAP_PIXELS = 32;

const Point2f HALF_DISPLAY{ DISPLAY_WIDTH / 2.0f, DISPLAY_HEIGHT / 2.0f };

constexpr const char* SHEEP_SPRITE_NAME = "spr_sheep1_idle_right";

constexpr const char* ISLAND_A_SPRITE_NAME = "spr_island_A";
constexpr const char* ISLAND_B_SPRITE_NAME = "spr_island_B";
constexpr const char* ISLAND_C_SPRITE_NAME = "spr_island_C";
constexpr const char* ISLAND_D_SPRITE_NAME = "spr_island_D";

constexpr const char* DOUGHNUT_SPRITE_NAME = "spr_doughnut_12";

constexpr int FLOOR_BOUND = DISPLAY_HEIGHT * 2;

//*****************NEW WORK*****************
constexpr const char* SPIKES_SPRITE_NAME = "spr_spikes";
constexpr const char* SPNNINGBLADE_SPRITE_NAME = "spr_spinning_blade";
constexpr const char* MARKER_SPRITE_NAME = "spr_invisible_marker";
constexpr const char* EXITDOUGHNUT_SPRITE_NAME = "level_exit";
constexpr const char* BOUNCYBUSH_SPRITE_NAME = "spr_bouncy_bush_4";
constexpr const char* SWINGINGBLADE_SPRITE_NAME = "spr_swinging_blade";
constexpr const char* WORFLEFT_SPRITE_NAME = "spr_wolf_left_3";
constexpr const char* WORFRIGHT_SPRITE_NAME = "spr_wolf_right_3";

enum GameObjectType
{
	TYPE_NOONE = -1,
	TYPE_SHEEP,
	TYPE_ISLAND,
	TYPE_DOUGHNUT,
	//***********NEW Type***********
	TYPE_SPIKES,
	TYPE_SPINNINGBLADE,
	TYPE_MARKER,
	TYPE_EXIT,
	TYPE_BOUNCEBUSHES,
	TYPE_SWINGBLADE,
	TYPE_WOLFLEFT,
	TYPE_WOLFRIGHT,
	//***********NEW Type***********
	TOTAL_TYPES
};
						//****CHANGE NUMBER*****
const char* SPRITE_NAMES[TOTAL_TYPES][4] =
{
	{ SHEEP_SPRITE_NAME, SHEEP_SPRITE_NAME, SHEEP_SPRITE_NAME, SHEEP_SPRITE_NAME },
	{ ISLAND_A_SPRITE_NAME, ISLAND_B_SPRITE_NAME, ISLAND_C_SPRITE_NAME, ISLAND_D_SPRITE_NAME },
	{ DOUGHNUT_SPRITE_NAME, DOUGHNUT_SPRITE_NAME, DOUGHNUT_SPRITE_NAME, DOUGHNUT_SPRITE_NAME },
	//********************NEW NAME*****************
	{ SPIKES_SPRITE_NAME,SPIKES_SPRITE_NAME,SPIKES_SPRITE_NAME,SPIKES_SPRITE_NAME },
	{ SPNNINGBLADE_SPRITE_NAME,SPNNINGBLADE_SPRITE_NAME,SPNNINGBLADE_SPRITE_NAME,SPNNINGBLADE_SPRITE_NAME },
	{ MARKER_SPRITE_NAME,MARKER_SPRITE_NAME,MARKER_SPRITE_NAME,MARKER_SPRITE_NAME },
	{ EXITDOUGHNUT_SPRITE_NAME,EXITDOUGHNUT_SPRITE_NAME,EXITDOUGHNUT_SPRITE_NAME,EXITDOUGHNUT_SPRITE_NAME },
	{ BOUNCYBUSH_SPRITE_NAME,BOUNCYBUSH_SPRITE_NAME,BOUNCYBUSH_SPRITE_NAME,BOUNCYBUSH_SPRITE_NAME },
	{ SWINGINGBLADE_SPRITE_NAME,SWINGINGBLADE_SPRITE_NAME,SWINGINGBLADE_SPRITE_NAME,SWINGINGBLADE_SPRITE_NAME },
	{ WORFLEFT_SPRITE_NAME,WORFLEFT_SPRITE_NAME,WORFLEFT_SPRITE_NAME,WORFLEFT_SPRITE_NAME },
	{ WORFRIGHT_SPRITE_NAME,WORFRIGHT_SPRITE_NAME,WORFRIGHT_SPRITE_NAME,WORFRIGHT_SPRITE_NAME },


};

struct EditorState
{
	int score = 0;
	GameObjectType editMode = TYPE_BOUNCEBUSHES;
	Point2f cameraTarget{ 0.0f, 0.0f };
	float zoom = 1.0f;
	int selectedObj = -1;
	Point2f selectedOffset{ 0.0f, 0.0f };
	int saveCooldown = 0;
};

EditorState editorState;

void HandleControls();
void DrawScene();
void DrawUserInterface();
bool PointInsideSpriteBounds( Point2f testPos, GameObject& obj );
void DrawObjectsOfType( GameObjectType type );
void SaveLevel();
void LoadLevel();

//-------------------------------------------------------------------------
// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	Play::LoadBackground( "Data\\Backgrounds\\spr_background.png" );
	editorState.cameraTarget = HALF_DISPLAY;
	Play::ColourSprite( "64px", Play::cBlack );
	LoadLevel();
}

//-------------------------------------------------------------------------
// Called by the PlayBuffer once for each frame of the game (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	static float fTotalGameTime = 0.f;
	fTotalGameTime += elapsedTime;

	HandleControls();
	DrawScene();
	DrawUserInterface();

	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

//-------------------------------------------------------------------------
// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

//-------------------------------------------------------------------------
// Camera movement, object placement / deletion etc.
void HandleControls( void )
{
	const int CAMERA_SPEED = 10;

	static bool drag = false;
	static Point2f dragOrigin{ 0.0f, 0.0f };

	if( Play::KeyPressed( 'S' ) && editorState.saveCooldown < 0 )
		SaveLevel();

	if( Play::KeyDown( VK_RIGHT ) )
		editorState.cameraTarget.x += CAMERA_SPEED / editorState.zoom;

	if( Play::KeyDown( VK_LEFT ) )
		editorState.cameraTarget.x -= CAMERA_SPEED / editorState.zoom;

	if( Play::KeyDown( VK_UP ) )
		editorState.cameraTarget.y -= CAMERA_SPEED / editorState.zoom;

	if( Play::KeyDown( VK_DOWN ) )
		editorState.cameraTarget.y += CAMERA_SPEED / editorState.zoom;

	if( Play::KeyPressed( VK_OEM_MINUS ) )// -
		editorState.zoom -= 0.1f;

	if( Play::KeyPressed( VK_OEM_PLUS ) )// +
		editorState.zoom += 0.1f;

	if( editorState.zoom < 0.2f )
		editorState.zoom = 0.2f;

	if( editorState.zoom > 1.0f )
		editorState.zoom = 1.0f;

	if( Play::KeyPressed( VK_SPACE ) )
	{
		switch( editorState.editMode )
		{
			case TYPE_SHEEP: editorState.editMode = TYPE_ISLAND; break;
			case TYPE_ISLAND: editorState.editMode = TYPE_DOUGHNUT; break;
			case TYPE_DOUGHNUT: editorState.editMode = TYPE_SPIKES; break;
				//*****************NEW EDITMODE*****************
			case TYPE_SPIKES: editorState.editMode = TYPE_SPINNINGBLADE; break;
			case TYPE_SPINNINGBLADE: editorState.editMode = TYPE_MARKER; break;
			case TYPE_MARKER: editorState.editMode = TYPE_EXIT; break;
			case TYPE_EXIT: editorState.editMode = TYPE_BOUNCEBUSHES; break;
			case TYPE_BOUNCEBUSHES: editorState.editMode = TYPE_SWINGBLADE; break;
			case TYPE_SWINGBLADE: editorState.editMode = TYPE_WOLFLEFT; break;
			case TYPE_WOLFLEFT: editorState.editMode = TYPE_WOLFRIGHT; break;
			case TYPE_WOLFRIGHT: editorState.editMode = TYPE_SHEEP; break;
		}
		editorState.selectedObj = -1;
	}

	Point2f mouseWorldPos = ( Play::GetMousePos() + Play::GetCameraPosition() ) / editorState.zoom;
	Point2f mouseWorldSnapPos = mouseWorldPos;
	mouseWorldSnapPos.x -= (int)mouseWorldSnapPos.x % SNAP_PIXELS;
	mouseWorldSnapPos.y -= (int)mouseWorldSnapPos.y % SNAP_PIXELS;

	if( Play::GetMouseButton( Play::LEFT ) )
	{
		if( editorState.selectedObj == -1 )
		{
			for( int id : Play::CollectGameObjectIDsByType( editorState.editMode ) )
			{
				GameObject& obj = Play::GetGameObject( id );
				if( PointInsideSpriteBounds( mouseWorldPos, obj ) )
				{
					editorState.selectedObj = obj.GetId();
					editorState.selectedOffset = obj.pos - mouseWorldSnapPos;
				}
			}

			if( editorState.selectedObj == -1 )
			{
				switch( editorState.editMode )
				{
					case TYPE_SHEEP:
						Play::GetGameObjectByType( TYPE_SHEEP ).pos = mouseWorldPos;
						break;
					default:
						editorState.selectedObj = Play::CreateGameObject( editorState.editMode, mouseWorldSnapPos, 50, SPRITE_NAMES[static_cast<int>( editorState.editMode )][0] );
						editorState.selectedOffset = { 0.0f, 0.0f };
						break;
				}
			}

		}
		else
		{
			GameObject& obj = Play::GetGameObject( editorState.selectedObj );
			obj.pos = mouseWorldSnapPos + editorState.selectedOffset;

			if( Play::KeyDown( '1' ) )
				obj.spriteId = Play::GetSpriteId( SPRITE_NAMES[static_cast<int>( editorState.editMode )][0] );

			if( Play::KeyDown( '2' ) )
				obj.spriteId = Play::GetSpriteId( SPRITE_NAMES[static_cast<int>( editorState.editMode )][1] );

			if( Play::KeyDown( '3' ) )
				obj.spriteId = Play::GetSpriteId( SPRITE_NAMES[static_cast<int>( editorState.editMode )][2] );

			if( Play::KeyDown( '4' ) )
				obj.spriteId = Play::GetSpriteId( SPRITE_NAMES[static_cast<int>( editorState.editMode )][3] );
		}
	}
	else
	{
		editorState.selectedObj = -1;
	}

	if( Play::GetMouseButton( Play::RIGHT ) )
	{
		for( int id : Play::CollectGameObjectIDsByType( editorState.editMode ) )
		{
			GameObject& obj = Play::GetGameObject( id );
			if( PointInsideSpriteBounds( mouseWorldPos, obj ) )
			{
				if( obj.type != TYPE_SHEEP )
					Play::DestroyGameObject( id );
			}
		}
	}

	Play::SetCameraPosition( ( editorState.cameraTarget * editorState.zoom ) - HALF_DISPLAY );
}

//-------------------------------------------------------------------------
void DrawScene( void )
{
	Play::DrawBackground();

	DrawObjectsOfType( TYPE_ISLAND );
	DrawObjectsOfType( TYPE_DOUGHNUT );
	DrawObjectsOfType( TYPE_SHEEP );
	//*****************Draw new static img*****************
	DrawObjectsOfType(TYPE_SPIKES);
	DrawObjectsOfType(TYPE_SPINNINGBLADE);
	DrawObjectsOfType(TYPE_MARKER);
	DrawObjectsOfType(TYPE_EXIT);
	DrawObjectsOfType(TYPE_BOUNCEBUSHES);
	DrawObjectsOfType(TYPE_SWINGBLADE);
	DrawObjectsOfType(TYPE_WOLFLEFT);
	DrawObjectsOfType(TYPE_WOLFRIGHT);

	if( editorState.selectedObj != -1 )
	{
		GameObject& obj = Play::GetGameObject( editorState.selectedObj );
		Point2f origin = Play::GetSpriteOrigin( obj.spriteId );
		Point2f size = { Play::GetSpriteWidth( obj.spriteId ), Play::GetSpriteHeight( obj.spriteId ) };
		Play::DrawRect( ( obj.pos - origin ) * editorState.zoom, ( obj.pos - origin + size ) * editorState.zoom, Play::cWhite );
		std::string s = "X:" + std::to_string( (int)( obj.pos.x + 0.5f ) ) + " / Y:" + std::to_string( (int)( obj.pos.y + 0.5f ) );
		Play::DrawDebugText( ( obj.pos - origin + Point2f( size.x / 2.0f, -10.0f / editorState.zoom ) ) * editorState.zoom, s.c_str(), Play::cWhite );
	}

}

//-------------------------------------------------------------------------
void DrawUserInterface( void )
{
	Play::SetDrawingSpace( Play::SCREEN );
	std::string sMode;
	switch( editorState.editMode )
	{
		case TYPE_SHEEP: sMode = "PLAYER"; break;
		case TYPE_ISLAND: sMode = "ISLANDS"; break;
		case TYPE_DOUGHNUT: sMode = "DONUTS"; break;
		//*****************Draw new static img*****************
		case TYPE_SPIKES: sMode = "SPIKES"; break;
		case TYPE_SPINNINGBLADE: sMode = "SPINNINGBLADE"; break;
		case TYPE_MARKER: sMode = "MARKER"; break;
		case TYPE_EXIT: sMode = "EXIT"; break;
		case TYPE_BOUNCEBUSHES: sMode = "BOUNCEBUSHES"; break;
		case TYPE_SWINGBLADE: sMode = "SWINGBLADE"; break;
		case TYPE_WOLFLEFT: sMode = "WOLFLEFT"; break;
		case TYPE_WOLFRIGHT: sMode = "WOLFRIGHT"; break;
	}

	Play::DrawRect( { 0, 0 }, { DISPLAY_WIDTH, 50 }, Play::cYellow, true );
	Play::DrawFontText( "64px", "MODE : " + sMode, { DISPLAY_WIDTH / 2, 25 }, Play::CENTRE );
	Play::DrawFontText( "64px", std::to_string( (int)( ( editorState.zoom * 100.0f ) + 0.5f ) ) + "%", { DISPLAY_WIDTH / 6, 25 }, Play::CENTRE );
	Play::DrawFontText( "64px", std::to_string( Play::CollectGameObjectIDsByType( editorState.editMode ).size() ) + " " + sMode, { ( DISPLAY_WIDTH * 5 ) / 6, 25 }, Play::CENTRE );

	float yBounds = FLOOR_BOUND * editorState.zoom - Play::GetCameraPosition().y;
	Play::DrawLine( { 0, yBounds }, { DISPLAY_WIDTH, yBounds }, Play::cBlack );
	Play::DrawDebugText( { DISPLAY_WIDTH / 2, yBounds + 10 }, "AUTOMATIC DEATH LEVEL", Play::cBlack );

	Play::DrawDebugText( { 20, DISPLAY_HEIGHT - 20 }, "HOLD 'H' FOR CONTROLS", Play::cBlack, false );

	if( Play::KeyDown( 'H' ) )
	{
		Play::DrawRect( { DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4 }, { DISPLAY_WIDTH * 3 / 4, DISPLAY_HEIGHT * 3 / 4 }, Play::cBlack, true );
		Play::DrawRect( { DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4 }, { DISPLAY_WIDTH * 3 / 4, DISPLAY_HEIGHT * 3 / 4 }, Play::cMagenta, false );
		int y = ( DISPLAY_HEIGHT / 4 ) + 50;
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y }, "LEVEL EDITOR HELP", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "---------------------", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 40 }, "NOTE: YOU CAN ONLY INTERACT WITH OBJECTS SPECIFIED BY THE MODE", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 40 }, "SPACE BAR = CHANGE OBJECT MODE", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE BUTTON = ADD OR SELECT OBJECT", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE DRAG = MOVE OBJECT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE DRAG AND KEYS 1-4 = CHANGE OBJECT SPRITE", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "RIGHT MOUSE BUTTON = DELETE OBJECT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "ARROW KEYS = SCROLL", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "PLUS AND MINUS KEYS = ZOOM IN AND OUT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "F1 = SHOW DEBUG INFO", Play::cWhite );
	}

	if( --editorState.saveCooldown > 0 )
	{
		Play::DrawRect( { 0, DISPLAY_HEIGHT - 50 }, { DISPLAY_WIDTH, DISPLAY_HEIGHT }, Play::cOrange, true );
		Play::DrawFontText( "64px", "OVERWRITING LEVEL", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 25 }, Play::CENTRE );
	}

	Play::SetDrawingSpace( Play::WORLD );
}


//-------------------------------------------------------------------------
void DrawObjectsOfType( GameObjectType type )
{
	for( int id : Play::CollectGameObjectIDsByType( type ) )
	{
		GameObject& obj = Play::GetGameObject( id );
		Play::DrawSpriteRotated( obj.spriteId, obj.pos * editorState.zoom, 0, 0, 1.0f * editorState.zoom );
	}
}

//-------------------------------------------------------------------------
bool PointInsideSpriteBounds( Point2f testPos, GameObject& obj )
{
	Point2f origin = Play::GetSpriteOrigin( obj.spriteId );
	Point2f size = { Play::GetSpriteWidth( obj.spriteId ), Play::GetSpriteHeight( obj.spriteId ) };
	Point2f topLeft = obj.pos - origin;
	Point2f botRight = topLeft + size;
	return testPos.x > topLeft.x && testPos.x < botRight.x&& testPos.y > topLeft.y && testPos.y < botRight.y;
}


//-------------------------------------------------------------------------
// Loads the objects from the Baamageddon\Level.lev file
void LoadLevel( void )
{
	std::ifstream levelfile;
	levelfile.open( "Level.lev" );

	std::string sType, sX, sY, sSprite;

	std::getline( levelfile, sType );

	while( !levelfile.eof() )
	{
		std::getline( levelfile, sType );
		std::getline( levelfile, sX );
		std::getline( levelfile, sY );
		std::getline( levelfile, sSprite );

		if( sType == "TYPE_SHEEP" )
			Play::CreateGameObject( TYPE_SHEEP, { std::stof( sX ), std::stof( sY ) }, 50, sSprite.c_str() );

		if( sType == "TYPE_ISLAND" )
			Play::CreateGameObject( TYPE_ISLAND, { std::stof( sX ), std::stof( sY ) }, 0, sSprite.c_str() );

		if( sType == "TYPE_DOUGHNUT" )
			Play::CreateGameObject( TYPE_DOUGHNUT, { std::stof( sX ), std::stof( sY ) }, 30, sSprite.c_str() );
		//*****************Load new File*****************
		if (sType == "TYPE_SPIKES")
			Play::CreateGameObject(	TYPE_SPIKES, { std::stof(sX), std::stof(sY) }, 50, sSprite.c_str());//COLLISION RADIUS
		if (sType == "TYPE_SPINNINGBLADE")
			Play::CreateGameObject(TYPE_SPINNINGBLADE, { std::stof(sX), std::stof(sY) }, 50, sSprite.c_str());//COLLISION RADIUS
		if (sType == "TYPE_MARKER")
			Play::CreateGameObject(TYPE_MARKER, { std::stof(sX), std::stof(sY) }, 30, sSprite.c_str());//COLLISION RADIUS
		if (sType == "TYPE_EXIT")
			Play::CreateGameObject(TYPE_EXIT, { std::stof(sX), std::stof(sY) }, 60, sSprite.c_str());//COLLISION RADIUS
		if (sType == "TYPE_BOUNCEBUSHES")
			Play::CreateGameObject(TYPE_BOUNCEBUSHES, { std::stof(sX), std::stof(sY) }, 40, sSprite.c_str());
		if (sType == "TYPE_SWINGBLADE")
			Play::CreateGameObject(TYPE_SWINGBLADE, { std::stof(sX), std::stof(sY) }, 40, sSprite.c_str());
		if (sType == "TYPE_WOLFLEFT")
			Play::CreateGameObject(TYPE_WOLFLEFT, { std::stof(sX), std::stof(sY) }, 40, sSprite.c_str());
		if (sType == "TYPE_WOLFRIGHT")
			Play::CreateGameObject(TYPE_WOLFRIGHT, { std::stof(sX), std::stof(sY) }, 40, sSprite.c_str());

	}

	levelfile.close();
}

//-------------------------------------------------------------------------
// Outputs the objects to the Baamageddon\Level.lev file
void SaveLevel( void )
{
	std::ofstream levelfile;
	levelfile.open( "Level.lev" );

	levelfile << "// This file is auto-generated by the Level Editor - it's not advisable to edit it directly as changes may be overwritten!\n";

	for( int id : Play::CollectAllGameObjectIDs() )
	{
		GameObject& obj = Play::GetGameObject( id );
		switch( obj.type )
		{
			case TYPE_SHEEP:
				levelfile << "TYPE_SHEEP\n";
				break;
			case TYPE_ISLAND:
				levelfile << "TYPE_ISLAND\n";
				break;
			case TYPE_DOUGHNUT:
				levelfile << "TYPE_DOUGHNUT\n";
				break;
			//*****************Load new File*****************
			case TYPE_SPIKES:
				levelfile << "TYPE_SPIKES\n";
				break;
			case TYPE_SPINNINGBLADE:
				levelfile << "TYPE_SPINNINGBLADE\n";
				break;
			case TYPE_MARKER:
				levelfile << "TYPE_MARKER\n";
				break;
			case TYPE_EXIT:
				levelfile << "TYPE_EXIT\n";
				break;
			case TYPE_BOUNCEBUSHES:
				levelfile << "TYPE_BOUNCEBUSHES\n";
				break;
			case TYPE_SWINGBLADE:
				levelfile << "TYPE_SWINGBLADE\n";
				break;
			case TYPE_WOLFLEFT:
				levelfile << "TYPE_WOLFLEFT\n";
				break;
			case TYPE_WOLFRIGHT:
				levelfile << "TYPE_WOLFRIGHT\n";
				break;

		}
		levelfile << std::to_string( obj.pos.x ) + "f\n" << std::to_string( obj.pos.y ) + "f\n";
		levelfile << Play::GetSpriteName( obj.spriteId ) << "\n";
	}
	
	levelfile.close();

	editorState.saveCooldown = 100;
}