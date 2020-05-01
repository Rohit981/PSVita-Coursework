#include "InGameState.h"
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


InGameState::InGameState()
{
	///game state set to level 1 for the game state
	game_state = LEVEL1;
	is_loading = true;
}


InGameState::~InGameState()
{
}


///Function for Initializing Player control Inputs
void InGameState::ControllerInput(gef::InputManager *input_manager_, gef::AudioManager * audio_manager_)
{
	///Checks if there is a Input Manager
	if (input_manager_)
	{
		
		///get controller data
		gef::SonyControllerInputManager* controller_input = input_manager_->controller_input();

		if (controller_input)
		{
			const gef::SonyController* controller = controller_input->GetController(0);
			const gef::Keyboard* keyboard = input_manager_->keyboard();

			if (controller)
			{
				gef::Matrix44 player_transform;


				///moving the object with left stick
				float left_x_axis = controller->left_stick_x_axis();
				float left_y_axis = controller->left_stick_y_axis();

				///Initializing the value of x and y variables for left axis stick
				float x = 0;
				float y = 0;


				x = left_x_axis;
				y = -(left_y_axis);

				///Rotating the object
				float right_x_axis = controller->right_stick_x_axis();

				///get the y axis of the left stick
				float right_y_axis = controller->right_stick_y_axis();

				///Initializing the variables for rotating the Player 
				double  rx, ry, result;
				rx = -(right_x_axis);
				ry = -(right_y_axis);

				///Find the angle when the player rotates
				float xv = cos(player_ref_.player_body_->GetAngle());
				float yv = sin(player_ref_.player_body_->GetAngle());

				///Apply the force to the player so that the player can move forward and backward
				player_ref_.player_body_->ApplyForceToCenter(b2Vec2(x*0.4, y*0.4), true);
				player_ref_.player_body_->SetAngularVelocity(right_x_axis);

				
				///Boost the player in the direction were is facing
				if (controller->buttons_down() & gef_SONY_CTRL_CROSS)
				{

					player_ref_.player_body_->ApplyForce(b2Vec2(xv * 1, yv * 1), b2Vec2(player_ref_.player_body_->GetPosition().x, player_ref_.player_body_->GetPosition().y), true);

				}

				///Shoot Bullet Input
				else if (controller->buttons_pressed() & gef_SONY_CTRL_R2)
				{
					bullet_ref_.InitBullet(world_, player_ref_.player_body_);
					isMoving_ = true;					
					audio_manager_->PlaySample(player_bullet_sfx_id);
				}
			}
		}
	}
}

///Function for Initializing fonts for the UI
void InGameState::InitFont(gef::Platform&platform_, gef::Font*font_)
{
	
	font_->Load("comic_sans");
}

///Functtion for Initializing the Game State Function
void InGameState::Init(gef::SpriteRenderer* sprite_render_, gef::Platform&platform_, gef::Font* font_, gef::InputManager* input_manager_, gef::AudioManager * audio_manager_)
{
	Loading_Text_ = CreateTextureFromPNG("Loading1.png", platform_);
	Loading_Background.set_texture(Loading_Text_);
	Loading_Background.set_position(gef::Vector4(480, 272, -0.3f));
	Loading_Background.set_height(544);
	Loading_Background.set_width(960);
	
	LoadingBarHalf_Text_ = CreateTextureFromPNG("Loading2.png", platform_);
	LoadingBarHalf_Background.set_texture(LoadingBarHalf_Text_);
	LoadingBarHalf_Background.set_position(gef::Vector4(480, 272, -0.3f));
	LoadingBarHalf_Background.set_height(544);
	LoadingBarHalf_Background.set_width(960);


	LoadingBarFull_Text_ = CreateTextureFromPNG("Loading3.png", platform_);
	LoadingBarFull_Background.set_texture(LoadingBarFull_Text_);
	LoadingBarFull_Background.set_position(gef::Vector4(480, 272, -0.3f));
	LoadingBarFull_Background.set_height(544);
	LoadingBarFull_Background.set_width(960);

	

}

///Function for Cleaning Up GameState Models and Sprites
void InGameState::CleanUp(gef::SpriteRenderer* sprite_render_, gef::InputManager* input_manager_, gef::Font* font_, gef::AudioManager * audio_manager_)
{
	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	//Destroying the 3D models
	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete Loading_Text_;
	Loading_Text_ = NULL;

	delete LoadingBarHalf_Text_;
	LoadingBarHalf_Text_ = NULL;

	delete LoadingBarFull_Text_;
	LoadingBarFull_Text_ = NULL;

	//Unload the Music
	audio_manager_->UnloadAllSamples();
	audio_manager_->UnloadMusic();
}

