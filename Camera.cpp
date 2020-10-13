#include "Camera.h"

Camera::Camera()
{
	//	Default camera settings:
	camera_eye = gef::Vector4(5.0f, 5.0f, 250.0f);
	camera_lookat = gef::Vector4(0.0f, 0.0f, 0.0f);
	camera_up = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov = gef::DegToRad(45.0f);
	near_plane = 0.01f;
	far_plane = 100.f;
}
