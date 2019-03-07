#ifndef LIE_VK_EXTENSION
#define LIE_VK_EXTENSION

#pragma once

#include <vector>
#include "Types.h"
#include "vulkan/vulkan.h"

namespace VK{

class Extension{
	std::vector<const char*> extensions;
public:
	void AddExtensions (const std::vector<const char*>& extensionsToAdd);
	U32 GetExtensionCount () const;
	const std::vector<const char*>& GetExtensions () const;
	static std::vector<VkExtensionProperties> GetSupportedExtensions ();
	static const Extension CreateDefaultExtensions ();
private:
	static void PrintSupportedExtensions ();
	bool HasExtension (const char* extension);
	bool CheckExtensionSupport ();
};
}

#endif 

