#include "g_win.h"
#include "base/log.h"

void g_Win::setup_window()
{
	WNDCLASS wndclass{};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = m_hinstance;
	wndclass.lpfnWndProc = m_wndproc;
	wndclass.lpszClassName = L"GhostClass";

	G_ASSERT(RegisterClass(&wndclass), "Failed to Register Window class");
	
	m_window = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE, wndclass.lpszClassName, L"Ghost Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hinstance, NULL);

	
	G_ASSERT(m_window, "Failed to create Windows");

	CalcWH();

}


void g_Win::CalcWH()
{
	RECT rect;
	if (GetClientRect(m_window, &rect))
	{
		m_width  = rect.right - rect.left;
		m_height = rect.bottom - rect.top;
	}
}



