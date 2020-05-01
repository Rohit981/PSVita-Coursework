#pragma once
#include <input/input_manager.h>
#include <graphics/sprite.h>
#include <system/application.h>
#include "graphics/scene.h"
#include "graphics\font.h"
#include "State.h"
#include "LoadModel.h"
#include "MenuState.h"

class Splash: public State
{
public:
	Splash();
	~Splash();

	///Draws the splash screen sprite and audio for it
	void Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_);

	///Cleans Up the splash texture and unloads the audio
	void CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_);

	/// Updates My touchInput and checks every frame that if my touch is touching or not
	bool Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_);

	///Renders the Splash Background and the font for starting the game
	void Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_);

	/// The font for the text is declared in this function
	void InitFont(gef::Platform&platform_, gef::Font* font_);

	///It changes the state to Menu state when the touch is released
	void TouchInput(gef::InputManager *input_manager_, gef::Platform& platform_, gef::AudioManager * audio_manager_);

	///Sprite Variable for the splash Background
	gef::Sprite splash_BackGround_;

	///Texture variable for the splash Background
	gef::Texture* Splash_Text_;

};