///Function checks if the bollean is true or not and basis on that do things for Update 
bool InGameState::Update(float frame_time, gef::InputManager* input_manager, gef::Platform&platform_, gef::AudioManager * audio_manager_)
{
	///This executes the loading Screen
	if (is_loading)
	{
		Loadtime += 1;
		if (Loadtime == 1)
		{
			InitFirst(platform_, input_manager, audio_manager_);
		}
		else if (Loadtime == 50)
		{
			InitSecond(platform_, input_manager, audio_manager_);
		}
		else if (Loadtime == 155)
		{
			InitThird(platform_, input_manager, audio_manager_);
			is_loading = false;
			Loadtime = 0;
		}

	}

	else
	{
		fps_ = 1.0f / frame_time;

		float32 timeStep = 1.0f / 60.0f;

		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		world_->Step(timeStep, velocityIterations, positionIterations);

		ControllerInput(input_manager, audio_manager_);
	
		time -= frame_time;
	
		PhysicsManager();

		ApplyPlayerForce();

		RayCollision(audio_manager_);

		Collision(platform_, audio_manager_); 
		enemy_ref_.OnEnemyCollision(world_, platform_);

		DeleteEnemyBullets();

		///Delete Enemy Bullets when they are off the screen
		if (enemy_bullet_ref_.IsEnemyBulletAlive == true)
		{
			if (enemy_bullet_ref_.body_->GetPosition().x < -30.f)
			{
					enemy_bullet_ref_.IsEnemyBulletAlive = false;
				
			}

		}

		
		RespawnPlayer(platform_);
		DeleteEnemy();
		checkingPlayerBullet();

		DeletePlayerBullets();


		LosingCondition();
		WiningCondition();
		
		
	}
	return true;
	
}

///Initializes the Background Mesh for the Game
void InGameState::InitBackground(gef::Platform&platform_)
{
	scene_asset_filename = "Background.scn";
	scene_assets_ = BackgroundModel.LoadSceneAssets(platform_, scene_asset_filename);
	if (scene_assets_)
	{
		mesh_instance_.set_mesh(BackgroundModel.GetMeshFromSceneAssets(scene_assets_));
		
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}
}

///Function that delets the font in the game
void InGameState::CleanUpFont(gef::Font* font_)
{
	delete font_;
	font_ = NULL;
}

///Function that Declares the Losing condition for the game
void InGameState::LosingCondition()
{
	if (player_ref_.Player_health_ <= 0)
	{
		///Changes the State to Lose
		game_state = LOSE;
		
	}

}

///Function that declares the Wininig condition for the game
void InGameState::WiningCondition()
{
	//TODO ADD High Score Comparison
	if (time <= 0 && player_ref_.Player_health_ > 0)
	{
		if (player_ref_.PlayerScore > high_score_)
		{
			high_score_ = player_ref_.PlayerScore;
			game_state = WIN;
		}
		else
		{
			
			game_state = LOSE;
		}

	}
}

