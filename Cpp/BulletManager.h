#pragma once
#include "game_object.h"
#include <box2d/Box2D.h>
#include "graphics/scene.h"
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include "LoadModel.h"


//namespace gef
//{
//	class Platform;
//
//}

class BulletManager : public GameObject
{
public:
	BulletManager();

	///Variables used for list of bullet Object and body
	std::list<Bullet> bullets_;
	std::list<b2Body*> bullet_bodies_;
	b2Body*body_;
	Bullet bullet_;


	void InitBullet(b2World*World2_, b2Body*Player_);
	void LoadBulletModel(gef::Platform & platform);

	///Variables used for Loading in Bullet Model
	gef::MeshInstance mesh_instance_;
	gef::Scene* scene_assets_;
	const char* scene_asset_filename;
	LoadModel BulletModel;

	
private:
	
};

