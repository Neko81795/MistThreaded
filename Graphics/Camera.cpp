#include "Camera.h"
#include <cmath>

namespace Graphics
{
	Camera::Camera(GraphicsEngine & graphics) : Graphics(graphics), CameraBuffer(graphics, ShaderType::Vertex, 0)
	{
		FOV = DirectX::XMConvertToRadians(70);
		Near = 0.01;
		Far = 1000;
		Look = Vector4(0, 0, -1, 0);

		CreateCameraViewMatrix();
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>(FOV)
			, graphics.Win.Width / static_cast<float>(graphics.Win.Height)
			, static_cast<float>(Near)
			, static_cast<float>(Far));
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::UpdateSize()
	{
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>(FOV)
			, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height)
			, static_cast<float>(Near)
			, static_cast<float>(Far));
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
		Look = look.Normalized();
		CreateCameraViewMatrix();
		auto& mat = CameraBuffer.GetDataForWrite();
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetFOV(double fov)
	{
		FOV = fov;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>(FOV)
			, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height)
			, static_cast<float>(Near)
			, static_cast<float>(Far));
		mat.ViewProjection = mat.View * Projection;
	}
	void Camera::SetNearPlane(double n)
	{
		Near = n;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>(FOV)
			, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height)
			, static_cast<float>(Near)
			, static_cast<float>(Far));
		mat.ViewProjection = mat.View * Projection;
	}

	void Camera::SetFarPlane(double f)
	{
		Far = f;
		auto& mat = CameraBuffer.GetDataForWrite();
		Projection = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>(FOV)
			, Graphics.Win.Width / static_cast<float>(Graphics.Win.Height)
			, static_cast<float>(Near)
			, static_cast<float>(Far));
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

	double Camera::GetFOV() const
	{
		return FOV;
	}

	double Camera::GetNearPlane() const
	{
		return Near;
	}

	double Camera::GetFarPlane() const
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
		Vector4 right = Look.Cross(up).Normalized();
		up = right.Cross(Look).Normalized();

		/*mat.View = DirectX::XMMatrixSet(
			right.X, up.X, Look.X, Position.X,
			right.Y, up.Y, Look.Y, Position.Y,
			right.Z, up.Z, Look.Z, Position.Z,
			0, 0, 0, 1
		);*/

		mat.View = DirectX::XMMatrixSet(
			static_cast<float>(right.X), static_cast<float>(right.Y), static_cast<float>(-right.Z), 0,
			static_cast<float>(up.X), static_cast<float>(up.Y), static_cast<float>(-up.Z), 0,
			static_cast<float>(Look.X), static_cast<float>(Look.Y), static_cast<float>(-Look.Z), 0,
			static_cast<float>(Position.X), static_cast<float>(Position.Y), static_cast<float>(Position.Z), 1
		);

		mat.View = DirectX::XMMatrixInverse(
			nullptr,
			mat.View);
	}
}
