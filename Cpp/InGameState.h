#pragma once
#include "State.h"
#include <input/input_manager.h>
#include <graphics/sprite.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include <input/touch_input_manager.h>


class InGameState: public State
{
public:
	InGameState();
	~InGameState();

	///Function for Initializing Player control Inputs
	void ControllerInput(gef::InputManager *input_manager_, gef::AudioManager * audio_manager_);

	///Function for Initializing fonts for the UI
	void InitFont(gef::Platform&platform_, gef::Font* font_);

	///Functtion for Initializing the Game State Function
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Function for Cleaning Up GameState Models and Sprites
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	///Function for Updating GameState
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Function that Render the Sprites and Models in GameState
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	///Initializes the Background for the Game
	void InitBackground(gef::Platform&platform_);

	///Function that delets the font in the game
	void CleanUpFont(gef::Font* font_);

	///Function that Declares the Losing condition for the game
	void LosingCondition();

	///Function that declares the Wininig condition for the game
	void WiningCondition();

	///Function that draws the HUD for the game
	void DrawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_);

	///Function that Applies a Upward force to Player when gravity is appling downward force
	void ApplyPlayerForce();

	///Sets up the Light for the Game State
	void SetupLights();

	///Delets the Player Bullets which are in a List
	void DeletePlayerBullets();

	///Moves the Player bullet forward in x Direction by Appling Force
	void MovePlayerBullets();

	///Delets the EnemyBullets 
	void DeleteEnemyBullets();

	///Spawns the EnemyBullet based on the condition
	void SpawnEnemyBullet(gef::AudioManager * audio_manager_);

	///RayCollision which Acts as Raycast  
	void RayCollision(gef::AudioManager * audio_manager_);

	///Function that checks the Collison between GameObjects
	void Collision(gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Load One Section of Init
	void InitFirst( gef::Platform&platform_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Load Second Section of Init
	void InitSecond( gef::Platform&platform_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Load Third Section of Init
	void InitThird( gef::Platform&platform_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Declare all the UpdateSimulation for the different Objects
	void PhysicsManager();

	///Reinstantiate the player back to it's starting position
	void RespawnPlayer(gef::Platform & platform_);

	///Delete the Enemy When they go of the screen
	void DeleteEnemy();

	///Move Player bullet is it's true
	void checkingPlayerBullet();


	///Variable for Initializing the Render3D
	gef::Renderer3D* renderer_3d_;

	///create the physics world
	b2World* world_;
	
	///Initializing the Sound Variable for the Game State
	int32 player_bullet_sfx_id = 0;
	int32 enemy_bullet_sfx_id = 0;
	int32 enemy_imapct_sfx_id = 0;
	int32 player_impact_sfx_id = 0;

	///Variable for Rendering the Background 
	gef::MeshInstance mesh_instance_;
	gef::Scene* scene_assets_;
	const char* scene_asset_filename;
	LoadModel BackgroundModel;

	/// ground variables
	Ground ground_ref_;
	
	///Top Celling Variables
	TopCelling celling_ref_;

	///Bollean which checks if it's Loading
	bool is_loading;

	///Load time variable
	float Loadtime = 0.f;

	///Sprites used for Loading Screen
	gef::Sprite Loading_Background;
	gef::Sprite LoadingBarHalf_Background;
	gef::Sprite LoadingBarFull_Background;

	///Textured used for Loading Screen
	gef::Texture* Loading_Text_;
	gef::Texture* LoadingBarHalf_Text_;
	gef::Texture* LoadingBarFull_Text_;
	

	///Variable for calculating the FPS for the game
	float fps_;

};

