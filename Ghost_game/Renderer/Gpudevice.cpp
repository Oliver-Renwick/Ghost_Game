
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

	bool GpuDevice::get_family_queue(VkPhysicalDevice physicalDevice)
	{
		uint32_t queuepropsCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuepropsCount, nullptr);
		G_INFO("count of queue props ", queuepropsCount);
		VkQueueFamilyProperties* queue_family_properties = (VkQueueFamilyProperties*)
			m_tempallocator->allocate(sizeof(VkQueueFamilyProperties) * queuepropsCount, alignof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuepropsCount, queue_family_properties);

		uint32_t queue_count = 0;
		VkBool32 is_Supported = VK_FALSE;
		for (uint32_t i = 0; i < queuepropsCount; i++)
		{
			VkQueueFamilyProperties queue_prop = queue_family_properties[i];
			G_INFO("Number of queue in this family  ", i, " is : ", queue_prop.queueCount);
			if (queue_prop.queueCount > 0 && queue_prop.queueFlags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT) )
			{
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface, &is_Supported);
				if (is_Supported)
				{
					vulkan_main_queue_family = i;
					break;
				}
			}
		}
		return is_Supported;

	}


	void GpuDevice::Init(DeviceCreation* device_creation)
	{
		m_tempallocator = device_creation->m_tempallocator;
		m_allocator		= device_creation->m_allocator;
		m_windows       = device_creation->m_windows;
		//m_stringbuffer.init(m_allocator, 1024 * 1024);


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

		VK_CHECK_RESULT(vkCreateInstance(&instance_info, nullptr, &m_instance));


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

		//Windows Surface
		VkWin32SurfaceCreateInfoKHR win32_surface{};
		win32_surface.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		win32_surface.hinstance = m_windows->m_hinstance;
		win32_surface.hwnd = m_windows->m_window;
		
		VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(m_instance, &win32_surface, nullptr, &m_surface));

		//Finiding physical device
		{
			uint32_t physicalDevice_count;
			vkEnumeratePhysicalDevices(m_instance, &physicalDevice_count, nullptr);
			VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)m_tempallocator->allocate(sizeof(VkPhysicalDevice) * physicalDevice_count, alignof(VkPhysicalDevice));
			vkEnumeratePhysicalDevices(m_instance, &physicalDevice_count, physicalDevices);

			VkPhysicalDevice discreteGPU = VK_NULL_HANDLE;
			VkPhysicalDevice integratedGPU = VK_NULL_HANDLE;

			for (int i = 0; i < physicalDevice_count; i++)
			{
				VkPhysicalDevice physicaldev = physicalDevices[i];
				vkGetPhysicalDeviceProperties(physicaldev, &physical_device_properties);


				if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					if (get_family_queue(physicaldev))
					{
						discreteGPU = physicaldev;
						break;
					}

					continue;
				}

				if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				{
					if (get_family_queue(physicaldev))
					{
						integratedGPU = physicaldev;
					}

					continue;
				}

			}

			if (discreteGPU){
				m_physicalDevice = discreteGPU;
			}else if (integratedGPU){
				m_physicalDevice = integratedGPU;
			}else{
				G_ASSERT(false, "Suitable GPU Device not Found");
			}
			G_INFO("Selected Device Name : ", physical_device_properties.deviceName);

		}

		m_tempallocator->deallocate(temp_allocmarker);

		{
			temp_allocmarker = m_tempallocator->get_marker();

			uint32_t extension_count = 0;
			vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extension_count, nullptr);
			VkExtensionProperties* extensions = (VkExtensionProperties*)m_tempallocator->allocate(sizeof(VkExtensionProperties) * extension_count, alignof(VkExtensionProperties));
			vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extension_count, extensions);

			for (uint32_t i = 0; i < extension_count; i++)
			{
				VkExtensionProperties extension = extensions[i];
				if (!strcmp(extension.extensionName, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME))
				{
					dynamic_rendering_extension_present = true;
					continue;
				}
			}

			m_tempallocator->deallocate(temp_allocmarker);
		}


		ubo_alignment = physical_device_properties.limits.minUniformBufferOffsetAlignment;
		ssbo_alignment = physical_device_properties.limits.minStorageBufferOffsetAlignment;
		

		//Descriptor Indexing
		VkPhysicalDeviceDescriptorIndexingFeatures indexing_feature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr };
		VkPhysicalDeviceFeatures2 device_feature2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexing_feature};
		vkGetPhysicalDeviceFeatures2(m_physicalDevice, &device_feature2);

		bindless_support = indexing_feature.descriptorBindingPartiallyBound && indexing_feature.runtimeDescriptorArray;


		{
			temp_allocmarker = m_tempallocator->get_marker();

			//Queue Creation
			uint32_t queuefamilycount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queuefamilycount, nullptr);
			VkQueueFamilyProperties* queuefamily_properties = (VkQueueFamilyProperties*)
				m_tempallocator->allocate(sizeof(VkQueueFamilyProperties) * queuefamilycount, alignof(VkQueueFamilyProperties));
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queuefamilycount, queuefamily_properties);


			uint32_t main_family_index = UINT32_MAX, compute_family_index = UINT32_MAX, transfer_family_index = UINT32_MAX;

			for (uint32_t fam_idx = 0; fam_idx < queuefamilycount; fam_idx++)
			{
				VkQueueFamilyProperties queue_prop = queuefamily_properties[fam_idx];

				if ( main_family_index == UINT32_MAX && 
					(queue_prop.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
				{
					main_family_index = fam_idx;
					G_INFO("Main family index  ", main_family_index);
					continue;
				}

				if (  (queue_prop.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
					 !(queue_prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) && 
					  compute_family_index == UINT32_MAX
					)
				{
					compute_family_index = fam_idx;
					G_INFO("Compute family index  ", compute_family_index);
					continue;
				}

				if (
					(queue_prop.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
					!(queue_prop.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
					!(queue_prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
					transfer_family_index == UINT32_MAX
					)
				{
					transfer_family_index = fam_idx;
					G_INFO("Transfer family index  ", transfer_family_index);
					continue;
				}
			}
			m_tempallocator->deallocate(temp_allocmarker);


			vulkan_main_queue_family = main_family_index;
			vulkan_compute_queue_family = compute_family_index;
			vulkan_transfer_queue_family = transfer_family_index;
		}


		temp_allocmarker = m_tempallocator->get_marker();
		// Device Extension Creation

		Array<const char*> device_extension;
		device_extension.Init(m_tempallocator, 2);
		device_extension.push(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		device_extension.push(VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME);

		if (dynamic_rendering_extension_present)
		{
			device_extension.push(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		}

		//Queue Creation
		uint32_t queue_count = 0;
		uint32_t queue_indexes[] = { vulkan_main_queue_family, vulkan_compute_queue_family, vulkan_transfer_queue_family };
		VkDeviceQueueCreateInfo queue_infos[3] = {};
		const float queue_priority[] = { 1.0f, 1.0f };

		for (int i = 0; i < 3; i++)
		{
			VkDeviceQueueCreateInfo& queue_info = queue_infos[queue_count++];
			queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info.queueCount = 1;
			queue_info.pQueuePriorities = queue_priority;
			queue_info.queueFamilyIndex = queue_indexes[i];
		}


		//Device Feature
		VkPhysicalDeviceFeatures2 device_feature{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
		VkPhysicalDeviceVulkan11Features vulkan_feature11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };

		void* current_pnext = &vulkan_feature11;

		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR };
		
		if (dynamic_rendering_extension_present)
		{
			dynamic_rendering_features.pNext = current_pnext;
			current_pnext = &dynamic_rendering_features;
		}

		if (bindless_support)
		{
			indexing_feature.pNext = current_pnext;
			current_pnext = &indexing_feature;
		}

		device_feature.pNext = current_pnext;
		vkGetPhysicalDeviceFeatures2(m_physicalDevice, &device_feature);

		G_ASSERT(vulkan_feature11.shaderDrawParameters == VK_TRUE, "Shader Draw Parameter is not available");



		VkDeviceCreateInfo device_info{};
		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.queueCreateInfoCount = queue_count;
		device_info.pQueueCreateInfos = queue_infos;
		device_info.enabledExtensionCount = device_extension.m_size;
		device_info.ppEnabledExtensionNames = device_extension.m_data;
		device_info.pNext = &device_feature;


		VK_CHECK_RESULT(vkCreateDevice(m_physicalDevice, &device_info, nullptr, &m_device));


		if (dynamic_rendering_extension_present)
		{
			cmd_begin_rendering = (PFN_vkCmdBeginRenderingKHR)vkGetDeviceProcAddr(m_device, "vkCmdBeginRenderingKHR");
			cmd_end_rendering = (PFN_vkCmdEndRenderingKHR)vkGetDeviceProcAddr(m_device, "vkCmdEndRenderingKHR");
		}

		//Get Queue
		vkGetDeviceQueue(m_device, vulkan_main_queue_family,        0, &m_mainQueue);
		vkGetDeviceQueue(m_device, vulkan_compute_queue_family,     0, &m_computeQueue);
		vkGetDeviceQueue(m_device, vulkan_transfer_queue_family,    0, &m_transferQueue);

		m_tempallocator->deallocate(temp_allocmarker);
	}
}