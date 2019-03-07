#include "Debug.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "Types.h"
#include "Instance.h"

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
static const bool MSC_VER = true;
#else
static const bool MSC_VER = false;
#endif

namespace LIE{

const std::string Debug::filePath = ".\\DebugFiles\\";
std::string Debug::fileName = " Log.txt";
bool Debug::validationLayerChecked = false;

const std::vector<const char*> Debug::validationLayers = {
"VK_LAYER_LUNARG_standard_validation"
};

VKAPI_ATTR VkBool32 VKAPI_CALL Debug::VKDebugCallback (
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData){
	if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
		Warning ("VK VALIDATION");
		PrintValidationMessage (messageType, pCallbackData->pMessage);
	}
	else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT){
		Error ("VK VALIDATION", false);
		PrintValidationMessage (messageType, pCallbackData->pMessage);
		Break ();
	}
	else{
		Print ("VK VALIDATION");
		PrintValidationMessage (messageType, pCallbackData->pMessage);
	}
	return VK_FALSE;
}

VkResult Debug::CreateDebugUtilsMessengerEXT (VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger){
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkCreateDebugUtilsMessengerEXT");
	if(func != nullptr){
		return func (instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Debug::DestroyDebugUtilsMessengerEXT (VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks* pAllocator){
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkDestroyDebugUtilsMessengerEXT");
	if(func != nullptr){
		func (instance, debugMessenger, pAllocator);
	}
}

void Debug::PrintValidationMessage (VkDebugUtilsMessageTypeFlagsEXT messageType, const std::string& message){
	switch(messageType){
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		Print ("VK Validation, general: " + message);
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		Print ("VK Validation, violated specification: " + message);
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		Print ("VK Validation, Performance: " + message);
	default:
		Error ("VK Validation unknown messageType");
	}
}

void Debug::InitFileName (){
	if(!IS_DEBUG)
		return;
	std::ostringstream oss;
	auto t = std::time (nullptr);
	auto tm = *std::localtime (&t);
	oss << std::put_time (&tm, "%d-%m-%Y %H-%M-%S");
	fileName = oss.str () + fileName;
}

void Debug::InitVKValidation (VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks* allocator){
	if(!IS_DEBUG) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = VKDebugCallback;

	if(CreateDebugUtilsMessengerEXT (instance, &createInfo, allocator, &debugMessenger) != VK_SUCCESS){
		Error ("failed to set up debug messenger!");
	}
}

void Debug::CheckValidationLayerSupport (){
	if(!IS_DEBUG || validationLayerChecked)
		return;
	U32 layerCount;
	vkEnumerateInstanceLayerProperties (&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers (layerCount);
	vkEnumerateInstanceLayerProperties (&layerCount, availableLayers.data ());
	Print ("\nSupported validation layers:");
	for(const char* layerName : validationLayers){
		bool layerFound = false;
		for(const auto& layerProperties : availableLayers){
			if(strcmp (layerName, layerProperties.layerName) == 0){
				layerFound = true;
				break;
			}
		}
		if(!layerFound){
			Warning ("Validation layers not supported");
			Warning (layerName);
		}
		else{
			Print (layerName);
		}
	}
	validationLayerChecked = true;
	Print ("\n");
}

const std::vector<const char*>& Debug::GetValidationLayers (){
	CheckValidationLayerSupport ();
	return validationLayers;
}

void Debug::Print (std::string message){
	if(!IS_DEBUG)
		return;
	std::ofstream file (filePath + fileName, std::ios::app);
	if(!file.is_open ()){
		std::cout << "Failed to open the Debug Log \n";
		return;
	}
	file << message << std::endl;
	file.close ();
}

void Debug::Error (std::string  message, bool hasToBreak){
	if(!IS_DEBUG)
		return;
	std::string m = "ERROR: " + message;
	Print (m);
	if(hasToBreak)
		Break ();
}

void Debug::Warning (std::string message){
	if(!IS_DEBUG)
		return;
	std::string m = "WARNING: " + message;
	Print (m);
}

void Debug::Break (){
	if(MSC_VER && IS_DEBUG)
		DEBUG_BREAK;
}
Debug::ValidationMessengerGuard::ValidationMessengerGuard (VK::Instance& instance, const VkAllocationCallbacks* allocator) :
	instance (instance.GetInstance ()), allocator (allocator){
	if(IS_DEBUG)
		InitVKValidation (this->instance, debugMessenger, this->allocator);
}
Debug::ValidationMessengerGuard::~ValidationMessengerGuard (){
	if(IS_DEBUG)
		DestroyDebugUtilsMessengerEXT (instance, debugMessenger, allocator);
}
}
