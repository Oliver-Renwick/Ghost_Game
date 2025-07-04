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

	ResourceManager resource_manager;
	resource_manager.Init(&scratch_allocator);



	scratch_allocator.deallocate(marker);

	resource_manager.shutdown();
	scratch_allocator.shutdown();

	system("pause");
}