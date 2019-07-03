#include "Material.h"

namespace VK{

void Material::Create (const Pipeline & pipeline){
	this->pipeline = pipeline;
}

void Material::Destroy (){
	pipeline.Destroy ();
}

const Pipeline& Material::GetPipeline () const{
	return pipeline;
}

}
