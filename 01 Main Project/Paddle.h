#pragma once
#include "GameObject.h"
class Paddle : public GameObject
{
public:
	Paddle();
	~Paddle();
	void Init();
	bool Update(float dt_);

	void SetScreenEdges(float l_, float r_, float u_, float d_) { m_bounds.set_x(l_); m_bounds.set_y(r_); m_bounds.set_z(u_); m_bounds.set_w(d_); }

private:
	gef::Vector4 m_bounds;

};

