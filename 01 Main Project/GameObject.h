#pragma once

#include <graphics/mesh_instance.h>
#include "system/debug_log.h"
#include "Vec3.h"
#include "StandardObject.h"

class GameObject : public gef::MeshInstance, public StandardObject
{
public:
	GameObject();
	void Init();
	bool Update(float dt_);
	void Render();
	void UpdateTransform();

	
private:
	
};

