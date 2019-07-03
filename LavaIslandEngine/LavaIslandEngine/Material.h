#define LIE_VK_MATERIAL
#pragma once

#include "Pipeline.h"

namespace VK{

class Material{
	Pipeline pipeline;
public:
	void Create (const Pipeline& pipeline);
	void Destroy ();
	const Pipeline& GetPipeline () const;
};

}
