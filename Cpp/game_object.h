#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include "graphics/scene.h"
#include "LoadModel.h"
#include "primitive_builder.h"



enum OBJECT_TYPE
{
	NO_TYPE,
	PLAYER,
	ENEMY,
	BULLET,
	ENEMYBULLET,
	GROUND,
	TOPCELLING	
};

enum Collision_Type
{
	NONE = 0,
	PLAYERCOLLISION = 1,
	NEUTRAL = 2,
	ENEMYCOLLISION = 4,
	PLAYERBULLETCOLLISION = 8,
	ENEMYBULLETCOLLISION = 16,
	GROUNDCOLLISION = 32,
	TOPCELLINGCOLLISION = 64
};

class GameObject : public gef::MeshInstance
{
public:
	//Iniitalizing the function for the Updating the body rotation and scale
	void UpdateFromSimulation(const b2Body* body);
	void UpdateFromSimulationBullet(const b2Body* body);
	void UpdateFromSimulationEnemy(const b2Body* body);
	void UpdateFromSimulationGround(const b2Body* body);
	
	//Variable which sets the Gameobject type and Collision type
	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE type() { return type_; }

	inline void set_collision(Collision_Type type) { collison_ = type; }
	inline Collision_Type collisontype_() { return collison_; }


private:
	OBJECT_TYPE type_;
	Collision_Type collison_;
};

class Player : public GameObject
{
public:
	Player();

	void InitPlayer(b2World*World_, gef::Platform & platform_);
	void LoadPlayerModel(gef::Platform &platform_);

	///Variables for Initializing the Player
	b2Body* player_body_;
	b2World* Playerworld_;
	b2BodyDef player_body_def;
	b2Fixture *player_fixture_;


	///Load Model
	gef::MeshInstance mesh_instance_;
	gef::Scene* scene_assets_;
	const char* scene_asset_filename;
	LoadModel PlayerModel;


	///Initializing the variable for the player Health and Player Score
	float Player_health_;
	int PlayerScore;

};

class Enemy : public GameObject
{
public:
	Enemy();
	///Function for drawing the enemy Mesh and Initializing the player
	void LoadEnemy(gef::Platform &platform_);
	void InitEnemy(b2World*World_, gef::Platform &platform_);

	///Function when enemy Collides with Player Bullet
	void OnEnemyCollision(b2World*World_, gef::Platform &platform_);

	///Variable for Initializing the enemy
	b2BodyDef enemy_body_def;
	b2Body* enemy_Body;

	///Loading the enemy Model
	gef::MeshInstance mesh_instance_;
	gef::Scene* scene_assets_;
	const char* scene_asset_filename;
	LoadModel EnemyModel;

	///Inializing the boolean for checking if the enemy has hit and enemyHealth
	bool isHit = false;
	int EnemyHealth = 10;
	
};

class Bullet : public GameObject
{
	public:
		Bullet();
		///Boolean for checking if player bullet is alive or not
		bool bulletBodyAlive = false;
		
};

class EnemyBullet : public GameObject
{
	public:
		EnemyBullet();
		///Function for drawing the Enemy Bullet Mesh and Initializing the Enemy Bullet
		void LoadEnemyBulletModel(gef::Platform& platform);
		void InitEnemyBullet(b2World*World2_, b2Body*Player_);

		///variables for Initializing the Enemy Bullet
		b2Body*body_;
		
		///Loading the Enemy Bullet Variables
		gef::MeshInstance mesh_instance_;
		gef::Scene* scene_assets_;
		const char* scene_asset_filename;
		LoadModel EnemyBulletModel;

		///Bollean for checking if bullet can be shot and if bullet is Alive or not
		bool isShooting = false;
		bool IsEnemyBulletAlive = false;
};

class Ground : public GameObject
{
		public:
			Ground();

			///Function for Drawing the Ground
			void InitGround(b2World*World_, gef::Platform& platform_);

			///ground variables
			b2Body* ground_body_;

			///Load Ground Model
			gef::MeshInstance ground_mesh_instance;
			gef::Mesh* ground_mesh_;
			gef::Scene* ground_scene_assets_;
			const char* ground_scene_asset_filename;
			LoadModel GroundModel;

};

class TopCelling : public GameObject
{
	public:
			TopCelling();

			///Function for Drawing the Top Celling
			void InitCelling(b2World*world_, gef::Platform& platform_);

			///Celling variables
			b2Body* Celling_body_;

			///Load Celling Model Variable
			gef::MeshInstance Celling_mesh_instance;
			gef::Mesh* Celling_mesh_;
			gef::Scene* celling_scene_assets_;
			const char* celling_scene_asset_filename;
			LoadModel GroundModel;
};



#endif // _GAME_OBJECT_H