#include "Vector3.h"
#include <cmath>

namespace Math
{
	Vector3::Vector3(double xyz) : X(xyz), Y(xyz), Z(xyz) {}

	Vector3::Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}

	Vector3 & Vector3::operator+=(const Vector3 & rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		return *this;
	}

	Vector3 & Vector3::operator-=(const Vector3 & rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.Z;
		return *this;
	}

	Vector3 & Vector3::operator*=(double rhs)
	{
		X *= rhs;
		Y *= rhs;
		Z *= rhs;
		return *this;
	}

	Vector3 & Vector3::operator/=(double rhs)
	{
		X /= rhs;
		Y /= rhs;
		Z /= rhs;
		return *this;
	}

	Vector3 Vector3::operator+(const Vector3 & rhs) const
	{
		Vector3 retval(*this);
		return retval += rhs;
	}

	Vector3 Vector3::operator-(const Vector3 & rhs) const
	{
		Vector3 retval(*this);
		return retval -= rhs;
	}

	Vector3 Vector3::operator*(double rhs) const
	{
		Vector3 retval(*this);
		return retval *= rhs;
	}

	Vector3 Vector3::operator/(double rhs) const
	{
		Vector3 retval(*this);
		return retval /= rhs;
	}

	Vector3 Vector3::operator-() const
	{
		return Vector3(-X, -Y, -Z);
	}

	double Vector3::operator[](int i) const
	{
		return *((&X) + i);
	}

	Vector3 Vector3::Normalized() const
	{
		Vector3 vec = *this / std::sqrt(X * X + Y * Y + Z * Z);
		return vec;
	}
	
	Vector3 Vector3::Cross(const Vector3 & rhs) const
	{
		return Vector3(
			Y * rhs.Z - Z * rhs.Y,
			Z * rhs.X - X * rhs.Z,
			X * rhs.Y - Y * rhs.X);
	}

	double Vector3::Dot(const Vector3 & rhs) const
	{
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
	}

	double Vector3::Length() const
	{
		return std::sqrt(LengthSq());
	}

	double Vector3::LengthSq() const
	{
		return Dot(*this);
	}
}
