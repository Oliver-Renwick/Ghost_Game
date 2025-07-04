
#include "Gpudevice.h"

namespace g_graphics
{
	static VkBool32 debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT types,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data) {

		bool trigger_rate = severity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

		if (trigger_rate) {
			std::cout << "Message ID => " << callback_data->pMessageIdName << "  " << callback_data->messageIdNumber << std::endl;
			std::cout << "Message => " << callback_data->pMessage << std::endl << std::endl;
		}

		return VK_FALSE;
	}
	VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messenger_info()
	{
		VkDebugUtilsMessengerCreateInfoEXT create_info{};
		
		create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info.pfnUserCallback = debug_utils_callback;
		create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

		return create_info;
	}

	void GpuDevice::Init(DeviceCreation* device_creation)
	{
		m_tempallocator = device_creation->m_tempallocator;
		m_allocator		= device_creation->m_allocator;
		m_stringbuffer.init(m_allocator, 1024 * 1024);


		//Instance Creation
		VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "Ghost_game", 1, "Ghost_engine", 1, VK_MAKE_API_VERSION(0,1,3,0)};

		VkInstanceCreateInfo instance_info{};
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pApplicationInfo = &appInfo;

		//Debug Layer
		instance_info.enabledLayerCount = ArraySize(requested_layer);
		instance_info.ppEnabledLayerNames = requested_layer;

		instance_info.enabledExtensionCount = ArraySize(requested_extensions);
		instance_info.ppEnabledExtensionNames = requested_extensions;

		const VkDebugUtilsMessengerCreateInfoEXT debug_create_info = create_debug_utils_messenger_info();
		
		instance_info.pNext = &debug_create_info;

		VkResult res = vkCreateInstance(&instance_info, nullptr, &m_instance);


		size_t temp_allocmarker = m_tempallocator->get_marker();

		//Choose extension
		{
			uint32_t extension_count;
			vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
			VkExtensionProperties* ext_props = (VkExtensionProperties*)m_tempallocator->allocate(sizeof(VkExtensionProperties) * extension_count, alignof(VkExtensionProperties));
			vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, ext_props);

			for (int i = 0; i < extension_count; i++) {

				if (strcmp(ext_props[i].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
				{
					debug_extension_present = true;
					continue;
				}
			}

			if (!debug_extension_present) {
				throw std::runtime_error("Debug Extension is not present");
			}
			else {
				PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
				VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = create_debug_utils_messenger_info();

				vkCreateDebugUtilsMessengerEXT(m_instance, &debug_messenger_create_info, nullptr, &vulkan_debug_utils_messenger);
			}
		}

	}
}