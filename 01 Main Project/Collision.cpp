#include "Collision.h"



Collision::Collision()
{
}


Collision::~Collision()
{
}

void Collision::CheckCollisionAABB(GameObject * go_a, GameObject * go_b)
{
	gef::Aabb obj1_ = go_a->mesh()->aabb(); //mesh_a.mesh()->aabb();
	gef::Aabb obj2_ = go_b->mesh()->aabb(); //mesh_b.mesh()->aabb();

											//	AABBs transformed:
	gef::Aabb obj1_xform = obj1_.Transform(go_a->transform());
	gef::Aabb obj2_xform = obj2_.Transform(go_b->transform());

	//	Check overlap:
	if (
		//	X:
		obj1_xform.max_vtx().x() > obj2_xform.min_vtx().x() &&
		obj1_xform.min_vtx().x() < obj2_xform.max_vtx().x() &&
		//	Y:
		obj1_xform.max_vtx().y() > obj2_xform.min_vtx().y() &&
		obj1_xform.min_vtx().y() < obj2_xform.max_vtx().y() &&
		//	Z:
		obj1_xform.max_vtx().z() > obj2_xform.min_vtx().z() &&
		obj1_xform.min_vtx().z() < obj2_xform.max_vtx().z())
	{
		Ball* go_ball = NULL;
		GameObject* go_brick = NULL;
		go_ball = (Ball*)go_a;
		go_brick = (Brick*)go_b;

		DIRECTION direction = CheckSide(go_b->GetPositionAsVec4() - go_a->GetPositionAsVec4());
		if (go_ball != NULL)
		{
			if (direction != -1)
			{
				switch (direction)
				{
				case LEFT:
				{
					gef::DebugOut("\n---> Direction: LEFT");
					go_ball->OnCollisionHorizontal();
				}
				break;
				case RIGHT:
				{
					gef::DebugOut("\n---> Direction: RIGHT");
					go_ball->OnCollisionHorizontal();
				}
				break;
				case UP:
				{
					gef::DebugOut("\n---> Direction: UP");
					go_ball->OnCollisionVertical();
				}
				break;
				case DOWN:
				{
					gef::DebugOut("\n---> Direction: DOWN");
					go_ball->OnCollisionVertical();
				}
				break;
				}
			}
			else
				gef::DebugOut("\n---> Direction: RETURNED -1!");
		}

		if (go_brick != NULL)
		{


		}


		return true;
	}
	else
		return false;
}
