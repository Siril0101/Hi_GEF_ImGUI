#pragma once
#include "GameObject.h"
#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include "primitive_builder.h"
#include <input/sony_controller_input_manager.h>
#include <input/touch_input_manager.h>
#include "system/debug_log.h"

class Collision
{
public:
	Collision();
	~Collision();

	void CheckCollisionAABB(GameObject* go_a, GameObject* go_b);
};

