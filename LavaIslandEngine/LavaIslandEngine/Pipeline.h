#ifndef LIE_VK_PIPELINE
#define LIE_VK_PIPELINE

#pragma once

#include <string>
#include <vector>
#include "Shader.h"

namespace VK{

class SwapChain;
class RenderPass;

class Pipeline{
private:
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	const LogicalDevice& device;
	const VkAllocationCallbacks* allocator;

	std::vector<VkShaderStageFlagBits> GetShaderStagesFromShaders (const std::vector<Shader>& shaders);
public:
	Pipeline (const std::vector<ShaderDetails>& shaderDetails, const SwapChain& swapChain, const RenderPass& renderPass, const VkAllocationCallbacks* allocator = nullptr);
	Pipeline (const Pipeline&) = delete;
	const VkPipeline& GetPipeline () const;
	~Pipeline ();
protected:

	virtual void FillVertexInputCreateInfo (VkPipelineVertexInputStateCreateInfo& vertexInputInfo);
	virtual void FillInputAssemblyCreateInfo (VkPipelineInputAssemblyStateCreateInfo& inputAssembly);
	virtual void FillViewPortStateCreateInfo (VkPipelineViewportStateCreateInfo& viewportState, const VkExtent2D& extent);
	virtual void FillShaderStageCreateInfo (VkPipelineShaderStageCreateInfo& fragShaderStageInfo, const Shader& shader);
	virtual void FillRasterizerCreateInfo (VkPipelineRasterizationStateCreateInfo& rasterizer);
	virtual void FillMultisampleCreateInfo (VkPipelineMultisampleStateCreateInfo& multisampling);
	virtual void FillColorBlendAttachmentState (VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	virtual void FillColorBlendStateCreateInfo (VkPipelineColorBlendStateCreateInfo& colorBlending,
												const VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	virtual void FillDynamicStateCreateInfo (VkPipelineDynamicStateCreateInfo& dynamicState);
	virtual void FillPipelineLayoutCreateInfo (VkPipelineLayoutCreateInfo& pipelineLayoutInfo);
};
}

#endif 
