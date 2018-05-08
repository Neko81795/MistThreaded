#include "stdafx.h"
#include "CppUnitTest.h"
#include "Math/Vector4.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Vector4_Tests)
	{
	public:
		TEST_METHOD(Constructor)
		{
			Math::Vector4 v1(1, 2);
			Assert::AreEqual(1.0, v1.X);
			Assert::AreEqual(1.0, v1.Y);
			Assert::AreEqual(1.0, v1.Z);
			Assert::AreEqual(2.0, v1.W);

			Math::Vector4 v2(1, 2, 3, 4);
			Assert::AreEqual(1.0, v2.X);
			Assert::AreEqual(2.0, v2.Y);
			Assert::AreEqual(3.0, v2.Z);
			Assert::AreEqual(4.0, v2.W);
		}

		TEST_METHOD(Operator)
		{
			Math::Vector4 v1(1, 2, 3, 4);

			Math::Vector4 v2(1, 2, 3, 4);

			// +
			Math::Vector4 v3 = v1 + v2;
			Assert::AreEqual(2.0, v3.X, L"Vector 4 + failed");
			Assert::AreEqual(4.0, v3.Y, L"Vector 4 + failed");
			Assert::AreEqual(6.0, v3.Z, L"Vector 4 + failed");
			Assert::AreEqual(8.0, v3.W, L"Vector 4 + failed");

			// -
			Math::Vector4 v4 = v1 - v2;
			Assert::AreEqual(0.0, v4.X, L"Vector 4 - failed");
			Assert::AreEqual(0.0, v4.Y, L"Vector 4 - failed");
			Assert::AreEqual(0.0, v4.Z, L"Vector 4 - failed");
			Assert::AreEqual(0.0, v4.W, L"Vector 4 - failed");

			// *
			Math::Vector4 v5 = v1 * 2;
			Assert::AreEqual(2.0, v5.X, L"Vector 4 * failed");
			Assert::AreEqual(4.0, v5.Y, L"Vector 4 * failed");
			Assert::AreEqual(6.0, v5.Z, L"Vector 4 * failed");
			Assert::AreEqual(8.0, v5.W, L"Vector 4 * failed");

			// /
			Math::Vector4 v6 = v1 / 2;
			Assert::AreEqual(0.5, v6.X, L"Vector 4 / failed");
			Assert::AreEqual(1.0, v6.Y, L"Vector 4 / failed");
			Assert::AreEqual(1.5, v6.Z, L"Vector 4 / failed");
			Assert::AreEqual(2.0, v6.W, L"Vector 4 / failed");

		}

		TEST_METHOD(Functions)
		{
			Math::Vector4 v1(1, 2, 3, 0);

			Math::Vector4 v2 = v1.Cross(v1);

			Assert::AreEqual(0.0, v2.X, L"Cross product1 failed");
			Assert::AreEqual(0.0, v2.Y, L"Cross product1 failed");
			Assert::AreEqual(0.0, v2.Z, L"Cross product1 failed");
			Assert::AreEqual(0.0, v2.W, L"Cross product1 failed");

			v1 = Math::Vector4(1, 0, 0, 0);
			v2 = Math::Vector4(0, 1, 0, 0);

			v2 = v1.Cross(v2);
			Assert::AreEqual(0.0, v2.X, L"Cross product2 failed");
			Assert::AreEqual(0.0, v2.Y, L"Cross product2 failed");
			Assert::AreEqual(1.0, v2.Z, L"Cross product2 failed");
			Assert::AreEqual(0.0, v2.W, L"Cross product2 failed");

			Math::Vector4 v3(5, 0, 0, 0);

			Math::Vector4 v4 = v3.Normalized();

			Assert::AreEqual(1.0, v4.X, L"Normalized failed");
			Assert::AreEqual(0.0, v4.Y, L"Normalized failed");
			Assert::AreEqual(0.0, v4.Z, L"Normalized failed");
			Assert::AreEqual(0.0, v4.W, L"Normalized failed");

			v1 = Math::Vector4(1, 2, 3, 0);
			Math::Vector4 v5 = v1.As<1,0,3,2>();
			Assert::AreEqual(2.0, v5.X, L"YXWZ Failed");
			Assert::AreEqual(1.0, v5.Y, L"YXWZ Failed");
			Assert::AreEqual(0.0, v5.Z, L"YXWZ Failed");
			Assert::AreEqual(3.0, v5.W, L"YXWZ Failed");
		}
	};
}