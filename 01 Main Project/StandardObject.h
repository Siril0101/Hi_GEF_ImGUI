#pragma once
#include <graphics/mesh_instance.h>
#include "system/debug_log.h"
#include "Vec3.h"

class StandardObject
{
public:
	StandardObject();

	//	Setters:

	//	Changing Position (Directly/Incrementing):
	void SetPosition(float new_x_, float new_y_, float new_z_) { m_position.SetValue(new_x_, new_y_, new_z_); }
	void SetPositionX(float new_x_) { m_position.SetX(new_x_); }
	void SetPositionY(float new_y_) { m_position.SetY(new_y_); }
	void SetPositionZ(float new_z_) { m_position.SetZ(new_z_); }
	void UpdatePositionBy(float x_increment_, float y_increment_, float z_increment_) { m_position.UpdateValue(x_increment_, y_increment_, z_increment_); }
	void UpdatePositionXBy(float x_increment_) { m_position.UpdateX(x_increment_); }
	void UpdatePositionYBy(float y_increment_) { m_position.UpdateY(y_increment_); }
	void UpdatePositionZBy(float z_increment_) { m_position.UpdateZ(z_increment_); }

	//	Changing Velocity (Directly/Incrementing):
	void SetVelocity(float new_x_, float new_y_, float new_z_) { m_velocity.SetValue(new_x_, new_y_, new_z_); }
	void SetVelocityX(float new_x_) { m_velocity.SetX(new_x_); }
	void SetVelocityY(float new_y_) { m_velocity.SetY(new_y_); }
	void SetVelocityZ(float new_z_) { m_velocity.SetZ(new_z_); }
	void UpdateVelocityBy(float x_increment_, float y_increment_, float z_increment_) { m_velocity.UpdateValue(x_increment_, y_increment_, z_increment_); }
	void UpdateVelocityXBy(float x_increment_) { m_velocity.UpdateX(x_increment_); }
	void UpdateVelocityYBy(float y_increment_) { m_velocity.UpdateY(y_increment_); }
	void UpdateVelocityZBy(float z_increment_) { m_velocity.UpdateZ(z_increment_); }

	//	Changing rotation (Directly/Incrementing):
	void SetRotation(float new_x_, float new_y_, float new_z_) { m_rotation.SetX(new_x_); m_rotation.SetY(new_y_); m_rotation.SetZ(new_z_); }
	void SetRotationX(float new_x_) { m_rotation.SetX(new_x_); }
	void SetRotationY(float new_y_) { m_rotation.SetY(new_y_); }
	void SetRotationZ(float new_z_) { m_rotation.SetZ(new_z_); }
	void UpdateRotationBy(float x_increment_, float y_increment_, float z_increment_) { m_rotation.UpdateValue(x_increment_, y_increment_, z_increment_); }
	void UpdateRotationXBy(float x_increment_) { m_rotation.UpdateX(x_increment_); }
	void UpdateRotationYBy(float y_increment_) { m_rotation.UpdateY(y_increment_); }
	void UpdateRotationZBy(float z_increment_) { m_rotation.UpdateZ(z_increment_); }

	//	Changing scale:
	void SetScale(float new_x_, float new_y_, float new_z_) { m_scale.SetX(new_x_); m_scale.SetY(new_y_); m_scale.SetZ(new_z_); }
	void SetScaleX(float new_x_) { m_scale.SetX(new_x_); }
	void SetScaleY(float new_y_) { m_scale.SetY(new_y_); }
	void SetScaleZ(float new_z_) { m_scale.SetZ(new_z_); }
	void UpdateScaleBy(float x_increment_, float y_increment_, float z_increment_) { m_scale.UpdateValue(x_increment_, y_increment_, z_increment_); }
	void UpdateScaleXBy(float x_increment_) { m_scale.UpdateX(x_increment_); }
	void UpdateScaleYBy(float y_increment_) { m_scale.UpdateY(y_increment_); }
	void UpdateScaleZBy(float z_increment_) { m_scale.UpdateZ(z_increment_); }

	float ClampTo(float value_, float min_, float max_);

	//	Getters:
	gef::Vector4 GetPositionAsVec4() { return m_position.GetAsVec4(); }
	gef::Vector4 GetVelocityAsVec4() { return m_velocity.GetAsVec4(); }
	gef::Vector4 GetRotationAsVec4() { return m_rotation.GetAsVec4(); }
	gef::Vector4 GetScaleAsVec4() { return m_scale.GetAsVec4(); }
	gef::Vector4 GetUpVec4() { return m_up; }
	gef::Vector4 GetRightVec4() { return m_right; }

protected:

	//	Position:
	Vec3 m_position;
	//	Velocity:
	Vec3 m_velocity;
	//	Rotation:
	Vec3 m_rotation;
	//	Scale:
	Vec3 m_scale;

	// Right and up:
	gef::Vector4 m_up;
	gef::Vector4 m_right;

};

