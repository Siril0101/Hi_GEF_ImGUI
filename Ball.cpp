#include "Ball.h"

void Ball::Init()
{
	GameObject::Init();

	m_bounds.set_x(-1400.0f);
	m_bounds.set_y(1400.0f);
	m_bounds.set_z(800.0f);
	m_bounds.set_w(-800.0f);
}

bool Ball::Update(float dt_)
{
	GameObject::Update(dt_);

	//	Check for Horizontal screen edges:
	if (m_position.GetX() <= m_bounds.x() || m_position.GetX() >= m_bounds.y())
		SetVelocityX(m_velocity.GetX() * -1.0f);
	if (m_position.GetY() <= m_bounds.w() || m_position.GetY() >= m_bounds.z())
		SetVelocityY(m_velocity.GetY() * -1.0f);
	return true;
}

void Ball::OnCollisionHorizontal()
{
	gef::DebugOut("...From: (%.1f, %.1f, %.1f)", m_velocity.GetX(), m_velocity.GetY(), m_velocity.GetZ());

	SetVelocityX(m_velocity.GetX() * -1.0f);
	gef::DebugOut("......new: (%.1f, %.1f, %.1f)", m_velocity.GetX(), m_velocity.GetY(), m_velocity.GetZ());
	GameObject::UpdateTransform();
}

void Ball::OnCollisionVertical()
{
	gef::DebugOut("...From: (%.1f, %.1f, %.1f)", m_velocity.GetX(), m_velocity.GetY(), m_velocity.GetZ());
	SetVelocityY(m_velocity.GetY() * -1.0f);
	gef::DebugOut("......new: (%.1f, %.1f, %.1f)", m_velocity.GetX(), m_velocity.GetY(), m_velocity.GetZ());
	GameObject::UpdateTransform();
}
