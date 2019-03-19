#ifndef LIE_TYPES
#define LIE_TYPES

#pragma once

#include <stdint.h>
#include "Debug.h"

using U16 = uint16_t;
using U32 = uint32_t;
using I32 = int_fast32_t;
using BYTE = char;
using SIZE = size_t;

#ifdef _DEBUG
#define ERROR(message) LIE::Debug::Error(message)
#define WARNING(message) LIE::Debug::Warning(message)
#define PRINT(message) LIE::Debug::Print(message)
#define BREAK LIE::Debug::Break()

#else
#define ERROR(message)
#define WARNING(message)
#define PRINT(message)
#define BREAK
#endif 


namespace LIE {
	
	static const char* APPLICATION_NAME = "No Application";
	static const char* ENGINE_NAME = "Lava Island Engine";
	static const U16 VERSION_MAJOR = 1;
	static const U16 VERSION_MINOR = 0;
	static const U32 DEFAULT_WIDTH = 1280;
	static const U32 DEFAULT_HEIGHT = 720;
}
#endif
