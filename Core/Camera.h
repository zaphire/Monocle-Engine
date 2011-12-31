#pragma once

#include "Transform.h"

namespace Monocle
{
	class Viewport
	{
	public:
		Viewport();
		Viewport(float x, float y, float width, float height);

		void Set(float x, float y, float width, float height);

		float x, y, width, height;
	};

	//! \brief Manages the details of a view through which items in a scene are rendered.
	//! Cameras are the eyes of the engine, their position, rotation, and depth of vision determine what is rendered
	//! onto the screen.
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

		Viewport viewport;

	private:
		int backLayer, frontLayer;
	};
}
