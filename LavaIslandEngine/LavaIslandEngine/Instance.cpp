#include "Instance.h"
#include <stdexcept>
#include "Extension.h"
#include "Debug.h"

namespace VK{
Instance::Instance (): Instance(Extension::CreateDefaultExtensions(), nullptr){
}

Instance::Instance (const Extension& extensions,
					const VkAllocationCallbacks* allocator) :
	allocator (allocator){

	VkApplicationInfo appInfo = {};
	FillAppInfo (appInfo);

	VkInstanceCreateInfo createInfo = {};
	FillCreateInfo (createInfo, appInfo, extensions);

	if(vkCreateInstance (&createInfo, allocator, &instance) != VK_SUCCESS){
		LIE::Debug::Error ("failed to create instance!");
	}
	LIE::Debug::Print ("Vulkan Instance created");
}

Instance::~Instance (){
	vkDestroyInstance (instance, allocator);
	LIE::Debug::Print ("Vulkan Instance destroyed");
}

VkInstance & Instance::GetInstance (){
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
