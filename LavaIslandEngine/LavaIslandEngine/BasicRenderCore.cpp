#include "BasicRenderCore.h"

#include "Queue.h"
#include "Window.h"
#include "SharedResources.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "StagingBuffer.h"
#include "BufferTransferer.h"
#include "UniformBuffer.h"
#include "UniformIncludes.h"
#include <chrono>

#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

namespace VK{

void BasicRenderCore::Create (const LogicalDevice& logicalDevice, const Surface& surface){
	this->logicalDevice = logicalDevice;
	this->surface = surface;
	swapChain = CreateSwapChain (logicalDevice, surface);
	imageviews = CreateImageView (swapChain);
	renderPass = CreateRenderPass (swapChain);
	frameBuffers = CreateFrameBuffers (imageviews, renderPass);
	descriptorPool = CreateDescriptorPool (swapChain);
	commandPool = CreateCommandPool (logicalDevice);
}

void BasicRenderCore::Destroy (){
	commandPool.Destroy ();
	descriptorPool.Destroy ();
	frameBuffers.Destroy ();
	DestroyGraphicsDatas ();
	for(Buffer& buffer : uniformBuffers)
		buffer.Destroy ();
	uniformBuffers.clear ();
	renderPass.Destroy ();
	imageviews.Destroy ();
	swapChain.Destroy ();
}

VkSwapchainKHR BasicRenderCore::GetSwapChain (){
	return swapChain.GetSwapchain();
}

const std::vector<CommandBuffers> & BasicRenderCore::GetCommandBuffers (){
	return commandBuffers;
}

void BasicRenderCore::Recreate (Window& window, const std::vector<MeshData<Vertex, UniformMVP>>& meshDatas){
	window.Refresh();
	sharedWindow = window;
	frameBuffers.Destroy ();
	for(DescriptorSets& sets : descriptorSets)
		descriptorPool.Free (sets);
	for(CommandBuffers& command : commandBuffers)
		commandPool.Free (command);
	commandBuffers.clear ();
	DestroyGraphicsDatas ();
	for(Buffer& buffer : uniformBuffers)
		buffer.Destroy ();
	uniformBuffers.clear ();
	renderPass.Destroy ();
	imageviews.Destroy ();
	swapChain.Destroy ();

	vkDeviceWaitIdle (logicalDevice.GetLogicalDevice ());

	swapChain = CreateSwapChain (logicalDevice, surface);
	imageviews = CreateImageView (swapChain);
	renderPass = CreateRenderPass (swapChain);
	frameBuffers = CreateFrameBuffers (imageviews, renderPass);
	AddMeshes (meshDatas);
	CreateUniformBuffers (uniformBuffers, logicalDevice, swapChain);

}

SwapChain BasicRenderCore::CreateSwapChain (const LogicalDevice & logicalDevice, const Surface & surface){
	SwapChain swapChain;
	swapChain.Create (logicalDevice, surface);
	return swapChain;
}

ImageViews BasicRenderCore::CreateImageView (const SwapChain & swapChain){
	ImageViews imagaviews;
	imagaviews.Create (swapChain);
	return imagaviews;
}

RenderPass BasicRenderCore::CreateRenderPass (const SwapChain & swapChain){
	RenderPass renderPass;
	renderPass.Create (swapChain);
	return renderPass;
}

Pipeline BasicRenderCore::CreatePipeline (const SwapChain& swapChain,
									  const RenderPass& renderPass,
									  const std::vector<ShaderDetails>& shaderDetails){
	Pipeline pipeline;
	pipeline.Create <Vertex, UniformMVP> (shaderDetails, swapChain, renderPass);
	return pipeline;
}

DescriptorPool BasicRenderCore::CreateDescriptorPool (const SwapChain & swapChain){
	DescriptorPool descriptorPool;
	descriptorPool.Create (swapChain);
	return descriptorPool;
}

FrameBuffers BasicRenderCore::CreateFrameBuffers (const ImageViews & imageViews, const RenderPass & renderPass){
	FrameBuffers frameBuffers;
	frameBuffers.Create (imageViews, renderPass);
	return frameBuffers;
}

void BasicRenderCore::CreateUniformBuffers (std::vector<UniformBuffer<UniformMVP>>& uniformBuffers,
											const LogicalDevice & logicalDevice,
											const SwapChain & swapChain){
	U16 imageCount = swapChain.GetImageCount ();
	uniformBuffers.resize (imageCount);
	for(U16 imageIndex = 0; imageIndex < imageCount; imageIndex++){
		UniformBuffer<UniformMVP> uniformBuffer;
		uniformBuffer.Create (logicalDevice);
		uniformBuffers[imageIndex] = uniformBuffer;
	}
}

CommandPool BasicRenderCore::CreateCommandPool (const LogicalDevice & logicalDevice){
	CommandPool commandPool;
	commandPool.Create (logicalDevice);
	return commandPool;
}

CommandBuffers BasicRenderCore::CreateCommandBuffers (const CommandPool& commandPool,
													  const FrameBuffers & frameBuffers,
													  const DescriptorSets& descriptorSets,
													  const std::vector<GraphicsData>& graphicsDatas){
	CommandBuffers commandBuffers;
	commandBuffers.Create (commandPool, frameBuffers, descriptorSets, graphicsDatas);
	return commandBuffers;
}

void BasicRenderCore::CreateMesh (const MeshData<Vertex, UniformMVP>& meshData){

	Pipeline pipeline = CreatePipeline (swapChain, renderPass, meshData.shaderDetails);

	std::vector<Buffer> vertexBuffers(1);
	VertexBuffer<Vertex> vertexBuffer;
	vertexBuffer.Create (logicalDevice, meshData.vertices);
	vertexBuffers[0] = vertexBuffer;

	IndexBuffer indexBuffer;
	indexBuffer.Create (logicalDevice, meshData.indices);

	BufferTransferer::TransferDataToBuffer (meshData.vertices, vertexBuffer);
	BufferTransferer::TransferDataToBuffer (meshData.indices, indexBuffer);

	graphicsDatas.push_back (GraphicsData{pipeline, vertexBuffers, indexBuffer});

}

DescriptorSets BasicRenderCore::CreateDescriptorSets (const SwapChain & swapChain,
													  const Pipeline & pipeline,
													  const std::vector<UniformBuffer<UniformMVP>>& uniformBuffers,
													  const DescriptorPool & descriptorPool){
	DescriptorSets descriptorSets;
	descriptorSets.Create (swapChain, pipeline, descriptorPool);
	descriptorSets.SetUniforms (uniformBuffers);
	return descriptorSets;
}

void BasicRenderCore::DestroyGraphicsDatas (){
	for(GraphicsData& data : graphicsDatas){
		data.indexBuffer.Destroy ();
		for(Buffer& buffer : data.vertexBuffers)
			buffer.Destroy ();
		data.pipeline.Destroy ();
	}
	graphicsDatas.clear ();
}

