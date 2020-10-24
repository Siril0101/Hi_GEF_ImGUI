#include "starter_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
//	For model:
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>



StarterApp::StarterApp(gef::PlatformD3D11& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	input_manager_(NULL),
	model_scene_one_(NULL)
{
}

void StarterApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);

	// make sure if there is a panel to detect touch input, then activate it
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
		input_manager_->touch_manager()->EnablePanel(0);

	//	Set up ImGUI:
	SetupImGui();

	//	Init Meshes:
	gef::Matrix44 t_mesh;
	t_mesh.SetIdentity();
	//	Mesh 1:
	t_mesh.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.0f));
	m_mesh_1.set_mesh(primitive_builder_->CreateBoxMesh(gef::Vector4(2.0f, 2.0f, 2.0f)));
	m_mesh_1.set_transform(t_mesh);
	//	Mesh 2:
	t_mesh.SetTranslation(gef::Vector4(10.0f, 0.0f, 0.0f));
	m_mesh_2.set_mesh(primitive_builder_->CreateBoxMesh(gef::Vector4(4.0f, 4.0f, 4.0f)));
	m_mesh_2.set_transform(t_mesh);

	InitFont();
	SetupCamera();
	SetupLights();
}



void StarterApp::AssignFirstMesh(const char* file_name_, gef::Scene** scene_, gef::Mesh** mesh_)
{
	gef::Scene* scn = new gef::Scene();
	scn->ReadSceneFromFile(platform_, file_name_);
	scn->CreateMaterials(platform_);

	*mesh_ = GetFirstMesh(scn);
	*scene_ = scn;
}

void StarterApp::CleanUp()
{
	if (primitive_builder_ != NULL)
	{
		delete primitive_builder_;
		primitive_builder_ = NULL;
	}

	CleanUpFont();

	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	//	Release:
	delete model_scene_one_;
	model_scene_one_ = NULL;
}

bool StarterApp::Update(float frame_time)
{
	//	Update ImGUI:
	UpdateImGui();

	fps_ = 1.0f / frame_time;

	//	Handle inputs:
	InputHandling(frame_time);
		   
	return true;
}

void StarterApp::UpdateImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void StarterApp::Render()
{
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	projection_matrix = platform_.PerspectiveProjectionFov(m_camera.GetCamFOV(), (float)platform_.width() / (float)platform_.height(), m_camera.GetCamNearPlane(), m_camera.GetCamFarPlane());
	view_matrix.LookAt(m_camera.GetCamEye(), m_camera.GetCamLookAt(), m_camera.GetCamUp());
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// Draw meshes here:
	renderer_3d_->Begin();

	renderer_3d_->DrawMesh(m_mesh_1);
	renderer_3d_->DrawMesh(m_mesh_2);

	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void StarterApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void StarterApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void StarterApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);

		//	Camz:
		gef::Vector4 cam_eye = m_camera.GetCamEye();
		font_->RenderText(sprite_renderer_, gef::Vector4(350.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Cam_eye: %.1f, %.1f, %.1f", cam_eye.x(), cam_eye.y(), cam_eye.z());
		
		//	GameObjects data:
		gef::Vector4 mesh_pos = m_mesh_1.transform().GetTranslation();
		font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 470.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Mesh 1: %.1f, %.1f, %.1f", mesh_pos.x(), mesh_pos.y(), mesh_pos.z());
		mesh_pos = m_mesh_2.transform().GetTranslation();
		font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 480.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Mesh 2: %.1f, %.1f, %.1f", mesh_pos.x(), mesh_pos.y(), mesh_pos.z());
		
	}

	DrawImGui();

}

