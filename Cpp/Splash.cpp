#include "Splash.h"
#include <system/debug_log.h>
#include <load_texture.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <input/touch_input_manager.h>
#include <maths/math_utils.h>



Splash::Splash()
{
	///first screen game state set
	game_state = INIT;
}


Splash::~Splash()
{
}


///Draws the splash screen sprite and audio for it 
void Splash::Init(gef::SpriteRenderer * sprite_render_, gef::Platform & platform_, gef::Font * font_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	Splash_Text_ = CreateTextureFromPNG("SplashScreen.png", platform_);
	splash_BackGround_.set_texture(Splash_Text_);
	splash_BackGround_.set_position(gef::Vector4(480, 272, -0.3f));
	splash_BackGround_.set_height(544);
	splash_BackGround_.set_width(960);

	audio_manager_->LoadMusic("SplashMusic.wav", platform_);
	audio_manager_->PlayMusic();

	play_ClickSound_sfx_id = audio_manager_->LoadSample("ClickSound.wav", platform_);

	InitFont(platform_, font_);
}

///Cleans Up the splash texture and unloads the audio
void Splash::CleanUp(gef::SpriteRenderer * sprite_render_, gef::InputManager * input_manager_, gef::Font * font_, gef::AudioManager * audio_manager_)
{
	delete Splash_Text_;
	Splash_Text_ = NULL;

	audio_manager_->UnloadMusic();

}

/// Updates My touchInput and checks every frame that if my touch is touching or not
bool Splash::Update(float frame_time, gef::InputManager * input_manager, gef::Platform & platform_, gef::AudioManager * audio_manager_)
{
	
	TouchInput(input_manager, platform_,audio_manager_);
	return true;
}

///Renders the Splash Background and the font for starting the game
void Splash::Render(gef::Platform & platform_, gef::Font * font_, gef::SpriteRenderer * sprite_renderer_, gef::AudioManager * audio_manager_)
{

	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(splash_BackGround_);
	font_->RenderText(sprite_renderer_, gef::Vector4(300.0f, 400.0f, -0.9f), 1.0f, 0xffffff00, gef::TJ_LEFT, "TOUCH THE SCREEN TO PLAY");
	sprite_renderer_->End();
}

/// The font for the text is declared in this function
void Splash::InitFont(gef::Platform & platform_, gef::Font * font_)
{
	
	font_->Load("comic_sans");
}

///It changes the state to Menu state when the touch is released
void Splash::TouchInput(gef::InputManager *input_manager_, gef::Platform& platform_, gef::AudioManager * audio_manager_)
{
	active_touch_id_ = -1;
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		/// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		/// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			/// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				/// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					
					/// we're just going to record the position of the touch
					touch_position_ = touch->position;
				}

				else if (touch->type == gef::TT_RELEASED)
				{
					/// the touch we are tracking has been released
					/// perform any actions that need to happen when a touch is released here
					audio_manager_->PlaySample(play_ClickSound_sfx_id);
					gef::DebugOut("Got Touched \n");

					game_state = MENU;

					/// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}

		}
	}
}
