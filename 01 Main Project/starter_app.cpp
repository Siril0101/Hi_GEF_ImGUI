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

	mesh_one_(NULL),
	model_scene_one_(NULL),
	io(NULL)
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

	test_num = 0;
	test_bool = false;

	InitFont();
	SetupCamera();
	SetupLights();

	//	Bricks:
	SetupBricks();
	//	Ball and Paddle:
	SetupPaddleAndBall();

	is_colliding = false;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
//		return true;
//}

LRESULT StarterApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
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
	/*delete player_one;
	player_one = NULL;
	delete player_two;
	player_two = NULL;*/

	delete mesh_one_;
	mesh_one_ = NULL;

	delete model_scene_one_;
	model_scene_one_ = NULL;

	if (this->io != NULL)
	{
		delete this->io;
		this->io = NULL;
	}
}

bool StarterApp::Update(float frame_time)
{
	//	Update ImGUI:
	UpdateImGui();

	fps_ = 1.0f / frame_time;

	//	Handle inputs:
	InputHandling(frame_time);

	//	Update GameObjects:
	ball.Update(frame_time);
	player_paddle.Update(frame_time);

	//	Bricks:
	for (int i = 0; i < all_bricks.size(); i++)
	{
		all_bricks[i].Update(frame_time);
	}

	//	Check collisions (ONLY SET AS COLLIDING IF ANY HAVE BEEN HIT):
	is_colliding = false;
	for (int i = 0; i < all_bricks.size(); i++)
	{
		if (CheckCollisionAABBToAABB(&ball, &all_bricks[i]))
		{
			is_colliding = true;
		}
	}
	if (CheckCollisionAABBToAABB(&ball, &player_paddle))
	{
		is_colliding = true;
	}
	if (CheckCollisionAABBToAABB(&ball, &brick_test))
	{
		is_colliding = true;
	}


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

	// draw meshes here
	renderer_3d_->Begin();


	// draw 3d stuff here
	renderer_3d_->DrawMesh(ball);
	renderer_3d_->DrawMesh(player_paddle);

	//	Bricks:
	for (int i = 0; i < all_bricks.size(); i++)
	{
		renderer_3d_->DrawMesh(all_bricks[i]);
	}

	//	TEST BRICK:
	renderer_3d_->DrawMesh(brick_test);
	renderer_3d_->DrawMesh(sphere_test);
	

	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

//	Collision detection (Sphere to Sphere):
bool StarterApp::CheckCollisionSphereToSphere(gef::MeshInstance mesh_a, gef::MeshInstance mesh_b)
{
	gef::Sphere obj1_sphere = mesh_a.mesh()->bounding_sphere();
	gef::Sphere obj2_sphere = mesh_b.mesh()->bounding_sphere();

	//	Spheres transformed:
	gef::Sphere obj1_xform = obj1_sphere.Transform(mesh_a.transform());
	gef::Sphere obj2_xform = obj2_sphere.Transform(mesh_b.transform());

	//	Get distance:
	float dist = DistanceBetweenPoints(obj1_xform.position(), obj2_xform.position());
	//	Total radii of both spheres:
	float total_radii = obj1_xform.radius() + obj2_xform.radius();

	//	Check if distance is within combined radii:

	gef::DebugOut("\n: p1_rad: %.1f, p2 rad: %.1f!", obj1_xform.radius(), obj2_xform.radius());
	if (dist <= total_radii)
	{
		gef::DebugOut("\n--> dist: %.1f, is colliding!", dist);
		return true;
	}
	else
	{
		gef::DebugOut("\n--> dist: %.1f, NOT colliding!", dist);
		return false;
	}
}
//	Collision detection (Axis Aligned Bounding Box):
bool StarterApp::CheckCollisionAABBToAABB(GameObject* go_a, GameObject* go_b)
{
	gef::Aabb obj1_ = go_a->mesh()->aabb(); //mesh_a.mesh()->aabb();
	gef::Aabb obj2_ = go_b->mesh()->aabb(); //mesh_b.mesh()->aabb();

	//	AABBs transformed:
	gef::Aabb obj1_xform = obj1_.Transform(go_a->transform());
	gef::Aabb obj2_xform = obj2_.Transform(go_b->transform());

	//	Check overlap:
	if (
		//	X:
		obj1_xform.max_vtx().x() > obj2_xform.min_vtx().x() &&
		obj1_xform.min_vtx().x() < obj2_xform.max_vtx().x() &&
		//	Y:
		obj1_xform.max_vtx().y() > obj2_xform.min_vtx().y() &&
		obj1_xform.min_vtx().y() < obj2_xform.max_vtx().y() &&
		//	Z:
		obj1_xform.max_vtx().z() > obj2_xform.min_vtx().z() &&
		obj1_xform.min_vtx().z() < obj2_xform.max_vtx().z())
	{
		Ball* go_ball = NULL;
		GameObject* go_brick = NULL;
		go_ball = (Ball*)go_a;
		go_brick = (Brick*)go_b;
		
		if (go_ball != NULL && go_brick != NULL)
		{
			gef::Vector4 norm = GetNormal(go_b, go_a);
			gef::DebugOut("\n--->norm: (%.f,%.f,%.f)", norm.x(), norm.y(), norm.z());
			gef::Vector4 refl = GetReflection(go_ball->GetVelocityAsVec4() , norm);
			gef::DebugOut("\n--->ref: (%.f,%.f,%.f)", refl.x(), refl.y(), refl.z());
			go_ball->SetVelocity(refl.x(), refl.y(), refl.z());
			
		}
		
		return true;
	}
	else
		return false;
}

gef::Vector4 StarterApp::GetNormal(GameObject * go_a, GameObject * go_b)
{
	gef::Vector4 dir = go_b->GetPositionAsVec4() - go_a->GetPositionAsVec4();
	gef::Vector4 ux = go_a->GetRightVec4();
	gef::Vector4 uy = go_a->GetUpVec4();

	float ex = go_a->GetScaleAsVec4().x() / 2;
	float ey = go_a->GetScaleAsVec4().y() / 2;

	float dx = dir.DotProduct(ux);
	if (dx > ex) dx = ex;
	if (dx < -ex) dx = -ex;

	float dy = dir.DotProduct(uy);
	if (dy > ey) dy = ey;
	if (dy < -ey) dy = -ey;

	gef::Vector4 point = go_a->GetPositionAsVec4() + (ux * dx + uy * dy);
	gef::Vector4 normal = go_b->GetPositionAsVec4() - point;
	normal.Normalise();

	return normal;
}

//https://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
gef::Vector4 StarterApp::GetReflection(gef::Vector4 incident_vec_, gef::Vector4 normal_)
{
	gef::Vector4 u = normal_ * (incident_vec_.DotProduct(normal_) / normal_.DotProduct(normal_));
	gef::Vector4 w = incident_vec_ - u;
	return w - u;
}

float StarterApp::DistanceBetweenPoints(gef::Vector4 a_, gef::Vector4 b_)
{
	//	X component:
	float x_sq = (a_.x() - b_.x());
	x_sq = x_sq * x_sq;
	//	Y component:
	float y_sq = (a_.y() - b_.y());
	y_sq = y_sq * y_sq;
	//	Z component:
	float z_sq = (a_.z() - b_.z());
	z_sq = z_sq * z_sq;

	//	Squareroot:
	return std::sqrtf(x_sq + y_sq + z_sq);

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
		gef::Vector4 ball_pos = ball.transform().GetTranslation();
		font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 470.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Ball: %.1f, %.1f, %.1f", ball_pos.x(), ball_pos.y(), ball_pos.z());
		gef::Vector4 ball_vel = ball.GetVelocityAsVec4();
		font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 480.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Ball Vel: %.1f, %.1f, %.1f", ball_vel.x(), ball_vel.y(), ball_vel.z());
		gef::Vector4 pad_pos = player_paddle.transform().GetTranslation();
		font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 490.0f, -0.9f), 0.5f, 0xffffffff, gef::TJ_LEFT, "Pad: %.1f, %.1f, %.1f", pad_pos.x(), pad_pos.y(), pad_pos.z());

		//	Collision text:
		if (is_colliding)
			font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 500.0f, -0.9f), 0.5f, 0xff00ff00, gef::TJ_LEFT, "Colliding: true");
		else
			font_->RenderText(sprite_renderer_, gef::Vector4(20.0f, 500.0f, -0.9f), 0.5f, 0xff0000ff, gef::TJ_LEFT, "Colliding: false");

	}

	DrawImGui();

}