 void BasicRenderCore::AddMeshes (const std::vector<MeshData<Vertex, UniformMVP>>& meshDatas){
	 for(const MeshData<Vertex, UniformMVP>& meshData : meshDatas)
		CreateMesh (meshData);
	 for(CommandBuffers& command : commandBuffers)
		 commandPool.Free (command);
	 for(DescriptorSets& sets : descriptorSets)
		 descriptorPool.Free (sets);
	 descriptorSets.clear ();
	 commandBuffers.clear ();
	 vkDeviceWaitIdle (logicalDevice.GetLogicalDevice ());
	 
	 CreateUniformBuffers (uniformBuffers, logicalDevice, swapChain);
	 for(GraphicsData& data : graphicsDatas)
		 descriptorSets.push_back (CreateDescriptorSets (swapChain, data.pipeline, uniformBuffers, descriptorPool));
	 //TODO descriptor setek mashogy
	 commandBuffers.push_back (CreateCommandBuffers (commandPool, frameBuffers, descriptorSets[0],graphicsDatas));
 }

 void BasicRenderCore::UpdateUniformBuffers (U16 currentFrame){
	 
	 static auto startTime = std::chrono::high_resolution_clock::now ();

	 auto currentTime = std::chrono::high_resolution_clock::now ();
	 float time = std::chrono::duration<float, std::chrono::seconds::period> (currentTime - startTime).count ();
	 UniformMVP ubo = {};
	 glm::mat4 model = glm::rotate (glm::mat4 (1.0f), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
	 glm::mat4 view = glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
	 glm::mat4 proj = glm::perspective (glm::radians (45.0f), 800.0f / 600.0f, 0.1f, 10.0f);
	 proj[1][1] *= -1;
	 ubo.mvp = proj * view * model;
	 void* data;
	 vkMapMemory (logicalDevice.GetLogicalDevice(), uniformBuffers[currentFrame].GetBufferMemory(), 0, sizeof (ubo), 0, &data);
	 memcpy (data, &ubo, sizeof (ubo));
	 vkUnmapMemory (logicalDevice.GetLogicalDevice (), uniformBuffers[currentFrame].GetBufferMemory());
 }

}

