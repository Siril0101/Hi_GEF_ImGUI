#pragma once
#include "maths/vector4.h"
#include "maths/math_utils.h"

class Camera
{
public:
	Camera();

	//	Setters:
	//	Camera eye:
	void SetCamEye(gef::Vector4 new_eye_) { camera_eye = new_eye_; }
	void UpdateEyeBy(gef::Vector4 increment_) { camera_eye += increment_; }
	//	Camera Lookat:
	void SetCamLookAt(gef::Vector4 new_lookat_) { camera_lookat = new_lookat_; }
	void UpdateLookAtBy(gef::Vector4 increment_) { camera_lookat += increment_; }
	void SetCamUp(gef::Vector4 new_up_) { camera_up = new_up_; }
	void SetCamFOV(float new_fov_) { camera_fov = new_fov_; }
	void SetCamNearPlane(float new_near_plane_) { near_plane = new_near_plane_; }
	void SetCamFarPlane(float new_far_plane_) { far_plane = new_far_plane_; }

	//	Getters:
	gef::Vector4 GetCamEye() { return camera_eye; }
	gef::Vector4 GetCamLookAt() { return camera_lookat; }
	gef::Vector4 GetCamUp() { return camera_up; }
	float GetCamFOV() { return camera_fov; }
	float GetCamNearPlane() { return near_plane; }
	float GetCamFarPlane() { return far_plane; }

private:
	gef::Vector4 camera_eye;
	gef::Vector4 camera_lookat;
	gef::Vector4 camera_up;
	float camera_fov;
	float near_plane;
	float far_plane;
};

