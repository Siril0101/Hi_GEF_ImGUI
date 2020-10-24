#include <platform/d3d11/system/platform_d3d11.h>
#include "starter_app.h"


unsigned int sceLibcHeapSize = 128 * 1024 * 1024;	// Sets up the heap area size as 128MiB.

//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// initialisation
	gef::PlatformD3D11 platform(hInstance, 960, 544, false, true,NULL,WndProc2);

	StarterApp myApp(platform);
	myApp.Run();

	return 0;
}


