#pragma once
#include <input/input_manager.h>
#include <graphics/sprite.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include "State.h"
#include "LoadModel.h"
#include "MenuState.h"

class Win: public State
{
public:
	Win();
	~Win();
	///It draws the win screen and replay sprite and loads the music
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Cleans up all texture and unloads all the music
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	/// Updates touchInput and checks every frame that if touch is touching or not
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Renders all the sprite 
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	///Renders the specific font
	void InitFont(gef::Platform&platform_, gef::Font* font_);

	///It checks if the touch is touching the specific sprite then change the game state to Mode
	void TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_);

	///This checks if the touch is Inside the sprite width and height
	bool IsInside(gef::Vector2 & point, gef::Sprite sprite_);

	///sprite variable for rendering win background
	gef::Sprite Win_BackGround_;

	///texture variable for rendering win background
	gef::Texture* Win_Text_;
	
};