void StarterApp::DrawImGui()
{
	ImGui::Begin("Test #1635");
	{
		static float f = 0.0f;
		float col = 0.f;
		int show = 1;
		ImGui::Text(u8"Hello, world! ");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&col);
		if (ImGui::Button("Test Window")) test_num++;//show ^= 1;
		if (ImGui::Button("Another Window")) test_num++;// show ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("TESTBOOL:", &test_bool);
	}
	ImVec2 v = ImGui::GetWindowSize();  // v = {32, 48} ,   is tool small
	ImGui::Text("%f %f", v.x, v.y);
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
	m_camera.SetCamEye(gef::Vector4(5.0f, 5.0f, 1000.0f));
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
	io = &ImGui::GetIO();

	ImGui_ImplWin32_Init(platform_.hwnd());
	ImGui_ImplDX11_Init(platform_.device(), platform_.device_context());

	ImGui::StyleColorsDark();
}

void StarterApp::SetupBricks()
{
	const gef::Mesh* mesh_cube = primitive_builder_->GetDefaultCubeMesh();

	float x_spacing = 400.0f;
	float y_spacing = 100.0f;
	float x_start_pos = -1000.0f;
	float y_start_pos = 200.0f;

	//	Create a "grid" of bricks:
	for (int row = 0; row < 5; row++)
	{
		for (int column = 0; column < 6; column++)
		{
			Brick new_brick = Brick();
			new_brick.set_mesh(mesh_cube);
			new_brick.Init();
			new_brick.SetPosition(x_start_pos + (x_spacing * column), y_start_pos + (y_spacing * row), -1000.0f);
			new_brick.SetScale(300.0f, 30.0f, 30.0f);

			all_bricks.push_back(new_brick);
		}
	}

	gef::DebugOut("\nCreated %i bricks!", all_bricks.size());


	//TEST BRICK:
	brick_test.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	brick_test.Init();
	brick_test.SetPosition(0.0f, -250.0f, -1000.0f);
	brick_test.SetScale(350.0f, 350.0f, 350.0f);
	brick_test.UpdateTransform();

	//	TEST SPHERE:
	sphere_test.set_mesh(primitive_builder_->GetDefaultSphereMesh());
	sphere_test.Init();
	sphere_test.SetPosition(200.0f, -250.0f, -1000.0f);
	sphere_test.SetScale(300.0f, 300.0f, 350.0f);
	sphere_test.UpdateTransform();
}

