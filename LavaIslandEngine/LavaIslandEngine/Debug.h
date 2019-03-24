
#ifndef LIE_DEBUG
#define LIE_DEBUG

#pragma once

#include <string>
#include <vector>
#include "vulkan/vulkan.h"

#ifdef _DEBUG
static const bool IS_DEBUG = true;
#else
static const bool IS_DEBUG = false;
#endif


namespace VK{
class Instance;
}

namespace LIE{

class Debug{
private:
	static bool validationLayerChecked;
	static const std::string filePath;
	static std::string fileName;
	static const std::vector<const char*> validationLayers;
	static VkResult CreateDebugUtilsMessengerEXT (VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugUtilsMessengerEXT * pDebugMessenger);
	static void DestroyDebugUtilsMessengerEXT (VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks * pAllocator);
	static void PrintValidationMessage (VkDebugUtilsMessageTypeFlagsEXT messageType, const std::string& message);
	static VKAPI_ATTR VkBool32 VKAPI_CALL VKDebugCallback (VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData, void * pUserData);
public:

	class ValidationMessengerGuard{
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkAllocationCallbacks* allocator;
	public:
		void Create (VK::Instance& instance, VkAllocationCallbacks* allocator = nullptr);
		void Destroy ();
	};

	static void InitFileName ();
	static void InitVKValidation (VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks* allocator = nullptr);
	static void CheckValidationLayerSupport ();
	static const std::vector<const char*>& GetValidationLayers ();
	static void Print (std::string message);
	static void Error (std::string message, bool hasToBreak = true);
	static void Warning (std::string message);
	static void Break ();

};

}
#endif 

