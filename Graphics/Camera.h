#pragma once
#pragma warning(push, 0)
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Vector4.h"
#pragma warning(pop)

namespace Graphics
{
	class Camera
	{
	public:
		Camera(GraphicsEngine& graphics);

		void UpdateSize();
		void SetPosition(Vector4 position);
		void SetLook(Vector4 look);
		void SetFOV(float fov);
		void SetNearPlane(float n);
		void SetFarPlane(float f);
		const Vector4& GetPosition() const;
		const Vector4& GetLook() const;
		float GetFOV() const;
		float GetNearPlane() const;
		float GetFarPlane() const;

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
		float FOV;
		float Near;
		float Far;

		ConstantBuffer<CameraMatrices> CameraBuffer;
		
		Camera& operator=(const Camera& cam) = delete;

		void CreateCameraViewMatrix();
	};
}
