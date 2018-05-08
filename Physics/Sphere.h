#pragma once
#include "Math/Vector4.h"
#include "Math/Vector3.h"

namespace Physics
{
	class Sphere
	{
	public:
		Sphere(Math::Vector3 center = Math::Vector3::Zero, double radius = 0);

		bool Intersects(const Math::Vector3& point);

		Math::Vector3 Center;
		double Radius;
	};
}
