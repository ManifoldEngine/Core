#pragma once

namespace Mani
{
	enum class TextureFiltering : uint8_t
	{
		LINEAR, // returns a weighted average of the color
		NEAREST, // returns the exact color
	};
}