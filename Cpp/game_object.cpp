#include "game_object.h"
#include <system/debug_log.h>

//
// UpdateFromSimulation
// 
// Update the transform of this object from a physics rigid body
//
void GameObject::UpdateFromSimulation(const b2Body* body)
{
	if (body)
	{
		//Setup Object Scale
		gef::Matrix44 object_scale;
		object_scale.Scale(gef::Vector4(1.f, 1.f, 1.f));

		// setup object rotation
		gef::Matrix44 object_rotation;
		object_rotation.RotationZ(body->GetAngle());
		

		// setup the object translation
		gef::Vector4 object_translation(body->GetPosition().x, body->GetPosition().y, 0.0f);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_scale*object_rotation;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}

///Function for Setting the roatation, scale and transform for player bullet
void GameObject::UpdateFromSimulationBullet(const b2Body* body)
{
	if (body)
	{
		gef::Matrix44 object_scale;
		object_scale.Scale(gef::Vector4(0.5f, 0.5f, 0.5f));

		// setup object rotation
		gef::Matrix44 object_rotationBullet;
		object_rotationBullet.RotationZ(body->GetAngle());

		// setup the object translation
		gef::Vector4 object_translation(body->GetPosition().x, body->GetPosition().y, 0.0f);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_scale * object_rotationBullet;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}

///Function for Setting the roatation, scale and transform for enemy bullet
void GameObject::UpdateFromSimulationEnemy(const b2Body * body)
{
	if (body)
	{
		gef::Matrix44 object_scale;
		object_scale.Scale(gef::Vector4(1.f, 1.f, 1.f));
		// setup object rotation
		gef::Matrix44 object_rotation;
		object_rotation.RotationZ(body->GetAngle());
		object_rotation.RotationY(body->GetAngle());

		// setup the object translation
		gef::Vector4 object_translation(body->GetPosition().x, body->GetPosition().y, 0.0f);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_scale * object_rotation;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}

///Function for Setting the roatation, scale and transform for Ground
void GameObject::UpdateFromSimulationGround(const b2Body * body)
{
	if (body)
	{
		gef::Matrix44 object_scale;
		object_scale.Scale(gef::Vector4(8.f, 0.5f, 1.f));
		// setup object rotation
		gef::Matrix44 object_rotation;
		//object_rotation.RotationZ(body->GetAngle());
		object_rotation.RotationY(body->GetAngle());

		// setup the object translation
		gef::Vector4 object_translation(body->GetPosition().x, body->GetPosition().y, 0.0f);

		// build object transformation matrix
		gef::Matrix44 object_transform = object_scale * object_rotation;
		object_transform.SetTranslation(object_translation);
		set_transform(object_transform);
	}
}


Player::Player()
{
	set_type(PLAYER);
	set_collision(PLAYERCOLLISION);

	
}

///Initializing the player Object
void Player::InitPlayer(b2World * World_, gef::Platform & platform_)
{
	
	if (scene_assets_)
	{
		set_mesh(PlayerModel.GetMeshFromSceneAssets(scene_assets_));
		//player_.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}



	// create a physics body for the player

	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(-3.0f, 0.0f);

	player_body_ = World_->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	player_shape.SetAsBox(0.2f, 0.1f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	player_fixture_def.filter.categoryBits = Collision_Type::PLAYERCOLLISION;
	player_fixture_def.filter.maskBits = Collision_Type::NEUTRAL | Collision_Type::ENEMYCOLLISION | Collision_Type::ENEMYBULLETCOLLISION | Collision_Type::GROUNDCOLLISION| Collision_Type::TOPCELLINGCOLLISION;

	// create the fixture on the rigid body
	player_fixture_ = player_body_->CreateFixture(&player_fixture_def);


	// update visuals from simulation data
	UpdateFromSimulation(player_body_);
	player_body_->SetUserData(this);
}

///Load Player Model
void Player::LoadPlayerModel(gef::Platform & platform_)
{
	const char* scene_asset_filename = "Ship2.scn";
	scene_assets_ = PlayerModel.LoadSceneAssets(platform_, scene_asset_filename);
}

Enemy::Enemy()
{
	set_type(ENEMY);
	set_collision(ENEMYCOLLISION);

	

}

///Loads Enemy Model Mesh 
void Enemy::LoadEnemy(gef::Platform &platform_)
{
	scene_asset_filename = "EnemyShip.scn";
	scene_assets_ = EnemyModel.LoadSceneAssets(platform_, scene_asset_filename);
}

///Initialize the Enemy Object
void Enemy::InitEnemy(b2World * World_, gef::Platform &platform_)
{
	isHit = false;
	if (scene_assets_)
	{
		set_mesh(EnemyModel.GetMeshFromSceneAssets(scene_assets_));
		
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}

	// create a physics body for the player
	enemy_body_def.type = b2_dynamicBody;
	enemy_body_def.position = b2Vec2(2.f, 0.f);

	enemy_Body = World_->CreateBody(&enemy_body_def);

	// create the shape for the player
	b2PolygonShape enemy_shape;
	enemy_shape.SetAsBox(0.4f, 0.4f);

	// create the fixture
	b2FixtureDef enemy_fixture_def;
	enemy_fixture_def.shape = &enemy_shape;
	enemy_fixture_def.density = 1.0f;

	enemy_fixture_def.filter.categoryBits = Collision_Type::ENEMYCOLLISION;
	enemy_fixture_def.filter.maskBits = Collision_Type::NEUTRAL | Collision_Type::PLAYERCOLLISION|Collision_Type::PLAYERBULLETCOLLISION;


	// create the fixture on the rigid body
	enemy_Body->CreateFixture(&enemy_fixture_def);

	// update visuals from simulation data
	UpdateFromSimulationEnemy(enemy_Body);
	enemy_Body->SetGravityScale(0.0f);
	enemy_Body->SetUserData(this);

	//ray.InitRay(World_,primitive_builder_, platform_, enemy_Body);
	
}

///Check if the Enemy Bullet is colliding or not then reduce the Enemy Health
void Enemy::OnEnemyCollision(b2World * World_, gef::Platform &platform_)
{
	if (isHit == true)
	{
		EnemyHealth -= 10;
	
		if (EnemyHealth <= 0)
		{
			World_->DestroyBody(enemy_Body);
			InitEnemy(World_, platform_);
			
			
		}
	}
}

///Bullet GameObject
Bullet::Bullet()
{
	set_type(BULLET);
	set_collision(PLAYERBULLETCOLLISION);
}

EnemyBullet::EnemyBullet()
{
	set_type(ENEMYBULLET);
	set_collision(ENEMYBULLETCOLLISION);
}

///Load Model for the Enemy Bullet
void EnemyBullet::LoadEnemyBulletModel(gef::Platform & platform)
{
	const char* scene_asset_filename = "EnemyMissile.scn";
	scene_assets_ = EnemyBulletModel.LoadSceneAssets(platform, scene_asset_filename);
}

///Initialize the EnemyBullet Object
void EnemyBullet::InitEnemyBullet(b2World * World2_, b2Body * enemy_)
{

	if (scene_assets_)
	{
		set_mesh(EnemyBulletModel.GetMeshFromSceneAssets(scene_assets_));
		//player_.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}

	b2BodyDef bullet_body_def;
	bullet_body_def.type = b2_dynamicBody;
	//bullet_body_def.angle = enemy_->GetAngle();

	bullet_body_def.position = b2Vec2(enemy_->GetPosition().x - 0.5f, enemy_->GetPosition().y + 0.1f);


	body_ = World2_->CreateBody(&bullet_body_def);
	

	// create the shape for the player
	b2PolygonShape bullet_shape;
	bullet_shape.SetAsBox(0.2f, 0.2f);

	// create the fixture
	b2FixtureDef bullet_fixture_def;
	bullet_fixture_def.shape = &bullet_shape;
	bullet_fixture_def.density = 1.0f;

	bullet_fixture_def.filter.categoryBits = Collision_Type::ENEMYBULLETCOLLISION;
	bullet_fixture_def.filter.maskBits = Collision_Type::NEUTRAL | Collision_Type::PLAYERCOLLISION;
	// create the fixture on the rigid body
	body_->CreateFixture(&bullet_fixture_def);


	// update visuals from simulation data
	UpdateFromSimulationBullet(body_);

	body_->SetGravityScale(0.0f);
	body_->SetUserData(this);
}

Ground::Ground()
{
	set_type(GROUND);
	set_collision(GROUNDCOLLISION);
}

///Initialize the ground Object
void Ground::InitGround(b2World*world_, gef::Platform& platform_)
{
		ground_scene_asset_filename = "Ground.scn";
		ground_scene_assets_ = GroundModel.LoadSceneAssets(platform_, ground_scene_asset_filename);

		if (ground_scene_assets_)
		{
			set_mesh(GroundModel.GetMeshFromSceneAssets(ground_scene_assets_));
			//set_mesh(primitive_builder_->GetDefaultCubeMesh());
		}
		else
		{
			gef::DebugOut("Scene file %s failed to load\n", ground_scene_asset_filename);
		}

		
		// create a physics body
		b2BodyDef body_def;
		body_def.type = b2_staticBody;
		body_def.position = b2Vec2(0.0f, -2.0f);
	
		ground_body_ = world_->CreateBody(&body_def);
	
		// create the shape
		b2PolygonShape shape;
		shape.SetAsBox(4.0f, 0.5f);
	
		// create the fixture
		b2FixtureDef fixture_def;
		fixture_def.shape = &shape;

		fixture_def.filter.categoryBits = Collision_Type::GROUNDCOLLISION;
		fixture_def.filter.maskBits = Collision_Type::NEUTRAL | Collision_Type::PLAYERCOLLISION;
	
		// create the fixture on the rigid body
		ground_body_->CreateFixture(&fixture_def);
	
		// update visuals from simulation data
		UpdateFromSimulationGround(ground_body_);
	
		ground_body_->SetUserData(this);
}

TopCelling::TopCelling()
{
	set_type(TOPCELLING);
	set_collision(TOPCELLINGCOLLISION);
}

void TopCelling::InitCelling(b2World * world_, gef::Platform & platform_)
{
	celling_scene_asset_filename = "Ground.scn";
	celling_scene_assets_ = GroundModel.LoadSceneAssets(platform_, celling_scene_asset_filename);

	if (celling_scene_assets_)
	{
		set_mesh(GroundModel.GetMeshFromSceneAssets(celling_scene_assets_));
		//set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", celling_scene_asset_filename);
	}


	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(0.0f, 2.5f);

	Celling_body_ = world_->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(4.0f, 0.5f);

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	fixture_def.filter.categoryBits = Collision_Type::TOPCELLINGCOLLISION;
	fixture_def.filter.maskBits = Collision_Type::NEUTRAL | Collision_Type::PLAYERCOLLISION;

	// create the fixture on the rigid body
	Celling_body_->CreateFixture(&fixture_def);

	// update visuals from simulation data
	UpdateFromSimulationGround(Celling_body_);

	Celling_body_->SetUserData(this);
}
