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

#include "Camera.h"

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
	
	void InputHandling(float frame_time_);
	void ProcessControllerInputs(float frame_time_);
	void ProcessTouchInputs(float frame_time_);
	void ProcessKeyboardInputs(float frame_time_);

	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::Font* font_;

	//	Touch input data:
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	float fps_;

	Camera m_camera;

	//	Model loading:
	PrimitiveBuilder* primitive_builder_;
	gef::Scene* model_scene_one_;

	class gef::MeshInstance m_mesh_1;
	class gef::MeshInstance m_mesh_2;
		
	//	Testing imgui window:
	bool should_show_demo = false;
	int button_counter = 0;
};

#endif // _STARTER_APP_H
