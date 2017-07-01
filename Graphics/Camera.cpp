#include "Camera.h"
#include <cmath>

namespace Graphics
{
	Camera::Camera(GraphicsEngine & graphics) : Graphics(graphics), CameraBuffer(graphics, ShaderType::Vertex, 0)
	{
		FOV = DirectX::XMConvertToRadians(70);
		Near = 0.01f;
		Far = 1000;
		Look = Vector4(0, 0, -1, 0);

		CreateCameraViewMatrix();
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(FOV, graphics.Win.Width / static_cast<float>(graphics.Win.Height), Near, Far);
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::UpdateSize()
	{
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(FOV, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height), Near, Far);
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetPosition(Vector4 position)
	{
		Position = position;
		CreateCameraViewMatrix();
		auto& mat = CameraBuffer.GetDataForWrite();
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetLook(Vector4 look)
	{
		Look = look.Normalize();
		CreateCameraViewMatrix();
		auto& mat = CameraBuffer.GetDataForWrite();
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetFOV(float fov)
	{
		FOV = fov;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(FOV, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height), Near, Far);
		mat.ViewProjection = mat.View * Projection;
	}
	void Camera::SetNearPlane(float n)
	{
		Near = n;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(FOV, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height), Near, Far);
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetFarPlane(float f)
	{
		Far = f;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(FOV, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height), Near, Far);
		mat.ViewProjection = mat.View * Projection;
	}

	const Vector4 & Camera::GetPosition() const
	{
		return Position;
	}

	const Vector4 & Camera::GetLook() const
	{
		return Look;
	}

	float Camera::GetFOV() const
	{
		return FOV;
	}

	float Camera::GetNearPlane() const
	{
		return Near;
	}

	float Camera::GetFarPlane() const
	{
		return Far;
	}

	void Camera::Use()
	{
		CameraBuffer.Use();
	}

	void Camera::CreateCameraViewMatrix()
	{
		auto& mat = CameraBuffer.GetDataForWrite();

		Vector4 up(0, 1, 0, 0);
		Vector4 right = Look.Cross(up).Normalize();
		up = right.Cross(Look).Normalize();
		
		/*mat.View = DirectX::XMMatrixSet(
			right.X, up.X, Look.X, Position.X,
			right.Y, up.Y, Look.Y, Position.Y,
			right.Z, up.Z, Look.Z, Position.Z,
			0, 0, 0, 1
		);*/

		mat.View = DirectX::XMMatrixSet(
			right.X, right.Y, -right.Z, 0,
			up.X, up.Y, -up.Z, 0,
			Look.X, Look.Y, -Look.Z, 0,
			Position.X, Position.Y, Position.Z, 1
		);

		mat.View = DirectX::XMMatrixInverse(
			nullptr,
			mat.View);
	}
}
