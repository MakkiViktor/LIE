#ifndef LIE_VK_PIPELINE
#define LIE_VK_PIPELINE
#pragma once

#include <string>
#include <vector>
#include "Shader.h"
#include "VertexIncludes.h"
#include "UniformIncludes.h"

namespace VK{

class SwapChain;
class RenderPass;

class Pipeline{
private:
	VkPipeline graphicsPipeline;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkDevice device;
	VkAllocationCallbacks* allocator;

	std::vector<VkShaderStageFlagBits> GetShaderStagesFromShaders (const std::vector<Shader>& shaders);

	virtual void FillInputAssemblyCreateInfo (VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
	virtual void FillViewPortStateCreateInfo (VkPipelineViewportStateCreateInfo& viewportState, const VkExtent2D& extent, VkViewport& viewport, VkRect2D& scissor);
	virtual void FillShaderStageCreateInfo (VkPipelineShaderStageCreateInfo& fragShaderStageInfo, const Shader& shader);
	virtual void FillRasterizerCreateInfo (VkPipelineRasterizationStateCreateInfo& rasterizer);
	virtual void FillMultisampleCreateInfo (VkPipelineMultisampleStateCreateInfo& multisampling);
	virtual void FillColorBlendAttachmentState (VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	virtual void FillColorBlendStateCreateInfo (VkPipelineColorBlendStateCreateInfo& colorBlending,
												const VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	virtual void FillDynamicStateCreateInfo (VkPipelineDynamicStateCreateInfo& dynamicState);
	virtual void FillPipelineLayoutCreateInfo (VkPipelineLayoutCreateInfo& pipelineLayoutInfo, const VkDescriptorSetLayout& descriptorSetLayout);

	void Create (const std::vector<ShaderDetails>& shaderDetails,
				 std::vector<VkVertexInputBindingDescription> bindingDescriptions,
				 std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
				 VkDescriptorSetLayoutCreateInfo uniformLayoutInfo,
				 const SwapChain& swapChain,
				 const RenderPass& renderPass,
				 VkAllocationCallbacks* allocator);
public:
	void Destroy ();

	const VkPipeline& GetPipeline () const;
	VkDescriptorSetLayout GetDescriptorSetLayout () const;
	VkPipelineLayout GetPipelineLayout () const;

	template<class VERTEX, class UNIFORM>
	void Create (const std::vector<ShaderDetails>& shaderDetails,
				 const SwapChain& swapChain,
				 const RenderPass& renderPass,
				 VkAllocationCallbacks* allocator = nullptr){
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		Create (shaderDetails,
				VERTEX::GetBindingDescriptions (),
				VERTEX::GetAttributeDescriptions (),
				UNIFORM::GetDescriptorInfo (uboLayoutBinding),
				swapChain,
				renderPass,
				allocator);
	}
};
}

#endif 
