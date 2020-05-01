#pragma once
#include "State.h"
#include <input/input_manager.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include <input/touch_input_manager.h>


class MenuState: public State
{
public:
	MenuState();
	~MenuState();

	
	///It draws the menu screen, play button, credit button and setting button sprite and loads the music
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Cleans up all texture and unloads all the music
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	/// Updates touchInput and checks every frame that if touch is touching or not
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Renders all the sprite 
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	///It checks if the touch is touching the specific sprite then change the game state to Mode or credit or options screen
	void TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_);

	///This checks if the touch is Inside the sprite width and height
	bool IsInside(gef::Vector2 & point, gef::Sprite sprite_);
	
	///Sprite variables for the Menu state
	gef::Sprite Play_BackGround_;
	gef::Sprite Credit_BackGround_;
	gef::Sprite Option_BackGround_;

	///Texture variables for the Menu State
	gef::Texture* Play_Text_;
	gef::Texture* Credit_Text_;
	gef::Texture* Option_Text_;


};

