#include "Camera.h"
#include "Graphics.h"

namespace Monocle
{

	Viewport::Viewport()
	{
		x = y = 0.0f;
		width = height = 1.0f;
	}

	Viewport::Viewport(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{
	}

	void Viewport::Set(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}


	Camera::Camera(const Camera& camera)
		: Transform(camera), isVisible(camera.isVisible), frontLayer(camera.frontLayer), backLayer(camera.backLayer)
	{

	}

	Camera::Camera()
		: Transform(), isVisible(true), frontLayer(-100), backLayer(100)
	{
		position = Graphics::GetScreenCenter();
	}

	void Camera::ApplyMatrix()
	{
		//Graphics::IdentityMatrix();
		Graphics::DefaultMatrix();
		Graphics::Scale(scale);
		Graphics::Translate(position * -1.0f);
		Graphics::Rotate(rotation, 0, 0, 1);
	}

	void Camera::SetLayers(int backLayer, int frontLayer)
	{
		this->frontLayer = frontLayer;
		this->backLayer = backLayer;
	}

	int Camera::GetBackLayer()
	{
		return backLayer;
	}

	int Camera::GetFrontLayer()
	{
		return frontLayer;
	}

	//void Camera::SetActive(bool on)
	//{
	//	isActive = on;
	//}

	//bool Camera::IsActive()
	//{
	//	return isActive;
	//}
}