#pragma once
#include "GameObject.h"
class Ball : public GameObject
{
public:
	void Init();
	void SetScreenEdges(float l_, float r_, float u_, float d_) { m_bounds.set_x(l_); m_bounds.set_y(r_); m_bounds.set_z(u_); m_bounds.set_w(d_); }
	bool Update(float dt_);
	void OnCollisionHorizontal();
	void OnCollisionVertical();
private:
	gef::Vector4 m_bounds; //	(l,r,u,d).
};

