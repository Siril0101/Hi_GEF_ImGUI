#ifndef _STARTER_APP_H
#define _STARTER_APP_H

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
//	For ImGUI:
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "GameObject.h"
#include "Camera.h"
#include "Brick.h"
#include "Ball.h"
#include "Paddle.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class PlatformD3D11;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class InputManager;
	//	For model loading:
	class Scene;
	class Skeleton;
}

class StarterApp : public gef::Application
{
public:
	StarterApp(gef::PlatformD3D11& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

	void AssignFirstMesh(const char* file_name_, gef::Scene** scene_, gef::Mesh** mesh_);
	gef::Mesh* GetFirstMesh(gef::Scene* scene);

	bool CheckCollisionSphereToSphere(gef::MeshInstance mesh_a, gef::MeshInstance mesh_b);
	bool CheckCollisionAABBToAABB(GameObject* go_a, GameObject* go_b);

	gef::Vector4 GetNormal(GameObject* go_a, GameObject* go_b);
	gef::Vector4 GetReflection(gef::Vector4 incident_vec_, gef::Vector4 normal_);

	float DistanceBetweenPoints(gef::Vector4 a_, gef::Vector4 b_);

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();

	//	Imgui functions:
	void SetupImGui();
	void UpdateImGui();
	void DrawImGui();

	//	Breakout level:
	void SetupBricks();
	void SetupPaddleAndBall();
	void ResetLevel();

	void InputHandling(float frame_time_);
	void ProcessControllerInputs(float frame_time_);
	void ProcessTouchInputs(float frame_time_);
	void ProcessKeyboardInputs(float frame_time_);
	void ProcessMouseInputs(float frame_time_);

	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::Font* font_;

	float fps_;

	Camera m_camera;

	//	Model loading:
	class gef::Mesh* mesh_one_;
	gef::Scene* model_scene_one_;
	
	//	Ball/Paddle:
	Ball ball;
	Paddle player_paddle;

	bool is_colliding;

	PrimitiveBuilder* primitive_builder_;

	//	Bricks:
	std::vector<Brick> all_bricks;	

	Brick brick_test;
	GameObject sphere_test;

	//testing imgui:
	ImGuiIO* io;
	int test_num;
	bool test_bool;

	//	Touch input data:
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;
};

#endif // _STARTER_APP_H
