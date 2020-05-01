#pragma once
#include "State.h"
#include <input/input_manager.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include <input/touch_input_manager.h>

class Options: public State
{
public:
	Options();
	~Options();
	
	///It draws the control screen sprite and back button sprite and loads the music
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Cleans up all the option texture and back button texture
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	/// Updates My touchInput and checks every frame that if my touch is touching or not
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Renders the Option and BackButton sprite 
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	///It checks if the touch is touching the specific sprite then change the game state to Menu screen
	void TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_);

	///This checks if the touch is Inside the sprite width and height
	bool IsInside(gef::Vector2 & point, gef::Sprite sprite_);

	///Sprite Variable used for the Option Screen
	gef::Sprite Option_Background;
	gef::Sprite BackButton_Background;

	///Texture variable used for the Option Screen
	gef::Texture* Option_Text;
	gef::Texture* BackButton_Text;
};

