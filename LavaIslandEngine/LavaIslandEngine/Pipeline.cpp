#include "Pipeline.h"
#include "SwapChain.h"
#include "Types.h"
#include "RenderPass.h"

namespace VK{

Pipeline::Pipeline (const std::vector<ShaderDetails>& shaderDetails, const SwapChain& swapChain, const RenderPass& renderPass, const VkAllocationCallbacks* allocator):
 device(swapChain.GetLogicalDevice()), allocator(allocator){

	std::vector<Shader> shaders;
	std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos;
	
	for(const ShaderDetails& shaderDetail : shaderDetails){
		shaders.push_back (Shader (shaderDetail, device, allocator));
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
		FillShaderStageCreateInfo (fragShaderStageInfo, shaders.back());
		shaderCreateInfos.push_back (fragShaderStageInfo);
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {}; 
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

	FillVertexInputCreateInfo (vertexInputInfo);
	FillInputAssemblyCreateInfo (inputAssembly);
	FillViewPortStateCreateInfo (viewportState, swapChain.GetExtent  ());
	FillMultisampleCreateInfo (multisampling);
	FillColorBlendAttachmentState (colorBlendAttachment);
	FillDynamicStateCreateInfo (dynamicState);
	FillColorBlendStateCreateInfo (colorBlending, colorBlendAttachment);
	FillRasterizerCreateInfo (rasterizer);
	FillPipelineLayoutCreateInfo (pipelineLayoutInfo);

	if(vkCreatePipelineLayout (device.GetLogicalDevice(), &pipelineLayoutInfo, allocator, &pipelineLayout) != VK_SUCCESS){
		ERROR ("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {}	;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<U32> (shaderCreateInfos.size ());
	pipelineInfo.pStages = shaderCreateInfos.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass.GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;

	if(vkCreateGraphicsPipelines (device.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, allocator, &graphicsPipeline) != VK_SUCCESS){
		ERROR("failed to create graphics pipeline!");
	}

	PRINT ("Pipeline Created");
}

const VkPipeline& Pipeline::GetPipeline () const{
	return graphicsPipeline;
}

Pipeline::~Pipeline (){
	vkDestroyPipelineLayout (device.GetLogicalDevice(), pipelineLayout, allocator);
	vkDestroyPipeline (device.GetLogicalDevice(),graphicsPipeline, allocator);
	PRINT ("Pipeline destroyed");
}

std::vector<VkShaderStageFlagBits> Pipeline::GetShaderStagesFromShaders (const std::vector<Shader>& shaders){
	std::vector<VkShaderStageFlagBits> stages(shaders.size());
	for(const Shader& shader : shaders){
		stages.push_back (shader.GetStage ());
	}
	return std::vector<VkShaderStageFlagBits> ();
}

void Pipeline::FillVertexInputCreateInfo (VkPipelineVertexInputStateCreateInfo & vertexInputInfo){
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
}

void Pipeline::FillInputAssemblyCreateInfo (VkPipelineInputAssemblyStateCreateInfo& inputAssembly){
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void Pipeline::FillViewPortStateCreateInfo (VkPipelineViewportStateCreateInfo & viewportState, const VkExtent2D& extent){
	//If you want multiple viewport enable extension
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
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
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

void Pipeline::FillPipelineLayoutCreateInfo (VkPipelineLayoutCreateInfo & pipelineLayoutInfo){
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
}

}
