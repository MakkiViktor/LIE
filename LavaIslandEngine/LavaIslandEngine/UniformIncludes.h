#ifndef LIE_VK_UNIFORM_INCLUDES
#define LIE_VK_UNIFORM_INCLUDES
#pragma once

#include "glm/glm.hpp"
#include "vulkan/vulkan.h"
#include "Types.h"

//All uniform type's header file needs to be included here

namespace VK{

enum ShaderBindings{
	mvpBinding = 0
};

struct UniformMVP{
	glm::mat4 mvp;

	static VkDescriptorSetLayoutCreateInfo  GetDescriptorInfo (VkDescriptorSetLayoutBinding& uboLayoutBinding){
		uboLayoutBinding.binding = mvpBinding;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		return layoutInfo;
	}

	static U32 GetBinding(){
		return mvpBinding;
	}
};



}
#endif 
