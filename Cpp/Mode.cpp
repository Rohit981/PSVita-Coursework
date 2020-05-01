#include "Mode.h"
#include <system/debug_log.h>
#include <load_texture.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <system/debug_log.h>
#include <maths/math_utils.h>


Mode::Mode()
{

	game_state = MODE;
	Is_Easy_Mode = false;
	Is_Medium_Mode = false;
	Is_Hard_Mode = false;

}


Mode::~Mode()
{
}

///It draws the menu screen, hard button, medium button and easy button sprite and loads the music
void Mode::Init(gef::SpriteRenderer * sprite_render_, gef::Platform & platform_, gef::Font * font_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	Menu_Text_ = CreateTextureFromPNG("Space_PNG.png", platform_);
	menu_BackGround_.set_texture(Menu_Text_);
	menu_BackGround_.set_position(gef::Vector4(480, 272, -0.3f));
	menu_BackGround_.set_height(544);
	menu_BackGround_.set_width(960);

	Medium_Mode_Text = CreateTextureFromPNG("Medium.png", platform_);
	Medium_Mode_Background.set_texture(Medium_Mode_Text);
	Medium_Mode_Background.set_position(gef::Vector4(444, 253, -0.3f));
	Medium_Mode_Background.set_height(126);
	Medium_Mode_Background.set_width(126);

	Hard_Mode_Text = CreateTextureFromPNG("Hard.png", platform_);
	Hard_Mode_Background.set_texture(Hard_Mode_Text);
	Hard_Mode_Background.set_position(gef::Vector4(723, 253, -0.3f));
	Hard_Mode_Background.set_height(126);
	Hard_Mode_Background.set_width(126);

	Easy_Mode_Text = CreateTextureFromPNG("Easy.png", platform_);
	Easy_Mode_Background.set_texture(Easy_Mode_Text);
	Easy_Mode_Background.set_position(gef::Vector4(180, 253, -0.3f));
	Easy_Mode_Background.set_height(126);
	Easy_Mode_Background.set_width(126);

	audio_manager_->LoadMusic("MenuMusic.wav", platform_);
	audio_manager_->PlayMusic();

	
}

///Cleans up all texture and unloads all the music
void Mode::CleanUp(gef::SpriteRenderer * sprite_render_, gef::InputManager * input_manager_, gef::Font * font_, gef::AudioManager * audio_manager_)
{
	delete Menu_Text_;
	Menu_Text_ = NULL;

	delete Medium_Mode_Text;
	Medium_Mode_Text = NULL;

	delete Hard_Mode_Text;
	Hard_Mode_Text = NULL;

	delete Easy_Mode_Text;
	Easy_Mode_Text = NULL;

	audio_manager_->UnloadMusic();
}

/// Updates touchInput and checks every frame that if touch is touching or not
bool Mode::Update(float frame_time, gef::InputManager * input_manager, gef::Platform & platform_, gef::AudioManager * audio_manager_)
{
	TouchInput(input_manager, audio_manager_);
	return true;
}

///Renders all the sprite 
void Mode::Render(gef::Platform & platform_, gef::Font * font_, gef::SpriteRenderer * sprite_renderer_, gef::AudioManager * audio_manager_)
{
	
	sprite_renderer_->Begin(false);

	sprite_renderer_->DrawSprite(menu_BackGround_);
	sprite_renderer_->DrawSprite(Medium_Mode_Background);
	sprite_renderer_->DrawSprite(Hard_Mode_Background);
	sprite_renderer_->DrawSprite(Easy_Mode_Background);

	sprite_renderer_->End();
}

///It checks if the touch is touching the specific sprite then change the game state to Level1
void Mode::TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	active_touch_id_ = -1;
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		///get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);
		if (panel_touches.size() >0)
		{
			int hhh = 0;
		}
		///go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			///if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				///check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					/// we're just going to record the position of the touch
					touch_position_ = touch->position;

					if (IsInside(touch_position_, Medium_Mode_Background))
					{
						audio_manager_->PlaySample(play_ClickSound_sfx_id);
						Is_Medium_Mode = true;
						game_state = LEVEL1;						

					}
					else if (IsInside(touch_position_, Hard_Mode_Background))
					{
						audio_manager_->PlaySample(play_ClickSound_sfx_id);
						Is_Hard_Mode = true;
						game_state = LEVEL1;
					
					}
					else if (IsInside(touch_position_, Easy_Mode_Background))
					{
						audio_manager_->PlaySample(play_ClickSound_sfx_id);
						Is_Easy_Mode = true;
						game_state = LEVEL1;
						
					}

				}	

				else if (touch->type == gef::TT_RELEASED)
				{
					
					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}

		}
	}
}

///This checks if the touch is Inside the sprite width and height
bool Mode::IsInside(gef::Vector2 & point, gef::Sprite sprite_)
{
	///test Left
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
