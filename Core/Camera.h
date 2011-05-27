#pragma once

#include "Transform.h"

namespace Monocle
{
	class Camera : public Transform
	{
	public:
		Camera();
		Camera(const Camera& camera);

        //! Applies the position translation, scale, and rotation of this camera to the graphics context.
		void ApplyMatrix();

        //! Sets the layers that this camera can see.
        //! \param backLayer The deepest layer in this camera's vision
        //! \param frontLayer The shallowest layer in this camera's vision
		void SetLayers(int backLayer, int frontLayer);
		//! Returns the deepest layer the camera can see
		int GetBackLayer();
		//! Returns the shallowest layer the camera can see
		int GetFrontLayer();

        //! Whether or not the camera is to be rendered
		bool isVisible;

	private:
		int backLayer, frontLayer;
	};
}
