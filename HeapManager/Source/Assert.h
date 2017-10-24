#pragma once

#include <crtdbg.h>
#include <assert.h>
#include <cassert>

#if defined(_DEBUG)
	#define ASSERT(condition)	void(0)
#else
	#define ASSERT(condition)	void(0)
#endif