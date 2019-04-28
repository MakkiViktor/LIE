#include "PhysicalDevice.h"

#include <set>
#include "Types.h"
#include "Instance.h"
#include "Debug.h"
#include "StructureOperations.h"
#include "Surface.h"
#include "SwapChain.h"

namespace VK{
//PhysicalDeviceDetails

PhysicalDeviceDetails::PhysicalDeviceDetails (){
	SetDeviceExtensions ({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
	SetQueueFamilyInfos ({VK::QueueFamilyInfo::graphicsFamilyQueueInfo});
}

void PhysicalDeviceDetails::SetQueueFamilyInfos (const std::vector<QueueFamilyInfo>& infos){
	queueFamilyInfos = infos;
}
void PhysicalDeviceDetails::SetDeviceExtensions (const std::vector<const char*>& deviceExtensions){
	this->deviceExtensions = deviceExtensions;
}
void PhysicalDeviceDetails::SetEnabledFeatures (const VkPhysicalDeviceFeatures & enabledFeatures){
	this->enabledFeatures = enabledFeatures;
}

const std::vector<QueueFamilyInfo>& PhysicalDeviceDetails::GetQueueFamilyInfos () const{
	return queueFamilyInfos;
}

const VkPhysicalDeviceFeatures & PhysicalDeviceDetails::GetEnabledFeatures () const{
	return enabledFeatures;
}

const std::vector<const char*>& PhysicalDeviceDetails::GetDeviceExtensions () const{
	return deviceExtensions;
}

//PhysicalDevice

void PhysicalDevice::Create (const Instance & instance, const Surface & surface, const PhysicalDeviceDetails & details){
	std::vector<VkPhysicalDevice> devices (GetPhysicalDevices (instance));
	SelectDevice (devices, surface, details);
	SetupDetails (details);
	if(physicalDevice == VK_NULL_HANDLE)
		ERROR ("No physical device selected");
	PRINT ("Physical device selected");
}

void PhysicalDevice::AddQueueFamilyIndex (const QueueFamilyInfo& info){
	QueueFamilyIndex queueFamilyIndex = QueueFamilyIndex::CreateQueueFamilyIndex (physicalDevice,info);
	if(SO::Contains (queueFamilyIndices, queueFamilyIndex)){
		WARNING ("Queue index already added");
		return;
	}
	PRINT ("QueueFamily Added");
	queueFamilyIndices.push_back (queueFamilyIndex);
}

void PhysicalDevice::AddDeviceExtensions (const std::vector<const char*>& deviceExtensions){
	if(!HasExtensionSupport (physicalDevice, deviceExtensions))
		ERROR ("Device doesnt support extensions, try another isDeviceSuitable check");
	this->deviceExtensions = deviceExtensions;
}

const std::vector<QueueFamilyIndex>& PhysicalDevice::GetQueueFamilyIndices () const{
	return queueFamilyIndices;
}

std::vector<U32> PhysicalDevice::GetQueueFamilyIndicesArray () const{
	std::vector<U32> indices (queueFamilyIndices.size ());
	for(const auto& queueFamilyIndex : queueFamilyIndices)
		indices.push_back (queueFamilyIndex.queueFamily.value());
	return indices;
}

QueueFamilyIndex PhysicalDevice::GetQueueFamilyIndexByFlag (VkQueueFlagBits queueFlag) const{
	for(auto qi : queueFamilyIndices){
		if(qi.queueFlag == queueFlag)
			return qi;
	}
	ERROR ("QueueFamily not addded to this physical device");
	return queueFamilyIndices[0];
}


VkPhysicalDevice PhysicalDevice::GetPhysicalDevice () const {
	return physicalDevice;
}

 void PhysicalDevice::AddEnabledFeatures (const VkPhysicalDeviceFeatures & enabledFeatures){
	this->enabledFeatures = enabledFeatures;
}

 void PhysicalDevice::SetupDetails (const PhysicalDeviceDetails & details){
	 for(const auto& queueFamilyInfo : details.GetQueueFamilyInfos()){
		 AddQueueFamilyIndex (queueFamilyInfo);
	 }
	 deviceExtensions = details.GetDeviceExtensions ();
	 enabledFeatures = details.GetEnabledFeatures ();
 }

 const VkPhysicalDeviceFeatures& PhysicalDevice::GetEnabledFeatures () const{
	 return enabledFeatures;
 }

 const std::vector<const char*>& PhysicalDevice::GetDeviceExtensions () const{
	 return deviceExtensions;
 }

VkPhysicalDeviceFeatures PhysicalDevice::GetDeviceFeatures (const VkPhysicalDevice & device){
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures (device, &deviceFeatures);
	return deviceFeatures;
}

VkPhysicalDeviceProperties PhysicalDevice::GetDeviceProperties (const VkPhysicalDevice & device){
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties (device, &deviceProperties);
	return deviceProperties;
}

bool PhysicalDevice::HasQueueFamily (const VkPhysicalDevice& device, const QueueFamilyInfo& info){
	QueueFamilyIndex index = QueueFamilyIndex::CreateQueueFamilyIndex (device,info);
	return index.IsComplete();
}


bool PhysicalDevice::HasPresentSupport (const VkPhysicalDevice & device, const Surface& surface, U32 queueIndex){
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR (device, queueIndex, surface.GetSurface(), &presentSupport);
	return presentSupport;
}

bool PhysicalDevice::HasExtensionSupport (const VkPhysicalDevice & device, const std::vector<const char*>& deviceExtensions){
	U32 extensionCount;
	vkEnumerateDeviceExtensionProperties (device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions (extensionCount);
	vkEnumerateDeviceExtensionProperties (device, nullptr, &extensionCount, availableExtensions.data ());

	std::set<std::string> requiredExtensions (deviceExtensions.begin (), deviceExtensions.end ());

	for(const auto& extension : availableExtensions){
		requiredExtensions.erase (extension.extensionName);
	}

	return requiredExtensions.empty ();
}

std::vector<VkPhysicalDevice> PhysicalDevice::GetPhysicalDevices (const Instance& instance){
	U32 deviceCount = 0;
	vkEnumeratePhysicalDevices (instance.GetInstance (), &deviceCount, nullptr);
	if(deviceCount == 0){
		LIE::Debug::Error ("No GPU found");
	}
	std::vector<VkPhysicalDevice> devices (deviceCount);
	vkEnumeratePhysicalDevices (instance.GetInstance (), &deviceCount, devices.data ());
	return devices;
}

bool PhysicalDevice::IsDeviceSuitable (const VkPhysicalDevice & device, const Surface& surface, const PhysicalDeviceDetails& details){
	for(const auto& queueFamilyInfo : details.GetQueueFamilyInfos()){
		if(!HasQueueFamily (device, queueFamilyInfo))
			return false;
		if(queueFamilyInfo.hasPresentSupport){
			if(!HasPresentSupport (device, surface,
			   QueueFamilyIndex::CreateQueueFamilyIndex (device,
			   queueFamilyInfo).queueFamily.value ()))
				return false;
		}
	}
	if(!HasExtensionSupport (device, details.GetDeviceExtensions ()))
		return false;
	if(!SwapChainDetails::IsPhysicalDeviceSuitable (surface, device))
		return false;

	return true;
}

void PhysicalDevice::SelectDevice (const std::vector<VkPhysicalDevice>& devices, const Surface& surface, const PhysicalDeviceDetails& details){
	for(const auto& device : devices){
		if(IsDeviceSuitable (device, surface, details)){
			physicalDevice = device;
			break;
		}
	}
	if(physicalDevice == VK_NULL_HANDLE){
		LIE::Debug::Error ("failed to find a suitable GPU!");
	}
}

//QueueFamilyIndex

QueueFamilyIndex::QueueFamilyIndex (){}

bool QueueFamilyIndex::IsComplete (const QueueFamilyInfo& info){
	return queueFamily.has_value () &&
		   queueFlag == info.queueFlag &&
		   queueCount == info.queueCount &&
		   priority == info.priority;
}

bool QueueFamilyIndex::IsComplete (){
	return queueFamily.has_value ();
}

bool QueueFamilyIndex::operator==(const QueueFamilyIndex & other) const{
	return queueFamily.value() == other.queueFamily.value();
}

QueueFamilyIndex QueueFamilyIndex::CreateQueueFamilyIndex (const VkPhysicalDevice & physicalDevice, const QueueFamilyInfo & info){
	U32 queueFamilyCount = 0;
	QueueFamilyIndex queueFamilyIndex;
	queueFamilyIndex.queueFlag = info.queueFlag;
	queueFamilyIndex.priority = info.priority;
	vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, queueFamilies.data ());

	U32 i = 0;
	for(const auto& queueFamily : queueFamilies){
		if(queueFamily.queueCount > 0 && queueFamily.queueFlags & info.queueFlag){
			queueFamilyIndex.queueFamily = i;
		}
		if(queueFamilyIndex.IsComplete (info)){
			break;
		}
		i++;
	}
	if(!queueFamilyIndex.IsComplete (info)){
		LIE::Debug::Error ("No queue index found");
	}
	return queueFamilyIndex;
}

//QueueFamilyInfo

QueueFamilyInfo QueueFamilyInfo::CreateGraphicsQueueFamilyInfo (){
	QueueFamilyInfo info = {};
	info.queueFlag = VK_QUEUE_GRAPHICS_BIT;
	info.hasPresentSupport = true;
	info.priority = 1.0;
	info.queueCount = 1;
	return info;
}

const QueueFamilyInfo QueueFamilyInfo::graphicsFamilyQueueInfo = CreateGraphicsQueueFamilyInfo ();


}
