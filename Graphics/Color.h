#pragma once

namespace Graphics
{
	struct Color
	{
		float RGBA[4];
		float& R;
		float& G;
		float& B;
		float& A;

		/// <summary>
		/// Creates a new color
		/// </summary>
		/// <param name="r">the red channel [0,1]</param>
		/// <param name="g">the green channel [0,1]</param>
		/// <param name="b">the blue channel [0,1]</param>
		/// <param name="a">the alpha channel [0,1]</param>
		Color(float r, float g, float b, float a = 1);
		/// <summary>
		/// Creates a new color
		/// </summary>
		/// <param name="rgba">a pointer to an array of color channels</param>
		/// <param name="alpha">if the array has an alpha channel</param>
		Color(const float* rgba, bool alpha);
		Color(const Color& color);

		operator float*();
		operator const float*() const;
		Color& operator=(const Color& rhs);
		float operator[](int i);
	};
}
