#ifndef STRUCTURE_OPERATIONS
#define STRUCTURE_OPERATIONS

#pragma once

#include <vector>
#include "VulkanTypeOperators.h"

namespace SO{
	template<typename Type>
	static bool Contains (const std::vector<Type>& list, const Type& element);


	template<typename Type>
	bool Contains (const std::vector<Type>& list, const Type & element){
		for(const auto& e : list){
			if(e == element)
				return true;
		}
		return false;
	}
}
#endif 
