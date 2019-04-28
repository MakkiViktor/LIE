#ifndef LIE_VK_DESCRIPTOR_SETS
#define LIE_VK_DESCRIPTOR_SETS
#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "UniformBuffer.h"

namespace VK{

class SwapChain;
class DescriptorPool;
class Pipeline;

class DescriptorSets{
private:
	std::vector<VkDescriptorSetLayout> layouts;
	std::vector<VkDescriptorSet> descriptorSets;
public:
	void Create (const SwapChain& swapChain,
				 const Pipeline& pipeline,
				 const DescriptorPool& descriptorPool);

	const VkDescriptorSet& operator[](SIZE index) const;

	U32 Size () const;
	const VkDescriptorSet* Data () const;

	template<class UNIFORM>
	void SetUniforms (std::vector<UniformBuffer<UNIFORM>> uniformBuffers);
};

template<class UNIFORM>
inline void DescriptorSets::SetUniforms (std::vector<UniformBuffer<UNIFORM>> uniformBuffers){
	for(size_t i = 0; i < uniformBuffers.size (); i++){
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i].GetBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof (UNIFORM);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = UNIFORM::GetBinding();
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional
		vkUpdateDescriptorSets (uniformBuffers[i].GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

}

#endif