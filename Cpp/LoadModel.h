#pragma once
#include <graphics/mesh_instance.h>
#include "graphics/scene.h"
class LoadModel
{
public:
	
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

};

