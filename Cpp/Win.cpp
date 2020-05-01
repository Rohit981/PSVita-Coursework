#include "Win.h"
#include <system/debug_log.h>
#include <load_texture.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <input/touch_input_manager.h>
#include <maths/math_utils.h>


Win::Win()
{
	game_state = WIN;
	
}


Win::~Win()
{
}
///It draws the win screen and replay sprite and loads the music
void Win::Init(gef::SpriteRenderer * sprite_render_, gef::Platform & platform_, gef::Font * font_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	Win_Text_ = CreateTextureFromPNG("WinBackground.png", platform_);
	Win_BackGround_.set_texture(Win_Text_);
	Win_BackGround_.set_position(gef::Vector4(480, 272, -0.3f));
	Win_BackGround_.set_height(544);
	Win_BackGround_.set_width(960);

	Play_Again_Text_ = CreateTextureFromPNG("replay.png", platform_);
	Play_Again_BackGround_.set_texture(Play_Again_Text_);
	Play_Again_BackGround_.set_position(gef::Vector4(444, 453, -0.3f));
	Play_Again_BackGround_.set_height(120);
	Play_Again_BackGround_.set_width(256);

	///Loading Music
	audio_manager_->LoadMusic("VictoryMusic.wav", platform_);

	audio_manager_->PlayMusic();

	InitFont(platform_, font_);

}

///Cleans up all texture and unloads all the music
void Win::CleanUp(gef::SpriteRenderer * sprite_render_, gef::InputManager * input_manager_, gef::Font * font_, gef::AudioManager * audio_manager_)
{
	delete Win_Text_;
	Win_Text_ = NULL;

	delete Play_Again_Text_;
	Play_Again_Text_ = NULL;

	audio_manager_->UnloadMusic();
}

/// Updates touchInput and checks every frame that if touch is touching or not
bool Win::Update(float frame_time, gef::InputManager * input_manager, gef::Platform & platform_, gef::AudioManager * audio_manager_)
{
	TouchInput(input_manager,audio_manager_);
	return true;
}

///Renders all the sprite 
void Win::Render(gef::Platform & platform_, gef::Font * font_, gef::SpriteRenderer * sprite_renderer_, gef::AudioManager * audio_manager_)
{
	
	sprite_renderer_->Begin(false);

	sprite_renderer_->DrawSprite(Win_BackGround_);
	sprite_renderer_->DrawSprite(Play_Again_BackGround_);
	font_->RenderText(sprite_renderer_, gef::Vector4(300.0f, 250.0f, -0.9f), 2.0f, 0xff00ff00, gef::TJ_LEFT, "SCORE: %.i", player_ref_.PlayerScore);

	sprite_renderer_->End();
}

///Renders the specific font
void Win::InitFont(gef::Platform & platform_, gef::Font * font_)
{
	font_->Load("comic_sans");
}

///It checks if the touch is touching the specific sprite then change the game state to Mode
void Win::TouchInput(gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
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
					/// we're just going to record the position of the touch
					touch_position_ = touch->position;
				}

				if (IsInside(touch_position_, Play_Again_BackGround_))
				{
					audio_manager_->PlaySample(play_ClickSound_sfx_id);
					game_state = MODE;

				}

				else if (touch->type == gef::TT_RELEASED)
				{					
					/// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}

		}
	}
}

///This checks if the touch is Inside the sprite width and height
bool Win::IsInside(gef::Vector2 & point, gef::Sprite sprite_)
{
	///test Left
	if(point.x < sprite_.position().x() - sprite_.width() / 2)
	{
		return false;
	}

	///test Right
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