void StarterApp::SetupPaddleAndBall()
{
	//	LOAD A MODEL:
	AssignFirstMesh("ball1.scn", &model_scene_one_, &mesh_one_);

	//	Creating overriden sphere:
	gef::Sphere new_bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 50.5f);
	mesh_one_->set_bounding_sphere(new_bounds);

	ball.set_mesh(mesh_one_);
	for (auto& mat: model_scene_one_->materials)
	{
		mat->set_colour(gef::Colour(1.0f, 1.0f, 0.0f).GetABGR());
	}

	//	Initialise GameObjects:
	ball.Init();
	ball.SetPosition(-850.0f, -600.0f, -1000.0f);
	ball.SetVelocity(250.0f, 250.0f, 0.0f);
	ball.SetScale(0.5f, 0.5f, 0.5f);
	ball.SetScreenEdges(-1400.0f, 1400.0f, 800.0f, -800.0f);
	   
	//	Player paddle:
	player_paddle.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	player_paddle.Init();
	player_paddle.SetPosition(0.0f, -700.0f, -1000.0f);
	player_paddle.SetScale(300.0f, 25.0f, 25.0f);
	player_paddle.SetScreenEdges(-1300.0f, 1300.0f, 800.0f, -800.0f);
}

void StarterApp::ResetLevel()
{
	gef::DebugOut("\nReset Level!");
	//ball.SetPosition(-800.0f, -600.0f, -1000.0f);
	ball.SetPosition(sphere_test.GetPositionAsVec4().x(), sphere_test.GetPositionAsVec4().y(), sphere_test.GetPositionAsVec4().z());
	ball.SetVelocity(250.0f, 250.0f, 0.0f);
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

		//	Mouse input
		ProcessMouseInputs(frame_time_);
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
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_UP))
			//camera_eye_.set_z(camera_eye_.z() - 2.0f);
			m_camera.UpdateEyeBy(gef::Vector4(0.0f, 0.0f, -200.0f * frame_time_));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_DOWN))
		m_camera.UpdateEyeBy(gef::Vector4(0.0f, 0.0f, 200.0f * frame_time_));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_LEFT))
			m_camera.UpdateEyeBy(gef::Vector4(-200.0f * frame_time_, 0.0f, 0.0f));
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_RIGHT))
			m_camera.UpdateEyeBy(gef::Vector4(200.0f * frame_time_, 0.0f, 0.0f));

		//	Reset camera:
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_R))
		{
			gef::DebugOut("\nPressed R!");
			SetupCamera();
			ResetLevel();
		}

		//	Move player paddle:
		float accel = 1000.0f;
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_A))
			player_paddle.UpdatePositionXBy(-accel * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_D))
			player_paddle.UpdatePositionXBy(accel * frame_time_);
		//	Move ball:
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_I))
			ball.UpdatePositionYBy(accel * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_K))
			ball.UpdatePositionYBy(-accel * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_J))
			ball.UpdatePositionXBy(-accel * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_L))
			ball.UpdatePositionXBy(accel * frame_time_);

		//	Rotation:
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_Q))
			ball.UpdateRotationYBy(5.0f * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_E))
			ball.UpdateRotationYBy(-5.0f * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_MINUS))
			ball.UpdateScaleBy(-0.1f * frame_time_, -0.1f * frame_time_, -0.1f * frame_time_);
		if (keyboard->IsKeyDown(gef::Keyboard::KeyCode::KC_EQUALS))
			ball.UpdateScaleBy(0.1f * frame_time_, 0.1f * frame_time_, 0.1f * frame_time_);
	}
}

void StarterApp::ProcessMouseInputs(float frame_time_)
{
	//	Set IO for imgui:
	//https://discourse.dearimgui.org/t/input-handling-on-linux/247
	
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
