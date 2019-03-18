#include "Instance.h"
#include <stdexcept>
#include "Extension.h"
#include "Types.h"

namespace VK{

void Instance::Create (const Extension & extensions, VkAllocationCallbacks* allocator){
	allocator = allocator;

	VkApplicationInfo appInfo = {};
	FillAppInfo (appInfo);

	VkInstanceCreateInfo createInfo = {};
	FillCreateInfo (createInfo, appInfo, extensions);

	if(vkCreateInstance (&createInfo, allocator, &instance) != VK_SUCCESS){
		ERROR ("failed to create instance!");
	}
	PRINT ("Vulkan Instance created");
}

void Instance::Destroy (){
	if(instance != VK_NULL_HANDLE){
		vkDestroyInstance (instance, allocator);
		PRINT ("Vulkan Instance destroyed");
		instance = VK_NULL_HANDLE;
	}
}

const VkInstance & Instance::GetInstance () const{
	return instance;
}

void Instance::FillAppInfo (VkApplicationInfo & appInfo){
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = LIE::APPLICATION_NAME;
	appInfo.applicationVersion = VK_MAKE_VERSION (LIE::VERSION_MAJOR, LIE::VERSION_MINOR, 0);
	appInfo.pEngineName = LIE::ENGINE_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION (LIE::VERSION_MAJOR, LIE::VERSION_MINOR, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
}

void Instance::FillCreateInfo (VkInstanceCreateInfo & createInfo, const VkApplicationInfo& appInfo, const Extension& extensions){
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensions.GetExtensionCount();
	createInfo.ppEnabledExtensionNames = extensions.GetExtensions ().data ();
	if(IS_DEBUG){
		createInfo.enabledLayerCount = static_cast<uint32_t>(LIE::Debug::GetValidationLayers().size());
		createInfo.ppEnabledLayerNames = LIE::Debug::GetValidationLayers ().data ();
	}
	else{
		createInfo.enabledLayerCount = 0;
	}
}
}
