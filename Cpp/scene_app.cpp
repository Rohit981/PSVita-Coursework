#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <load_texture.h>
#include <graphics/mesh.h>
#include <input\sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <input\keyboard.h>


SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	currentGameState(NO_STATE),
	currentState(NULL)
{
}

void SceneApp::Init()
{
	///Create sprite render
  	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	///Create font
	font_ = new gef::Font(platform_);

	///create a Input Manager
	input_manager_ = gef::InputManager::Create(platform_);

	///creat a touchmanager and enabling the panel
	gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	touch_input->EnablePanel(0);

	///initialise the audio manager
	audio_manager_ = gef::AudioManager::Create();

	///Initializing the Changestate function
	ChangeState(INIT);
	
}

///Delete the sprite, gameobject and etc  
void SceneApp::CleanUp()
{
	///Initialize to no state because we initialize the cleanup function in our changestate function
	ChangeState(NO_STATE);

}

///Update is called every frame and updates the state and call's the specific update function for the state
bool SceneApp::Update(float frame_time)
{
	///Calling Input manager for all the states
	input_manager_->Update();

	///change the states in Update
	ChangeState(currentState->game_state);
	currentState->Update(frame_time, input_manager_, platform_, audio_manager_);
	

	return true;
}

///Renders the specific states sprites and models
void SceneApp::Render()
{
	//Change state function for Render
	currentState->Render(platform_, font_, sprite_renderer_, audio_manager_);
	
}

///Function that changes the state 
void SceneApp::ChangeState(GAMESTATE nextState)
{
	///A return function that changes the state 
	if (currentGameState == nextState)
		return;

	///bollean initialized for different difficulty
	bool easy = false, medium = false, hard = false;

	///Exit the current state
	if (currentState != NULL) {

		///Get the current difficulty
		easy = currentState->Is_Easy_Mode;
		medium = currentState->Is_Medium_Mode;
		hard = currentState->Is_Hard_Mode;

		///Declaring the cleanup function for states and delete the state after calling the cleanup
		currentState->CleanUp(sprite_renderer_, input_manager_, font_, audio_manager_);
		delete currentState;
		currentState = 0;
	}

	///Swap the game state
	currentGameState = nextState;

	///Enter the new state
	switch (currentGameState) {
	case INIT:
	{
		currentState = new Splash;

	}
	break;
	case MENU:
	{
		currentState = new MenuState;
	}
	break;
	case CREDITS:
	{
		currentState = new Credit;
	}
	break;
	case OPTIONS:
	{
		currentState = new Options;
	}
	break;
	case MODE:
	{
		currentState = new Mode;
	}
	break;
	
	case LEVEL1:
	{
		currentState = new InGameState;

	}
	break;
	case LOSE:
	{
		currentState = new Lose;

	}
	break;
	case WIN:
	{
		currentState = new Win;

	}
	break;
	default:
		break;
	}
	
	///If the state is not Null then call the Initialize function for the state
	if (currentState != NULL) {
		currentState->Is_Easy_Mode = easy;
		currentState->Is_Medium_Mode = medium;
		currentState->Is_Hard_Mode = hard;
		currentState->Init(sprite_renderer_, platform_, font_, input_manager_, audio_manager_);
	}
}




