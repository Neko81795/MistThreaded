#include "Vector4.h"
#include <cmath>

namespace Graphics
{
	Vector4::Vector4(float xyz, float w) : X(xyz), Y(xyz), Z(xyz), W(w) {}

	Vector4::Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

	Vector4 & Vector4::operator+=(const Vector4 & rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		W += rhs.W;
		return *this;
	}

	Vector4 & Vector4::operator-=(const Vector4 & rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.Z;
		W -= rhs.W;
		return *this;
	}

	Vector4 & Vector4::operator*=(float rhs)
	{
		X *= rhs;
		Y *= rhs;
		Z *= rhs;
		W *= rhs;
		return *this;
	}

	Vector4 & Vector4::operator/=(float rhs)
	{
		X /= rhs;
		Y /= rhs;
		Z /= rhs;
		W /= rhs;
		return *this;
	}

	Vector4 Vector4::operator+(const Vector4 & rhs) const
	{
		Vector4 retval(*this);
		return retval += rhs;
	}

	Vector4 Vector4::operator-(const Vector4 & rhs) const
	{
		Vector4 retval(*this);
		return retval -= rhs;
	}

	Vector4 Vector4::operator*(float rhs) const
	{
		Vector4 retval(*this);
		return retval *= rhs;
	}

	Vector4 Vector4::operator/(float rhs) const
	{
		Vector4 retval(*this);
		return retval /= rhs;
	}

	Vector4 Vector4::operator-() const
	{
		return Vector4(-X, -Y, -Z, -W);
	}

	Vector4 Vector4::Normalize() const
	{
		Vector4 vec = *this / std::sqrt(X * X + Y * Y + Z * Z);
		vec.W = W;
		return vec;
	}
	
	Vector4 Vector4::Cross(const Vector4 & rhs) const
	{
		return Vector4(
			Y * rhs.Z - Z * rhs.Y,
			Z * rhs.X - X * rhs.Z,
			X * rhs.Y - Y * rhs.X);
	}
}
