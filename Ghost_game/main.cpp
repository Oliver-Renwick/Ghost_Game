#include <Windows.h>
#include "base/Array.h"
#include "base/log.h"
#include "base/console.h"
#include "base/Filemanager.h"
#include "base/Resource.h"
#include "Renderer/Gpudevice.h"
#include "Renderer/g_win.h"

using namespace g_graphics;
bool running = true;
LRESULT CALLBACK mainWindowCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	setUp_Console();

	
	StackAllocator scratch_allocator;
	scratch_allocator.init(Mega(4));

	size_t marker = scratch_allocator.get_marker();

	//Windows Test
	g_Win win{};
	win.m_hinstance = hInst;
	win.m_wndproc = mainWindowCallBack;
	win.setup_window();

	DeviceCreation dc;
	dc.m_tempallocator = &scratch_allocator;
	dc.m_windows = &win;

	GpuDevice gpu;
	gpu.Init(&dc);


	// Game Loop......
	MSG msg;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}




	scratch_allocator.shutdown();

	system("pause");
}


LRESULT CALLBACK mainWindowCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		running = false;

		OutputDebugStringA("WM_Destroy has been called\n");
		//Handle this as error : recreate Window?
	}break;
	case WM_CLOSE:
	{
		OutputDebugStringA("WM_Close has been called\n");
		//Handle this for user to ask questions like "are you sure to close this app"?
	}break;

	case WM_SIZE:
	{
		OutputDebugStringA("WM_SIZE\n");
	}break;

	case WM_MOVE:
	{
		OutputDebugStringA("The Window has been moved\n");
	}break;

	case WM_PAINT:
	{
	}break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}