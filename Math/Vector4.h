#pragma once
#include "Vector.h"

namespace Math
{
	struct Vector4 : public Vector
	{
	public:
		static Vector4 Zero;
		static Vector4 Origin;
		static Vector4 One;
		double X;
		double Y;
		double Z;
		double W;

		Vector4(double xyz = 0, double w = 0);
		Vector4(double x, double y, double z, double w = 0);

		Vector4& operator +=(const Vector4& rhs);
		Vector4& operator -=(const Vector4& rhs);
		Vector4& operator *=(double rhs);
		Vector4& operator /=(double rhs);

		Vector4 operator +(const Vector4& rhs) const;
		Vector4 operator -(const Vector4& rhs) const;
		Vector4 operator *(double rhs) const;
		Vector4 operator /(double rhs) const;
		Vector4 operator -() const;
		double operator [](int i) const;

		Vector4 Normalized() const;

		Vector4 Cross(const Vector4& rhs) const;
	};
}