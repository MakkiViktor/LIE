#include "DescriptorSets.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "DescriptorPool.h"
#include "UniformIncludes.h"

namespace VK{

void DescriptorSets::Create (const SwapChain & swapChain, const Pipeline & pipeline, const DescriptorPool & descriptorPool){
	U16 imageCount = swapChain.GetImageCount ();
	layouts = std::vector<VkDescriptorSetLayout> (imageCount, pipeline.GetDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.GetDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
	allocInfo.pSetLayouts = layouts.data ();

	descriptorSets.resize (imageCount);
	if(vkAllocateDescriptorSets (swapChain.GetLogicalDevice(), &allocInfo, descriptorSets.data ()) != VK_SUCCESS){
		ERROR ("failed to allocate descriptor sets!");
	}
}

const VkDescriptorSet & DescriptorSets::operator[](SIZE index) const{
	return descriptorSets[index];
}

U32 DescriptorSets::Size () const{
	return static_cast<U32>(descriptorSets.size());
}

const VkDescriptorSet * DescriptorSets::Data () const{
	return descriptorSets.data();
}

}
