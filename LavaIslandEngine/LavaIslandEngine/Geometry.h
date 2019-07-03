#define LIE_VK_GEOMETRY
#pragma once

#include <vector>
#include "Types.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace VK{

template<class VERTEX>
class Geometry{
	VertexBuffer<VERTEX> vertexBuffer;
	IndexBuffer indexBuffer;
public:
	void Create (const VertexBuffer<VERTEX>& vertexBuffer, const IndexBuffer& indexBuffer);
	void Destroy ();
	const VertexBuffer<Vertex>& GetVertexBuffer () const;
	const IndexBuffer& GetIndexBuffer () const;
};

template<class VERTEX>
inline void Geometry<VERTEX>::Create (const VertexBuffer<VERTEX>& vertexBuffer, const IndexBuffer & indexBuffer){
	this->vertexBuffer = vertexBuffer;
	this->indexBuffer = indexBuffer;
}

template<class VERTEX>
inline void Geometry<VERTEX>::Destroy (){
	vertexBuffer.Destroy ();
	indexBuffer.Destroy ();
}

template<class VERTEX>
inline const VertexBuffer<Vertex>& Geometry<VERTEX>::GetVertexBuffer () const{
	return vertexBuffer;
}

template<class VERTEX>
inline const IndexBuffer & Geometry<VERTEX>::GetIndexBuffer () const{
	return indexBuffer;
}

}