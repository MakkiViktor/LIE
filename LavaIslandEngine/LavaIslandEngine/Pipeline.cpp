#include "Pipeline.h"
#include "SwapChain.h"
#include "Types.h"
#include "RenderPass.h"

namespace VK{

const VkPipeline& Pipeline::GetPipeline () const{
	return graphicsPipeline;
}

VkDescriptorSetLayout Pipeline::GetDescriptorSetLayout () const{
	return descriptorSetLayout;
}

VkPipelineLayout Pipeline::GetPipelineLayout () const{
	return pipelineLayout;
}

void Pipeline::Create (const std::vector<ShaderDetails>& shaderDetails,
					   std::vector<VkVertexInputBindingDescription> bindingDescriptions,
					   std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
					   VkDescriptorSetLayoutCreateInfo uniformLayoutInfo,
					   const SwapChain& swapChain,
					   const RenderPass& renderPass,
					   VkAllocationCallbacks* allocator){
	allocator = allocator;
	device = swapChain.GetLogicalDevice ();

	std::vector<Shader> shaders;
	std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos;

	for(const ShaderDetails& shaderDetail : shaderDetails){
		shaders.push_back (Shader (shaderDetail, device, allocator));
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
		FillShaderStageCreateInfo (fragShaderStageInfo, shaders.back ());
		shaderCreateInfos.push_back (fragShaderStageInfo);
	}

	if(vkCreateDescriptorSetLayout (device, &uniformLayoutInfo, allocator, &descriptorSetLayout) != VK_SUCCESS){
		ERROR("failed to create descriptor set layout!");
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	VkRect2D scissor = {};
	VkViewport viewport = {};
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<U32>(bindingDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data ();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<U32>(attributeDescriptions.size ());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

	FillInputAssemblyCreateInfo (inputAssembly);
	FillViewPortStateCreateInfo (viewportState, swapChain.GetExtent (), viewport, scissor);
	FillMultisampleCreateInfo (multisampling);
	FillColorBlendAttachmentState (colorBlendAttachment);
	FillDynamicStateCreateInfo (dynamicState);
	FillColorBlendStateCreateInfo (colorBlending, colorBlendAttachment);
	FillRasterizerCreateInfo (rasterizer);
	FillPipelineLayoutCreateInfo (pipelineLayoutInfo, descriptorSetLayout);

	if(vkCreatePipelineLayout (device, &pipelineLayoutInfo, allocator, &pipelineLayout) != VK_SUCCESS){
		ERROR ("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<U32> (shaderCreateInfos.size ());
	pipelineInfo.pStages = shaderCreateInfos.data ();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass.GetRenderPass ();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;

	if(vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipelineInfo, allocator, &graphicsPipeline) != VK_SUCCESS){
		ERROR ("failed to create graphics pipeline!");
	}

	PRINT ("Pipeline Created");
}

void Pipeline::Destroy (){
	vkDestroyPipelineLayout (device, pipelineLayout, allocator);
	vkDestroyPipeline (device, graphicsPipeline, allocator);
	vkDestroyDescriptorSetLayout (device, descriptorSetLayout, allocator);
	PRINT ("Pipeline destroyed");
}

std::vector<VkShaderStageFlagBits> Pipeline::GetShaderStagesFromShaders (const std::vector<Shader>& shaders){
	std::vector<VkShaderStageFlagBits> stages(shaders.size());
	for(const Shader& shader : shaders){
		stages.push_back (shader.GetStage ());
	}
	return std::vector<VkShaderStageFlagBits> ();
}

void Pipeline::FillInputAssemblyCreateInfo (VkPipelineInputAssemblyStateCreateInfo& inputAssembly){
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void Pipeline::FillViewPortStateCreateInfo (VkPipelineViewportStateCreateInfo & viewportState, const VkExtent2D& extent, VkViewport& viewport, VkRect2D& scissor){
	//If you want multiple viewport enable extension
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset = {0, 0};
	scissor.extent = extent;

	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
}

void Pipeline::FillShaderStageCreateInfo (VkPipelineShaderStageCreateInfo & fragShaderStageInfo, const Shader & shader){
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = shader.GetStage ();
	fragShaderStageInfo.module = shader.GetModule ();
	fragShaderStageInfo.pName = "main";
}

void Pipeline::FillRasterizerCreateInfo (VkPipelineRasterizationStateCreateInfo & rasterizer){
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
}

void Pipeline::FillMultisampleCreateInfo (VkPipelineMultisampleStateCreateInfo & multisampling){
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional
}

void Pipeline::FillColorBlendAttachmentState (VkPipelineColorBlendAttachmentState & colorBlendAttachment){
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
	/* If blended use this way
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	*/
}

void Pipeline::FillColorBlendStateCreateInfo (VkPipelineColorBlendStateCreateInfo & colorBlending,
											  const VkPipelineColorBlendAttachmentState& colorBlendAttachment){
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional
}

void Pipeline::FillDynamicStateCreateInfo (VkPipelineDynamicStateCreateInfo & dynamicState){
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;
}

void Pipeline::FillPipelineLayoutCreateInfo (VkPipelineLayoutCreateInfo & pipelineLayoutInfo, const VkDescriptorSetLayout& descriptorSetLayout){
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
}

}