///Function that draws the HUD for the game
void InGameState::DrawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_)
{
	if (font_)
	{
		// display UI for the Game
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, gef::Vector4(170.0f, 10.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_RIGHT, "HEALTH: %.0f", player_ref_.Player_health_);
		font_->RenderText(sprite_renderer_, gef::Vector4(800.0f, 10.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "SCORE: %.i", player_ref_.PlayerScore);
		font_->RenderText(sprite_renderer_, gef::Vector4(400.0f, 10.0f, -0.9f), 1.0f, 0xffffff00, gef::TJ_LEFT, "TIME: %.0f", time);

	}
}

///Function that Applies a Upward force to Player when gravity is appling downward force
void InGameState::ApplyPlayerForce()
{
	float Mass_Player = player_ref_.player_body_->GetMass();

	float Acceleration_Player = 0.5f;

	player_ref_.player_body_->ApplyForceToCenter(b2Vec2(0, (Mass_Player*Acceleration_Player)), true);
}

///Sets up the Light for the Game State
void InGameState::SetupLights()
{
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

///Delets the Player Bullets which are in a List
void InGameState::DeletePlayerBullets()
{

	/// Iterate through the list of bullet Objects and remove the specific object when bullet is not Alive
	std::list<b2Body*>::list::iterator bullet_body = bullet_ref_.bullet_bodies_.begin();
	for (std::list<Bullet>::iterator bullet_ = bullet_ref_.bullets_.begin(); bullet_ != bullet_ref_.bullets_.end(); )
	{
		if (bullet_->bulletBodyAlive == false ) {

			///Destroy the BUllet Body and remove it from the list
			world_->DestroyBody(*bullet_body);
			bullet_body = bullet_ref_.bullet_bodies_.erase(bullet_body);

			///Remove the Bullet Object
			bullet_ = bullet_ref_.bullets_.erase(bullet_);
		}
		else {

			///If condition is true then increament bullets
			bullet_++;
			bullet_body++;
		}
	}

}

///Moves the Player bullet forward in x Direction by Appling Force
void InGameState::MovePlayerBullets()
{
	for (std::list<b2Body*>::iterator bullet_body_ = bullet_ref_.bullet_bodies_.begin(); bullet_body_ != bullet_ref_.bullet_bodies_.end(); ++bullet_body_)
	{
		float xv = cos(player_ref_.player_body_->GetAngle());
		float yv = sin(player_ref_.player_body_->GetAngle());

		((Bullet*)(*bullet_body_)->GetUserData())->UpdateFromSimulationBullet(*bullet_body_);
		(*bullet_body_)->ApplyForceToCenter(b2Vec2(xv*5.f, yv * 5), true);

	}
}

///Delets the EnemyBullets 
void InGameState::DeleteEnemyBullets()
{
		if ( enemy_bullet_ref_.IsEnemyBulletAlive == false && enemy_bullet_ref_.isShooting == false )
		{

			world_->DestroyBody(enemy_bullet_ref_.body_);
			enemy_bullet_ref_.body_ = NULL;
			
		}

		
		else
		{
			///Moves the Enemy Bullets in negative direction by apllling force in that direction
				float xv = cos(player_ref_.player_body_->GetAngle());
				float yv = sin(player_ref_.player_body_->GetAngle());

				enemy_bullet_ref_.UpdateFromSimulationBullet(enemy_bullet_ref_.body_);
				(enemy_bullet_ref_.body_)->ApplyForceToCenter(b2Vec2(-(2.f), 0.f), true);
				
		}

}

///Spawns the EnemyBullet based on the condition
void InGameState::SpawnEnemyBullet(gef::AudioManager * audio_manager_)
{
	if (enemy_bullet_ref_.isShooting == true && enemy_bullet_ref_.IsEnemyBulletAlive == false)
	{
		
			enemy_bullet_ref_.InitEnemyBullet(world_, enemy_ref_.enemy_Body);
			enemy_bullet_ref_.IsEnemyBulletAlive = true;			
			audio_manager_->PlaySample(enemy_bullet_sfx_id);
			enemy_bullet_ref_.isShooting = false;

		
	}
}

///RayCollision which lets the Enemy to shoot bullet if the ray collides with player 
void InGameState::RayCollision(gef::AudioManager * audio_manager_)
{
	b2RayCastOutput out;
	b2RayCastInput in;
	in.maxFraction = 1;
	in.p2 = b2Vec2(player_ref_.player_body_->GetPosition().x, player_ref_.player_body_->GetPosition().y);
	
	in.p1 = b2Vec2(enemy_ref_.enemy_Body->GetPosition().x, enemy_ref_.enemy_Body->GetPosition().y);

	

	bool did_this_hit = player_ref_.player_fixture_->RayCast(&out, in, 0);

	if (did_this_hit && enemy_bullet_ref_.IsEnemyBulletAlive == false )
	{
		enemy_bullet_ref_.isShooting = true;
	
		SpawnEnemyBullet(audio_manager_);
		 
		
	}
}

///Function that Render the Sprites and Models in GameState
void InGameState::Render(gef::Platform&platform_, gef::Font* font_, gef::SpriteRenderer* sprite_renderer_, gef::AudioManager * audio_manager_)
{
	if (!is_loading)
	{

		float fov = gef::DegToRad(45.0f);
		float aspect_ratio = (float)platform_.width() / (float)platform_.height();
		gef::Matrix44 projection_matrix;
		projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
		renderer_3d_->set_projection_matrix(projection_matrix);

		// view
		gef::Vector4 camera_eye(0.0f, 0.0f, 5.0f);
		gef::Vector4 camera_lookat(0.0f, 0.0f, 0.0f);
		gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
		gef::Matrix44 view_matrix;
		view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
		renderer_3d_->set_view_matrix(view_matrix);

		// draw 3d geometry
		renderer_3d_->Begin();

		//draw ground
		renderer_3d_->DrawMesh(ground_ref_);
		renderer_3d_->set_override_material(NULL);

		//draw Background
		renderer_3d_->DrawMesh(mesh_instance_);

		//draw player
		renderer_3d_->DrawMesh(player_ref_);

		//draw Enemy
		renderer_3d_->DrawMesh(enemy_ref_);

		//draw the Player Bullet
		for (std::list<Bullet>::iterator bullet = bullet_ref_.bullets_.begin(); bullet != bullet_ref_.bullets_.end(); ++bullet)
		{
		

			if (bullet->bulletBodyAlive)
			{
				renderer_3d_->DrawMesh(*bullet);
			}

		
		}

		//draw the enemy Bullet
		if (enemy_bullet_ref_.IsEnemyBulletAlive == true)
		{
				renderer_3d_->DrawMesh(enemy_bullet_ref_);
		}
	
		renderer_3d_->set_override_material(NULL);

		//Stop rendering the 3D geometry
		renderer_3d_->End();

		// start drawing sprites, but don't clear the frame buffer
		sprite_renderer_->Begin(false);

		DrawHUD(sprite_renderer_, font_);

		sprite_renderer_->End();
	}
	else
	{
		sprite_renderer_->Begin(false);

		if (Loadtime >= 1.f && Loadtime <= 49.f)
		{
			//DrawHUD(sprite_renderer_, font_);
			sprite_renderer_->DrawSprite(Loading_Background);

		}
		else if (Loadtime >= 50.f && Loadtime <= 149.f)
		{
			Loading_Background.set_texture(LoadingBarHalf_Text_);
			sprite_renderer_->DrawSprite(Loading_Background);


		}
		else if (Loadtime >= 150.f)
		{
			Loading_Background.set_texture(LoadingBarFull_Text_);
			sprite_renderer_->DrawSprite(Loading_Background);

		}
		sprite_renderer_->End();
	}
}

///Function that checks the Collison between GameObjects
void InGameState::Collision(gef::Platform&platform_, gef::AudioManager * audio_manager_)
{
	b2Contact* contact = world_->GetContactList();
	/// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num<contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			/// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			/// DO COLLISION RESPONSE HERE

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();
			



			Bullet*bullet = NULL;
			Enemy* enemy = NULL;
			EnemyBullet*enemyBullet = NULL;
			Player*player = NULL;
			Ground* ground = NULL;
			TopCelling*celling = NULL;

			//Force gameobject A to be the player to reduce duplicate code
			if (gameObjectB->type() == PLAYER) {
				GameObject* temp = gameObjectA;
				gameObjectA = gameObjectB;
				gameObjectB = temp;
			}			

			if (gameObjectA)
			{
				if (gameObjectA->type() == BULLET)
				{
					bullet = (Bullet*)gameObjectA;
					bullet->bulletBodyAlive = false;
					enemy_ref_.isHit = true;
					player_ref_.PlayerScore += 20;
					audio_manager_->PlaySample(enemy_imapct_sfx_id);
				}
				else if(gameObjectA->type() == ENEMYBULLET)
				{
					if (enemy_bullet_ref_.IsEnemyBulletAlive) 
					{
						enemyBullet = (EnemyBullet*)gameObjectA;
						enemy_bullet_ref_.IsEnemyBulletAlive = false;
						player_ref_.Player_health_ -= 50;
						player_ref_.player_body_->SetLinearVelocity(b2Vec2(0, 0));
						audio_manager_->PlaySample(player_impact_sfx_id);
					}
				}
				else if (gameObjectA->type() == GROUND)
				{
					ground = (Ground*)gameObjectA;
					player_ref_.Player_health_ -= 10;
					player_ref_.player_body_->ApplyForceToCenter(b2Vec2(0.f, 3.f), true);

					gef::DebugOut("Lost Health");
				}
				else if(gameObjectA->type() == TOPCELLING)
				{
					celling = (TopCelling*)gameObjectA;
					
					player_ref_.player_body_->ApplyForceToCenter(b2Vec2(0.f, -3.f), true);

					gef::DebugOut("hit");
				}
				
				if (gameObjectA->type() == ENEMY)
				{
					enemy = (Enemy*)gameObjectA;
					
				}
				else if(gameObjectA->type() == PLAYER)
				{
					player = (Player*)gameObjectA;
					
					
				}
				
			}

			//Check Collision for the GameObject B
			if (gameObjectB)
			{
				if (gameObjectB->type() == BULLET)
				{
					bullet = (Bullet*)gameObjectB;
					bullet->bulletBodyAlive = false;
					enemy_ref_.isHit = true;
					player_ref_.PlayerScore += 20;
					audio_manager_->PlaySample(enemy_imapct_sfx_id);
				}
				else if (gameObjectB->type() == ENEMYBULLET)
				{
					if (enemy_bullet_ref_.IsEnemyBulletAlive) 
					{
						enemyBullet = (EnemyBullet*)gameObjectB;
						enemy_bullet_ref_.IsEnemyBulletAlive = false;
						player_ref_.Player_health_ -= 50;
						player_ref_.player_body_->SetLinearVelocity(b2Vec2(0, 0));
						audio_manager_->PlaySample(player_impact_sfx_id);
					}
				}
				else if (gameObjectB->type() == GROUND)
				{
					ground = (Ground*)gameObjectB;
					player_ref_.Player_health_ -= 10;
					player_ref_.player_body_->ApplyForceToCenter(b2Vec2(0.f, 3.f), true);
					gef::DebugOut("Lost Health");

				}
				else if (gameObjectB->type() == TOPCELLING)
				{
					celling = (TopCelling*)gameObjectB;

					player_ref_.player_body_->ApplyForceToCenter(b2Vec2(0.f, -3.f), true);

					gef::DebugOut("hit");
				}

				if (gameObjectB->type() == ENEMY)
				{
					enemy = (Enemy*)gameObjectB;
					
				}
				else if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)gameObjectB;
					
				}
				
			}

		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

///Load One Section of Init
void InGameState::InitFirst( gef::Platform & platform_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	///Initializing the Render3D
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	/// initialise the physics world
	b2Vec2 gravity(0.0f, -0.5f);
	world_ = new b2World(gravity);


	SetupLights();
	
	///Assinging the time and Player Health based on the Mode
	if (Is_Easy_Mode == true)
	{
		time = 60.f;
		player_ref_.Player_health_ = 200.f;

	}
	else if (Is_Medium_Mode == true)
	{
		time = 90.f;
		player_ref_.Player_health_ = 150.f;

	}
	else if (Is_Hard_Mode == true)
	{
		time = 120.f;
		player_ref_.Player_health_ = 100.f;

	}
}

///Load Second Section of Init
void InGameState::InitSecond( gef::Platform & platform_, gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{

	//Loading Audio
	player_bullet_sfx_id = audio_manager_->LoadSample("PlayerBullet.wav", platform_);
	enemy_bullet_sfx_id = audio_manager_->LoadSample("EnemyBullet.wav", platform_);
	enemy_imapct_sfx_id = audio_manager_->LoadSample("EnemyExplosion.wav", platform_);
	player_impact_sfx_id = audio_manager_->LoadSample("PlayerExplosion.wav", platform_);

	//Loading Music
	audio_manager_->LoadMusic("GameSound.wav", platform_);
	audio_manager_->PlayMusic();
	isMoving_ = false;

	
}

///Load Third Section of Init
void InGameState::InitThird(gef::Platform & platform_ , gef::InputManager * input_manager_, gef::AudioManager * audio_manager_)
{
	player_ref_.PlayerScore = 0;

	player_ref_.LoadPlayerModel(platform_);
	player_ref_.InitPlayer(world_, platform_);

	InitBackground(platform_);
	ground_ref_.InitGround(world_, platform_);

	bullet_ref_.LoadBulletModel(platform_);

	enemy_ref_.LoadEnemy(platform_);
	enemy_ref_.InitEnemy(world_, platform_);
	enemy_bullet_ref_.LoadEnemyBulletModel(platform_);

	celling_ref_.InitCelling(world_, platform_);

}

///Declare all the UpdateSimulation for the different Objects
void InGameState::PhysicsManager()
{
	enemy_ref_.UpdateFromSimulationEnemy(enemy_ref_.enemy_Body);
	enemy_ref_.enemy_Body->ApplyForceToCenter(b2Vec2(-0.5, 0), true);

	player_ref_.UpdateFromSimulation(player_ref_.player_body_);
	ground_ref_.UpdateFromSimulationGround(ground_ref_.ground_body_);
	celling_ref_.UpdateFromSimulationGround(celling_ref_.Celling_body_);
}

///Reinstantiate the player back to it's starting position
void InGameState::RespawnPlayer(gef::Platform & platform_)
{
	
	if (player_ref_.player_body_->GetPosition().x >5.f || player_ref_.player_body_->GetPosition().x < -5.f)
	{
		player_ref_.InitPlayer(world_, platform_);

	}
}

///Delete the Enemy When they go of the screen
void InGameState::DeleteEnemy()
{
	if (enemy_ref_.isHit == false)
	{
		if (enemy_ref_.enemy_Body->GetPosition().x < -10.f)
		{
			enemy_ref_.isHit = true;
		}
	}
}

///Move Player bullet is it's true
void InGameState::checkingPlayerBullet()
{
	if (isMoving_ == true)
	{
		MovePlayerBullets();

	}
}
