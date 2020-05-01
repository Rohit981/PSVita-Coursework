#pragma once
#include "State.h"
#include <input/input_manager.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include <input/touch_input_manager.h>

class Mode : public State
{
public:
	Mode();
	~Mode();

	///It draws the menu screen, hard button, medium button and easy button sprite and loads the music
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Cleans up all texture and unloads all the music
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	/// Updates touchInput and checks every frame that if touch is touching or not
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Renders all the sprite 
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	///It checks if the touch is touching the specific sprite then change the game state to Level1
	void TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_);

	///This checks if the touch is Inside the sprite width and height
	bool IsInside(gef::Vector2 & point, gef::Sprite sprite_);

	///Sprite variables for the Mode State
	gef::Sprite Easy_Mode_Background;
	gef::Sprite Medium_Mode_Background;
	gef::Sprite Hard_Mode_Background;

	///Texture variables for the Mode State
	gef::Texture* Easy_Mode_Text;
	gef::Texture* Medium_Mode_Text;
	gef::Texture* Hard_Mode_Text;


};

