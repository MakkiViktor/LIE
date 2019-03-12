#ifndef LIE_VK_SHADER
#define LIE_VK_SHADER

#pragma once

#include <string>
#include <vector>
#include "vulkan/vulkan.h"

namespace VK{

class LogicalDevice;

class ShaderDetails{
public:
	std::string fileName;
	VkShaderStageFlagBits stage;
	ShaderDetails (const std::string& fileName, VkShaderStageFlagBits stage);
};

class Shader{
private:
	VkShaderModule shaderModule = VK_NULL_HANDLE;
	VkShaderStageFlagBits stage;
	const LogicalDevice& device;
	const VkAllocationCallbacks* allocator;

	static std::vector<char> LoadFile (const std::string& fileName);
public:
	Shader (const ShaderDetails& details, const LogicalDevice& device, const VkAllocationCallbacks* allocator = nullptr);
	Shader (Shader&& other);
	Shader (Shader&) = delete;
	~Shader ();
	VkShaderStageFlagBits GetStage () const;
	const VkShaderModule& GetModule () const;
protected:
	virtual void FillCreateInfo (VkShaderModuleCreateInfo& createInfo, std::vector<char>& code);

};

}

#endif 
