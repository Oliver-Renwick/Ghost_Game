#pragma once

#include "vulkan/vulkan.h"
#include "base/mem.h"
#include "base/String.h"
#include "base/Array.h"
#include "base/log.h"

namespace g_graphics
{

	static const char* requested_extensions[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	};

	static const char* requested_layer[] =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	struct DeviceCreation
	{
		//Window data

		uint32_t height = 0;
		uint32_t width = 0;
		uint32_t num_threads = 1;

		Allocator* m_allocator = nullptr;
		StackAllocator* m_tempallocator = nullptr;
	};

	struct GpuDevice
	{

		StackAllocator*    m_tempallocator;
		Allocator*		   m_allocator;
		VkInstance		   m_instance;
		VkPhysicalDevice   m_physicalDevice;
		String_Buffer	   m_stringbuffer;

		bool debug_extension_present = false;

		VkDebugUtilsMessengerEXT vulkan_debug_utils_messenger;
		VkPhysicalDeviceProperties physical_device_properties;
		void Init(DeviceCreation* device_creation);
		bool get_family_queue(VkPhysicalDevice physicalDevice);
	};

	VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messenger_info();
}