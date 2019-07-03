#define LIE_VK_MESH
#pragma once

#include "Geometry.h"
#include "Material.h"
#include "UniformBuffer.h"

namespace VK{

template<class UNIFORM, class VERTEX>
class Mesh{
	Geometry<VERTEX> geometry;
	Material material;
	UniformBuffers<UNIFORM> uniformBuffer;
public:
	void  Create (const Geometry<VERTEX>& geometry, 
		  const Material& material,
		  const UniformBuffers<UNIFORM>& uniformBuffer);

	void Destroy ();

	const VertexBuffer<Vertex>& GetVertexBuffer () const;
	const IndexBuffer& GetIndexBuffer () const;
	const std::vector<UniformBuffer<UNIFORM>>& GetUniformBuffers () const;

	void  SetUniform (const UNIFORM& uniform, const U32 imageIndex);
};

template<class UNIFORM, class VERTEX>
inline void Mesh<UNIFORM, VERTEX>::Create (const Geometry<VERTEX>& geometry, const Material & material, const UniformBuffers<UNIFORM>& uniformBuffer){
	this->geometry = geometry;
	this->material = material;
	this->uniformBuffer = uniformBuffer;
}

template<class UNIFORM, class VERTEX>
inline void Mesh<UNIFORM, VERTEX>::Destroy (){
	geometry.Destroy ();
	material.Destroy ();
}

template<class UNIFORM, class VERTEX>
inline const VertexBuffer<Vertex>& Mesh<UNIFORM, VERTEX>::GetVertexBuffer () const{
	return geometry.GetVertexBuffer ();
}

template<class UNIFORM, class VERTEX>
inline const IndexBuffer & Mesh<UNIFORM, VERTEX>::GetIndexBuffer () const{
	return geometry.GetIndexBuffer();
}


template<class UNIFORM, class VERTEX>
inline const std::vector<UniformBuffer<UNIFORM>>& Mesh<UNIFORM, VERTEX>::GetUniformBuffers () const{
	return uniformBuffer.GetUniformBuffers();
}

template<class UNIFORM, class VERTEX>
inline void Mesh<UNIFORM, VERTEX>::SetUniform (const UNIFORM& uniform, const U32 imageIndex){
	UniformBuffer buffer = uniformBuffer.GetUniformBuffers ()[imageIndex];
	void* data;
	vkMapMemory (buffer.GetLogicalDevice(), buffer.GetBufferMemory (), 0, sizeof (uniform), 0, &data);
	memcpy (data, &uniform, sizeof (uniform));
	vkUnmapMemory (buffer.GetLogicalDevice (), buffer.GetBufferMemory ());
}

}
