#include "GameObject.h"

GameObject::GameObject()
{
	m_position.SetValue(0.0f, 0.0f, 0.0f);
	m_velocity.SetValue(0.0f, 0.0f, 0.0f);
	m_rotation.SetValue(0.0f, 0.0f, 0.0f);
	m_scale.SetValue(1.0f, 1.0f, 1.0f);
}

void GameObject::Init()
{
	//	Initialise:
	m_position.SetValue(0.0f, 0.0f, 0.0f);
	m_velocity.SetValue(0.0f, 0.0f, 0.0f);
	m_rotation.SetValue(0.0f, 0.0f, 0.0f);
	m_scale.SetValue(1.0f, 1.0f, 1.0f);
}

bool GameObject::Update(float dt_)
{
	m_position += m_velocity * dt_;

	//	Update transform:
	UpdateTransform();

	return true;
}

void GameObject::Render()
{
}

void GameObject::UpdateTransform()
{
	gef::Matrix44 scale_xform, rot_x_xform, rot_y_xform, rot_z_xform, pos_xform;
	scale_xform.SetIdentity();
	rot_x_xform.SetIdentity();
	rot_y_xform.SetIdentity();
	rot_z_xform.SetIdentity();
	pos_xform.SetIdentity();

	//	Set scale:
	scale_xform.Scale(gef::Vector4(m_scale.GetX(), m_scale.GetY(), m_scale.GetZ(), 1.0f));

	//	Set rotation:
	rot_x_xform.RotationX(m_rotation.GetX());
	rot_y_xform.RotationY(m_rotation.GetY());
	rot_z_xform.RotationZ(m_rotation.GetZ());

	//	Set position:
	pos_xform.SetTranslation(m_position.GetAsVec4());

	//	Apply:
	gef::Matrix44 final_xform;
	final_xform.SetIdentity();
	final_xform = scale_xform * rot_x_xform * rot_y_xform * rot_z_xform * pos_xform;
	set_transform(final_xform);
}