void StarterApp::DrawImGui()
{
	ImGui::Begin("Test #1635");
	{
		static float f = 0.0f;
		float* col = 0;
		ImGui::Text(u8"Hello, world! ");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&col);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//	Show demo window if required:
		ImGui::Checkbox("Show Demo Window:", &should_show_demo);
		if (should_show_demo)
			ImGui::ShowDemoWindow(&should_show_demo);

		ImGui::Spacing();
		ImGui::Spacing();
		//	Button:
		if (ImGui::Button("Increment Counter")) button_counter++;
		ImGui::SameLine();
		ImGui::Text(u8"Counter: %i", button_counter);

		//	Show mesh 1 pos:
		gef::Vector4 temp_pos = m_mesh_1.transform().GetTranslation();
		ImGui::Text(u8"Mesh 1: %.1f,%.1f,%.1f", temp_pos.x(), temp_pos.y(), temp_pos.z());
		//	Show mesh 2 pos:
		temp_pos = m_mesh_2.transform().GetTranslation();
		ImGui::Text(u8"Mesh 2: %.1f,%.1f,%.1f", temp_pos.x(), temp_pos.y(), temp_pos.z());
		//	Show cam pos:
		temp_pos = m_camera.GetCamEye();
		ImGui::Text(u8"Cam: %.1f,%.1f,%.1f", temp_pos.x(), temp_pos.y(), temp_pos.z());

		ImGui::Spacing();
		ImGui::Spacing();
		//	Controls text:
		ImGui::Text(u8"CONTROLS: ");
		ImGui::Text(u8"- WASD - camera X and Z axis.");
		ImGui::Text(u8"- R - reset camera.");

	}

	ImVec2 v = ImGui::GetWindowSize();  // v = {32, 48} ,   is tool small
	ImGui::Spacing();
	ImGui::Text("Window size: %f %f", v.x, v.y);
	if (ImGui::GetFrameCount() < 10)
		printf("Frame %d: Size %f %f\n", ImGui::GetFrameCount(), v.x, v.y);

	ImGui::End();


	ImGui::Render();	// Assemble together the draw data:
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void StarterApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void StarterApp::SetupCamera()
{
	// Initialise the camera settings:
	m_camera.SetCamEye(gef::Vector4(5.0f, 5.0f, 100.0f));
	m_camera.SetCamLookAt(gef::Vector4(0.0f, 0.0f, 0.0f));
	m_camera.SetCamUp(gef::Vector4(0.0f, 1.0f, 0.0f));
	m_camera.SetCamFOV(gef::DegToRad(45.0f));
	m_camera.SetCamNearPlane(0.01f);
	m_camera.SetCamFarPlane(5000.f);
}

void StarterApp::SetupImGui()
{
	//	Default GEF only stores Platform, we need PlatformD3DX11 (in order to get hwnd, device, etc.):
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(platform_.hwnd());
	ImGui_ImplDX11_Init(platform_.device(), platform_.device_context());

	ImGui::StyleColorsDark();
}

void StarterApp::InputHandling(float frame_time_)
{
	//	Read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		//	Controller input:
		ProcessControllerInputs(frame_time_);
		ProcessTouchInputs(frame_time_);

		//	Keyboard input
		ProcessKeyboardInputs(frame_time_);
	}
}

void StarterApp::ProcessControllerInputs(float frame_time_)
{
	const gef::SonyControllerInputManager* controller_input = input_manager_->controller_input();
	if (controller_input)
	{
		const gef::SonyController* controller = controller_input->GetController(0);
		if (controller)
		{
			// check for button presses and print out some debug text
			if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
				gef::DebugOut("CROSS pressed\n");
			if (controller->buttons_pressed() & gef_SONY_CTRL_SQUARE)
				gef::DebugOut("SQUARE pressed\n");
			if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE)
				gef::DebugOut("TRIANGLE pressed\n");
			if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
				gef::DebugOut("CIRCLE pressed\n");

			// print out some debug text when left stick is moved
			if (controller->left_stick_x_axis() != 0.0f)
				gef::DebugOut("left_stick_x_axis: %f\n", controller->left_stick_x_axis());
			if (controller->left_stick_y_axis() != 0.0f)
				gef::DebugOut("left_stick_y_axis: %f\n", controller->left_stick_y_axis());

			// print out some debug text when left stick is moved
			if (controller->right_stick_x_axis() != 0.0f)
				gef::DebugOut("right_stick_x_axis: %f\n", controller->right_stick_x_axis());
			if (controller->right_stick_y_axis() != 0.0f)
				gef::DebugOut("right_stick_y_axis: %f\n", controller->right_stick_y_axis());
		}
	}
}

void StarterApp::ProcessTouchInputs(float frame_time_)
{

	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// do any processing for a new touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
					gef::DebugOut("CLICKED!");
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;
				}
			}
		}
	}
}

void StarterApp::ProcessKeyboardInputs(float frame_time_)
{
	gef::Keyboard* keyboard = input_manager_->keyboard();
	if (keyboard)
	{
		//	Camera movement:
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_W))
			m_camera.UpdateEyeBy(gef::Vector4(0.0f, 0.0f, -200.0f * frame_time_));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_S))
		m_camera.UpdateEyeBy(gef::Vector4(0.0f, 0.0f, 200.0f * frame_time_));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_A))
			m_camera.UpdateEyeBy(gef::Vector4(-200.0f * frame_time_, 0.0f, 0.0f));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_D))
			m_camera.UpdateEyeBy(gef::Vector4(200.0f * frame_time_, 0.0f, 0.0f));

		//	Reset camera:
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_R))
		{
			gef::DebugOut("\nPressed R!");
			SetupCamera();
		}
	}
}

gef::Mesh* StarterApp::GetFirstMesh(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = scene->CreateMesh(platform_, scene->mesh_data.front());
	}

	return mesh;
}
