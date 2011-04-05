#include "Camera.h"
#include "Graphics.h"

namespace Monocle
{
	Camera::Camera(const Camera& camera) : Transform(camera)
	{

	}

	Camera::Camera() : Transform()
	{


	}

	void Camera::ApplyMatrix()
	{
		Graphics::Scale(scale);
		Graphics::Translate(position * -1.0f);
	}
}