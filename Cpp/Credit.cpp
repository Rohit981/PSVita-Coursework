#include "Credit.h"
#include <system/debug_log.h>
#include <load_texture.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <system/debug_log.h>
#include <maths/math_utils.h>

Credit::Credit()
{
	game_state = CREDITS;
}


Credit::~Credit()
{
}

///It Initiates the Credit screen and Back Button sprite and texture and the audio for the credit
void Credit::Init(gef::SpriteRenderer * sprite_render_, gef::Platform & platform_, gef::Font * font_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	Credit_Text = CreateTextureFromPNG("CreditScreen.png", platform_);
	Credit_Background.set_texture(Credit_Text);
	Credit_Background.set_position(gef::Vector4(480, 272, -0.3f));
	Credit_Background.set_height(544);
	Credit_Background.set_width(960);

	BackButton_Text = CreateTextureFromPNG("BackButton.png", platform_);
	BackButton_Background.set_texture(BackButton_Text);
	BackButton_Background.set_position(gef::Vector4(80, 480, -0.3f));
	BackButton_Background.set_height(128);
	BackButton_Background.set_width(128);

	audio_manager_->LoadMusic("MenuMusic.wav", platform_);
	audio_manager_->PlayMusic();
}

///It clean ups the credit and back button texture and unloads all the audio
void Credit::CleanUp(gef::SpriteRenderer * sprite_render_, gef::InputManager * input_manager_, gef::Font * font_, gef::AudioManager * audio_manager_)
{
	delete Credit_Text;
	Credit_Text = NULL;

	delete BackButton_Text;
	BackButton_Text = NULL;

	audio_manager_->UnloadMusic();

}

/// Updates My touchInput and checks every frame that if my touch is touching or not
bool Credit::Update(float frame_time, gef::InputManager * input_manager, gef::Platform & platform_, gef::AudioManager * audio_manager_)
{
	TouchInput(input_manager, audio_manager_);
	return true;
}

///Renders the credit and BackButton sprite 
void Credit::Render(gef::Platform & platform_, gef::Font * font_, gef::SpriteRenderer * sprite_renderer_, gef::AudioManager * audio_manager_)
{
	sprite_renderer_->Begin(false);

	sprite_renderer_->DrawSprite(Credit_Background);
	sprite_renderer_->DrawSprite(BackButton_Background);

	
	sprite_renderer_->End();
}

///It checks if the touch is touching the specific sprite then change the game state to Menu screen
void Credit::TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	active_touch_id_ = -1;
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		/// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);
		if (panel_touches.size() >0)
		{
			int hhh = 0;
		}
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

					
					// we're just going to record the position of the touch
					touch_position_ = touch->position;

					if (IsInside(touch_position_, BackButton_Background))
					{
						audio_manager_->PlaySample(play_ClickSound_sfx_id);
						Is_Medium_Mode = true;
						game_state = MENU;

						//enemy_ref_.enemy.EnemyHealth += 20;

					}
					

				}

				else if (touch->type == gef::TT_RELEASED)
				{
					
					active_touch_id_ = -1;
				}
			}

		}
	}
}

///This checks if the touch is Inside the sprite width and height
bool Credit::IsInside(gef::Vector2 & point, gef::Sprite sprite_)
{
	///test left 
	if (point.x < sprite_.position().x() - sprite_.width() / 2)
	{
		return false;
	}

	///test right
	if (point.x > sprite_.position().x() + sprite_.width() / 2)
	{
		return false;
	}

	///test top
	if (point.y < sprite_.position().y() - sprite_.height() / 2)
	{
		return false;
	}

	///test bottom
	if (point.y > sprite_.position().y() + sprite_.height() / 2)
	{
		return false;
	}

	return true;
}

