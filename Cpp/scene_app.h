#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include <audio\audio_manager.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include "graphics/scene.h"
#include "game_object.h"
#include "BulletManager.h"
#include "LoadModel.h"
#include "State.h"
#include "InGameState.h"
#include "Splash.h"
#include "MenuState.h"
#include "Lose.h"
#include "Mode.h"
#include "Win.h"
#include "Credit.h"
#include "Options.h"
// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}



class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:

	///Function used for changing states
	void ChangeState(GAMESTATE nextState);
	
	///Sprite Render instance
	gef::SpriteRenderer* sprite_renderer_;

	///Font Instance
	gef::Font* font_;
	
	///audio manager instance
	gef::AudioManager*audio_manager_;
	///Input Manager
	gef::InputManager * input_manager_;
	
	///State Changer
	State * currentState;
	GAMESTATE currentGameState;
	
};


#endif // _SCENE_APP_H
