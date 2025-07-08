#pragma once

#include "base/mem.h"
#include "base/String.h"
#include "base/Array.h"
#include "base/log.h"
#include "g_win.h"
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_win32.h"

namespace g_graphics
{

	static const char* requested_extensions[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	"VK_KHR_win32_surface",
	};

	static const char* requested_layer[] =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	struct DeviceCreation
	{
		//Window data
		g_Win* m_windows;
		uint32_t num_threads = 1;

		Allocator* m_allocator = nullptr;
		StackAllocator* m_tempallocator = nullptr;
	};

	struct GpuDevice
	{

		StackAllocator*    m_tempallocator;
		Allocator*		   m_allocator;
		g_Win*			   m_windows;
		VkInstance		   m_instance;
		VkSurfaceKHR       m_surface;
		VkPhysicalDevice   m_physicalDevice;
		String_Buffer	   m_stringbuffer;

		bool debug_extension_present = false;


		uint32_t vulkan_main_queue_family;
		VkDebugUtilsMessengerEXT vulkan_debug_utils_messenger;
		VkPhysicalDeviceProperties physical_device_properties;
		void Init(DeviceCreation* device_creation);
		bool get_family_queue(VkPhysicalDevice physicalDevice);
	};

	VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messenger_info();
}