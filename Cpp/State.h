#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include <audio\audio_manager.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include "graphics\font.h"
#include "graphics/scene.h"
#include "game_object.h"
#include "BulletManager.h"
#include "LoadModel.h"
#include "graphics\renderer_3d.h"


enum GAMESTATE
{
	NO_STATE,
	INIT,
	MENU,
	CREDITS,
	OPTIONS,
	LEVEL1,
	LOSE,
	WIN,
	MODE,
	LOADING

};

class State
{
public:
	State();
	~State();

	///Variables for Menu Background
	gef::Sprite menu_BackGround_;
	gef::Sprite Play_Again_BackGround_;

	///Variables for texture for the Menu and Play Again
	gef::Texture* Menu_Text_;
	gef::Texture* Play_Again_Text_;

	
	gef::Texture* button_icon_;

	///Virtual Function created so that other State could inherit from it and so that we could use them in Scene App
	virtual void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_) {};
	virtual void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_) {};
	virtual bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_) {};
	virtual void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_) {};

	///Variable for click sound 
	int32 play_ClickSound_sfx_id = 0;
	
	///Variables for changing volume in Option
	gef::VolumeInfo volumeInfo;

	///Touch ID Variable used by other states
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	/// player variables
	 static Player player_ref_;

	///High Score declaration
	int high_score_ = 500;

	
	///Set Game states
	GAMESTATE game_state;


	///State Bollean
	bool Is_Easy_Mode;
	bool Is_Medium_Mode;
	bool Is_Hard_Mode;

	
	///Enemy Variables
	Enemy enemy_ref_;
	EnemyBullet enemy_bullet_ref_;

	///Bullet Variables
	BulletManager bullet_ref_;

	///Bollean used for bullets to move forward
	bool isMoving_;

	/// Game Time Variable
	float time;

	



};

