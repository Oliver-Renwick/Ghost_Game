#include <Windows.h>
#include "base/Array.h"
#include "base/log.h"
#include "base/console.h"
#include "base/Filemanager.h"
#include "base/Resource.h"
#include "Renderer/Gpudevice.h"

using namespace g_graphics;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	setUp_Console();

	
	StackAllocator scratch_allocator;
	scratch_allocator.init(Mega(4));

	size_t marker = scratch_allocator.get_marker();

	//Graphics Test
	DeviceCreation dc;
	dc.height = 500;
	dc.width = 500;
	dc.m_tempallocator = &scratch_allocator;
	dc.num_threads = 8;

	GpuDevice gpu;
	gpu.Init(&dc);


	scratch_allocator.shutdown();

	system("pause");
}