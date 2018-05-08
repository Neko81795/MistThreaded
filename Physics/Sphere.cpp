#include "Sphere.h"

namespace Physics
{
	Sphere::Sphere(Math::Vector3 center, double radius) : Center(center), Radius(radius){}

	bool Sphere::Intersects(const Math::Vector3 & point)
	{
		double dist = (point - Center).LengthSq();
		return dist <= (Radius * Radius);
	}


}
