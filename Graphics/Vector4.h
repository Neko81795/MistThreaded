#pragma once

namespace Graphics
{
	struct Vector4
	{
		float X;
		float Y;
		float Z;
		float W;

		Vector4(float xyz = 0, float w = 0);
		Vector4(float x, float y, float z, float w = 0);

		Vector4& operator +=(const Vector4& rhs);
		Vector4& operator -=(const Vector4& rhs);
		Vector4& operator *=(float rhs);
		Vector4& operator /=(float rhs);

		Vector4 operator +(const Vector4& rhs) const;
		Vector4 operator -(const Vector4& rhs) const;
		Vector4 operator *(float rhs) const;
		Vector4 operator /(float rhs) const;
		Vector4 operator -() const;

		Vector4 Normalize() const;

		Vector4 Cross(const Vector4& rhs) const;
	};
}