#ifndef LIE_VK_VERTEX_INCLUDES
#define LIE_VK_VERTEX_INCLUDES
#pragma once

#include "glm/glm.hpp"
#include "vulkan/vulkan.h"

//All vertex type's header file needs to be included here

namespace VK{

enum ShaderLocation{
	position = 0,
	color = 1
};

struct Vertex{
	glm::vec2 position;
	glm::vec3 color;

	inline static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions (){
		std::vector<VkVertexInputBindingDescription> bindingDescriptions (1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof (Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}
	inline static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions (){
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = ShaderLocation::position;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof (Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = ShaderLocation::color;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof (Vertex, color);
		return attributeDescriptions;
	}

};

}
#endif 
