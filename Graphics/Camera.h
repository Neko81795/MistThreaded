#pragma once
#pragma warning(push, 0)
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Math/Vector4.h"
#pragma warning(pop)

using Math::Vector4;

namespace Graphics
{
	class Camera
	{
	public:
		Camera(GraphicsEngine& graphics);

		void UpdateSize();
		void SetPosition(Vector4 position);
		void SetLook(Vector4 look);
		void SetFOV(double fov);
		void SetNearPlane(double n);
		void SetFarPlane(double f);
		const Vector4& GetPosition() const;
		const Vector4& GetLook() const;
		double GetFOV() const;
		double GetNearPlane() const;
		double GetFarPlane() const;

		void Use();

	private:
		struct CameraMatrices
		{
			DirectX::XMMATRIX View;
			DirectX::XMMATRIX ViewProjection;
		};
		GraphicsEngine& Graphics;
		Vector4 Position;
		Vector4 Look;
		DirectX::XMMATRIX Projection;
		double FOV;
		double Near;
		double Far;

		ConstantBuffer<CameraMatrices> CameraBuffer;
		
		Camera& operator=(const Camera& cam) = delete;

		void CreateCameraViewMatrix();
	};
}
