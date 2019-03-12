#include "Shader.h"

#include "LogicalDevice.h"
#include <fstream>

namespace VK{
//Shader
Shader::Shader (const ShaderDetails& details, const LogicalDevice& device, const VkAllocationCallbacks* allocator):
device(device), stage(details.stage), allocator(allocator){
	std::vector<char> code = LoadFile (details.fileName);
	VkShaderModuleCreateInfo createInfo = {};
	FillCreateInfo (createInfo, code);
	if(vkCreateShaderModule (device.GetLogicalDevice(), &createInfo, allocator, &shaderModule) != VK_SUCCESS){
		ERROR ("failed to create shader module!");
	}
	PRINT ("Shader module created : " + details.fileName);
}

Shader::Shader (Shader && other):
shaderModule(other.shaderModule), device(other.device), 
stage(other.stage), allocator(other.allocator)
{
	other.shaderModule = VK_NULL_HANDLE;
}

Shader::~Shader (){
	if(shaderModule != VK_NULL_HANDLE){
		vkDestroyShaderModule (device.GetLogicalDevice (), shaderModule, allocator);
		PRINT ("Shader module destroyed");
	}
}

VkShaderStageFlagBits Shader::GetStage () const{
	return stage;
}

const VkShaderModule & Shader::GetModule () const{
	return shaderModule;
}

std::vector<char> Shader::LoadFile (const std::string & fileName){
	std::ifstream file (fileName, std::ios::ate | std::ios::binary);
	if(!file.is_open ()){
		ERROR ("failed to open file: " + fileName);
	}

	size_t fileSize = (size_t)file.tellg ();
	std::vector<char> buffer (fileSize);
	file.seekg (0);
	file.read (buffer.data (), fileSize);
	file.close();
	return buffer;
}

void Shader::FillCreateInfo (VkShaderModuleCreateInfo & createInfo, std::vector<char>& code){
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size ();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data ());
}

//ShaderDetails
ShaderDetails::ShaderDetails (const std::string & fileName, VkShaderStageFlagBits stage):
fileName(fileName), stage(stage){}

}
