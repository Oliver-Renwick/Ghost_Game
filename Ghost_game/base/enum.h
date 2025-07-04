#pragma once
#include <stdint.h>

static uint32_t g_invalid_index = 0xffffffff;

namespace resource_type
{
	enum Enum
	{
		Model, Texture, Sound, Animation, null
	};

	static const char* value_names[] = { "Model", "Texture", "Sound", "Animation" };

	static const char* ToString(Enum e)
	{
		return ((uint32_t)e < null) ? value_names[(int)e] : "UnSupported";
	}
}