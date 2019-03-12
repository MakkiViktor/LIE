#ifndef LIE_VK_PHYSICAL_DEVICE
#define LIE_VK_PHYSICAL_DEVICE

#pragma once

#include "vulkan/vulkan.h"
#include <functional>
#include <vector>
#include <optional>
#include "Types.h"

namespace VK{

class Instance;
class Surface;

struct QueueFamilyInfo{
private:
	static QueueFamilyInfo CreateGraphicsQueueFamilyInfo ();
public:
	VkQueueFlagBits queueFlag;
	U32 queueCount;
	float priority;
	VkBool32 hasPresentSupport = false;
	static const QueueFamilyInfo graphicsFamilyQueueInfo;
};

struct QueueFamilyIndex{
private:
	QueueFamilyIndex ();
public:
	VkQueueFlagBits queueFlag;
	U32 queueCount = 1;
	float priority = 1.0;
	std::optional<U32> queueFamily;
	bool IsComplete (const QueueFamilyInfo& info);
	bool IsComplete ();
	bool operator == (const QueueFamilyIndex& other) const;
	static QueueFamilyIndex CreateQueueFamilyIndex (const VkPhysicalDevice& device,
													const QueueFamilyInfo& info);
};


class PhysicalDeviceDetails{
	std::vector<QueueFamilyInfo> queueFamilyInfos;
	std::vector<const char*> deviceExtensions;
	VkPhysicalDeviceFeatures enabledFeatures = {};
public:
	PhysicalDeviceDetails ();
	void SetQueueFamilyInfos (const std::vector<QueueFamilyInfo>& infos);
	void SetDeviceExtensions (const std::vector<const char*>& deviceExtensions);
	void SetEnabledFeatures (const VkPhysicalDeviceFeatures& enabledFeatures);

	const std::vector<QueueFamilyInfo>& GetQueueFamilyInfos () const;
	const VkPhysicalDeviceFeatures& GetEnabledFeatures () const;
	const std::vector<const char*>& GetDeviceExtensions () const;
};

class PhysicalDevice{
private:
	std::vector<QueueFamilyIndex> queueFamilyIndices;
	std::vector<const char*> deviceExtensions;
	VkPhysicalDeviceFeatures enabledFeatures = {};
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	static std::vector<VkPhysicalDevice> GetPhysicalDevices (Instance& instance);
	static bool IsDeviceSuitable(const VkPhysicalDevice& device, const Surface& surface, const PhysicalDeviceDetails& details);

	void AddQueueFamilyIndex (const QueueFamilyInfo& info);
	void AddDeviceExtensions (const std::vector<const char*>& deviceExtensions);
	void AddEnabledFeatures (const VkPhysicalDeviceFeatures& enabledFeatures);
	
	void SelectDevice (const std::vector<VkPhysicalDevice>& devices, const Surface& surface, const PhysicalDeviceDetails& details);
	void SetupDetails (const PhysicalDeviceDetails& details);
	
	static bool HasQueueFamily (const VkPhysicalDevice& device, const QueueFamilyInfo& info);
	static bool HasPresentSupport (const VkPhysicalDevice& device, const Surface& surface, U32 queueIndex);
	static bool HasExtensionSupport (const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions);
public:
	PhysicalDevice (Instance& instance, const Surface& surface, const PhysicalDeviceDetails& details);
	~PhysicalDevice ();
	
	VkPhysicalDevice GetPhysicalDevice () const;
	QueueFamilyIndex GetQueueFamilyIndexByFlag (VkQueueFlagBits queueFlag) const;
	const std::vector<QueueFamilyIndex>& GetQueueFamilyIndices () const;
	std::vector<U32> GetQueueFamilyIndicesArray () const;
	const VkPhysicalDeviceFeatures& GetEnabledFeatures () const;
	const std::vector<const char*>& GetDeviceExtensions () const;

	static VkPhysicalDeviceFeatures GetDeviceFeatures (const VkPhysicalDevice& device);
	static VkPhysicalDeviceProperties GetDeviceProperties (const VkPhysicalDevice& device);

	
};

}

#endif 
