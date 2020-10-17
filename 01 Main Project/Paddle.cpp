#include "Paddle.h"



Paddle::Paddle()
{
}


Paddle::~Paddle()
{
}

void Paddle::Init()
{
	GameObject::Init();

	m_bounds.set_x(-1400.0f);
	m_bounds.set_y(1400.0f);
	m_bounds.set_z(800.0f);
	m_bounds.set_w(-800.0f);
}

bool Paddle::Update(float dt_)
{
	GameObject::Update(dt_);

	//	Check for screen edges:
	m_position.SetX(ClampTo(m_position.GetX(), m_bounds.x(), m_bounds.y()));
	m_position.SetY(ClampTo(m_position.GetY(), m_bounds.w(), m_bounds.z()));

	return true;
}
