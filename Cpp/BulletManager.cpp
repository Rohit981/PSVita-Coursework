#include "BulletManager.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <input\sony_controller_input_manager.h>
#include <maths/math_utils.h>


BulletManager::BulletManager()
{
}


void BulletManager::InitBullet(b2World * World2_,b2Body *Player_)
{
	bullets_.push_back(Bullet());
	Bullet &bullet = bullets_.back();

	bullet.bulletBodyAlive = true;

	if (scene_assets_)
	{
		bullet.set_mesh(BulletModel.GetMeshFromSceneAssets(scene_assets_));
		
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}

	b2BodyDef bullet_body_def;
	bullet_body_def.type = b2_dynamicBody;
	bullet_body_def.angle = Player_->GetAngle();

	bullet_body_def.position = b2Vec2(Player_->GetPosition().x + 0.5f, Player_->GetPosition().y);


	body_ = World2_->CreateBody(&bullet_body_def);
	bullet_bodies_.push_back(body_);

	/// create the shape for the player
	b2PolygonShape bullet_shape;
	bullet_shape.SetAsBox(0.1f, 0.1f);

	/// create the fixture
	b2FixtureDef bullet_fixture_def;
	bullet_fixture_def.shape = &bullet_shape;
	bullet_fixture_def.density = 1.0f;

	bullet_fixture_def.filter.categoryBits = Collision_Type::PLAYERBULLETCOLLISION;
	bullet_fixture_def.filter.maskBits = Collision_Type::NEUTRAL| Collision_Type::ENEMYCOLLISION;

	/// create the fixture on the rigid body
	body_->CreateFixture(&bullet_fixture_def);


	/// update visuals from simulation data
	bullet.UpdateFromSimulationBullet(body_);

	body_->SetGravityScale(0.0f);
	body_->SetUserData(&bullet);
}

void BulletManager::LoadBulletModel(gef::Platform & platform_)
{
	const char* scene_asset_filename = "Missile.scn";
	scene_assets_ = BulletModel.LoadSceneAssets(platform_, scene_asset_filename);

}






