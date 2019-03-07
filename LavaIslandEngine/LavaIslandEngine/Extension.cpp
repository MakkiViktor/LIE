#include "Extension.h"

#include "GLFW/glfw3.h"
#include "Debug.h"

namespace VK{

void Extension::AddExtensions (const std::vector<const char*>& extensionsToAdd){
	for(auto e : extensionsToAdd){
		if(!HasExtension (e))
			extensions.push_back (e);
	}
	CheckExtensionSupport ();
}

U32 Extension::GetExtensionCount () const{
	return static_cast<U32> (extensions.size ());
}

const std::vector<const char*>& Extension::GetExtensions () const{
	return extensions;
}

std::vector<VkExtensionProperties> Extension::GetSupportedExtensions (){
	U32 supportedExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties (nullptr, &supportedExtensionCount, nullptr);
	std::vector<VkExtensionProperties> supportedExtensions (supportedExtensionCount);
	vkEnumerateInstanceExtensionProperties (nullptr, &supportedExtensionCount, supportedExtensions.data ());
	return supportedExtensions;
}

const Extension Extension::CreateDefaultExtensions (){
	Extension extensions;
	U32 extensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions (&extensionCount);
	std::vector<const char*> extensionList(glfwExtensions, glfwExtensions + extensionCount);
	if(IS_DEBUG){
		extensionList.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensionList.push_back (VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		PrintSupportedExtensions ();
	}
	extensions.AddExtensions (extensionList);
	return extensions;
}

void Extension::PrintSupportedExtensions (){

	LIE::Debug::Print ("\nSupported Extensions:");
	for(const auto& extension : GetSupportedExtensions ()){
		LIE::Debug::Print (extension.extensionName);
	}
}

bool Extension::HasExtension (const char* extension){
	for (auto e : extensions){
		if(strcmp (e, extension) == 0)
			return true;
	}
	return false;
}

bool Extension::CheckExtensionSupport (){
	auto supportedExtensions = GetSupportedExtensions ();
	LIE::Debug::Print ("\nNeeded Extensions:");
	for(auto e : extensions){
		LIE::Debug::Print (e);
		if(![&](){
			for(const auto extension : supportedExtensions){
				if(strcmp (extension.extensionName, e) == 0)
					return true;
			}
			return false;
		   }()){
			LIE::Debug::Error ("\nExtension not supported: \n" + std::string(e));
		}
	}
	LIE::Debug::Print ("\nAll Extensions supported.");
	return true;
}
}