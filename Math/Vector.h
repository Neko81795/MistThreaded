#pragma once

namespace Math {
	struct Vector3;
	struct Vector4;

	class Vector {
	public:
		template <int T1, int T2, int T3>
		Vector3 As();

		template <int T1, int T2, int T3, int T4>
		Vector4 As();
	};


	template<int T1, int T2, int T3>
	inline Vector3 Vector::As()
	{
		return Vector3((*this)[T1], (*this)[T2], (*this)[T3]);
	}

	template<int T1, int T2, int T3, int T4>
	inline Vector4 Vector::As()
	{
		return Vector3((*this)[T1], (*this)[T2], (*this)[T3], (*this)[T4]);
	}
}
