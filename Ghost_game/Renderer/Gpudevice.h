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
		VkDevice		   m_device;
		VkQueue			   m_mainQueue;
		VkQueue			   m_computeQueue;
		VkQueue			   m_transferQueue;
		String_Buffer	   m_stringbuffer;

		size_t ubo_alignment	= 256;
		size_t ssbo_alignment	= 256;
		
		//Extension Function
		PFN_vkCmdBeginRenderingKHR      cmd_begin_rendering;
		PFN_vkCmdEndRenderingKHR        cmd_end_rendering;
		PFN_vkQueueSubmit2KHR           queue_submit2;

		uint32_t vulkan_main_queue_family;
		uint32_t vulkan_compute_queue_family;
		uint32_t vulkan_transfer_queue_family;
		VkDebugUtilsMessengerEXT vulkan_debug_utils_messenger;
		VkPhysicalDeviceProperties physical_device_properties;
		bool dynamic_rendering_extension_present = false;
		bool debug_extension_present = false;
		bool bindless_support = false;


		void Init(DeviceCreation* device_creation);
		bool get_family_queue(VkPhysicalDevice physicalDevice);
	};

	VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messenger_info();
}