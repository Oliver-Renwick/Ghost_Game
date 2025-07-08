#pragma once

#include <Windows.h>


struct g_Win
{
	WNDPROC m_wndproc;
	HINSTANCE m_hinstance;
	HWND m_window;

	int m_height;
	int m_width;

	void setup_window();
	void CalcWH();
};