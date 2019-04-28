#ifndef LIE_VK_BASIC_RENDER_CORE
#define LIE_VK_BASIC_RENDER_CORE
#pragma once

#include "SwapChain.h"
#include "ImageViews.h"
#include "Pipeline.h"
#include "Shader.h"
#include "RenderPass.h"
#include "FrameBuffers.h"
#include "DescriptorSets.h"
#include "DescriptorPool.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "Surface.h"
#include "Semaphore.h"
#include "Fence.h"
#include "Buffer.h"

namespace VK{

class Queue;
class Window;
class IndexBuffer;

template<class UNIFORM>
struct UniformData{
	UNIFORM& uniform;
};

template<class VERTEX, class UNIFORM>
struct MeshData{
	std::vector<ShaderDetails> shaderDetails;
	std::vector<VERTEX> vertices;
	std::vector<U32> indices;
	UNIFORM uniforms;
};

class BasicRenderCore{
private:
	LogicalDevice logicalDevice;
	Surface surface;
	SwapChain swapChain;
	ImageViews imageviews;
	RenderPass renderPass;
	DescriptorPool descriptorPool;
	CommandPool commandPool;
	FrameBuffers frameBuffers;
	std::vector<GraphicsData> graphicsDatas;

	std::vector<CommandBuffers> commandBuffers;
	std::vector<UniformBuffer<UniformMVP>> uniformBuffers;
	std::vector<DescriptorSets> descriptorSets;

	virtual SwapChain CreateSwapChain (const LogicalDevice& logicalDevice, const Surface& surface);
	
	virtual ImageViews CreateImageView (const SwapChain& swapChain);
	
	virtual RenderPass CreateRenderPass (const SwapChain& swapChain);
	
	virtual FrameBuffers CreateFrameBuffers (const ImageViews& imageViews, const RenderPass& renderPass);

	virtual void CreateUniformBuffers (std::vector<UniformBuffer<UniformMVP>>& uniformBuffers,
									   const LogicalDevice& logicalDevice,
									   const SwapChain& swapChain);

	virtual Pipeline CreatePipeline (const SwapChain& swapChain, 
									   const RenderPass& renderPass,
									   const std::vector<ShaderDetails>& shaderDetails);
	
	virtual DescriptorPool CreateDescriptorPool (const SwapChain& swapChain);
	
	virtual DescriptorSets CreateDescriptorSets (const SwapChain& swapChain,
												 const Pipeline& pipeline,
												 const std::vector<UniformBuffer<UniformMVP>>& uniformBuffers,
												 const DescriptorPool& descriptorPool);
	
	virtual CommandPool CreateCommandPool (const LogicalDevice& logicalDevice);
	
	virtual CommandBuffers CreateCommandBuffers (const CommandPool& commandPool,
												 const FrameBuffers& frameBuffers,
												 const DescriptorSets& descriptorSets,
												 const std::vector<GraphicsData>& graphicsDatas);
	
	void CreateMesh (const MeshData<Vertex, UniformMVP>& meshData);
	
	void DestroyGraphicsDatas ();

public:
	void Create (const LogicalDevice& logicalDevice, const Surface& surface);
	void Recreate (Window& window, const std::vector<MeshData<Vertex,UniformMVP>>& meshDatas);
	void Destroy ();
	VkSwapchainKHR GetSwapChain ();
	const std::vector<CommandBuffers>& GetCommandBuffers ();
	void AddMeshes (const std::vector<MeshData<Vertex, UniformMVP>>& meshDatas);
	void UpdateUniformBuffers (U16 currentFrame);
};

}

#endif 
