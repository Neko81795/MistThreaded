#pragma once
#include "Vector.h"

namespace Math
{
	struct Vector3 : public Vector
	{
	public:
		static Vector3 Zero;
		static Vector3 Origin;
		static Vector3 One;
		double X;
		double Y;
		double Z;

		Vector3(double xyz = 0);
		Vector3(double x, double y, double z);

		Vector3& operator +=(const Vector3& rhs);
		Vector3& operator -=(const Vector3& rhs);
		Vector3& operator *=(double rhs);
		Vector3& operator /=(double rhs);

		Vector3 operator +(const Vector3& rhs) const;
		Vector3 operator -(const Vector3& rhs) const;
		Vector3 operator *(double rhs) const;
		Vector3 operator /(double rhs) const;
		Vector3 operator -() const;
		double operator [](int i) const;

		Vector3 Normalized() const;

		Vector3 Cross(const Vector3& rhs) const;
		double Dot(const Vector3& rhs) const;
		double Length() const;
		double LengthSq() const;
	};
}