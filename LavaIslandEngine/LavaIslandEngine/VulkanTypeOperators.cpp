#include "VulkanTypeOperators.h"

bool operator==(const VkSurfaceFormatKHR & lhs, const VkSurfaceFormatKHR & rhs){
	return lhs.colorSpace == rhs.colorSpace &&
		lhs.format == rhs.format;
}
