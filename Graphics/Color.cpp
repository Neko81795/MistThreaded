#include "Color.h"

#pragma warning(push, 0)
#include <d3d11_4.h>
#include <memory>
#pragma warning(pop)

namespace Graphics
{
	Color::Color(float r, float g, float b, float a) : RGBA{ r, g, b, a }, R(RGBA[0]), G(RGBA[1]), B(RGBA[2]), A(RGBA[3]){}

	Color::Color(const float * rgba, bool alpha) : R(RGBA[0]), G(RGBA[1]), B(RGBA[2]), A(RGBA[3])
	{
		R = rgba[0];
		G = rgba[1];
		B = rgba[2];
		if(alpha)
			A = rgba[3];
	}

	Color::Color(const Color & color) : R(RGBA[0]), G(RGBA[1]), B(RGBA[2]), A(RGBA[3])
	{
		memcpy(RGBA, color.RGBA, sizeof(RGBA));
	}

	Color & Color::operator=(const Color & rhs)
	{
		R = rhs.R;
		G = rhs.G;
		B = rhs.B;
		A = rhs.A;
		return *this;
	}

	float Color::operator[](int i)
	{
		return RGBA[i];
	}

	Color::operator float*()
	{
		return RGBA;
	}

	Color::operator const float*() const
	{
		return RGBA;
	}
}
