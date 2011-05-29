#pragma once

#include "Vector2.h"
#include "Entity.h"

namespace Monocle
{
    //! Base class for Editors.
	class Editor : public Entity
	{
	public:
		Editor();
		virtual ~Editor() {}
	};

    //! Additional base class for editors that require the ability to change camera position
	class CameraEditor
	{
	public:
        virtual ~CameraEditor() {}
	protected:
        //! Updates zoom (mousewheel) and position(middle click) of camera  
		void UpdateCamera();
		Vector2 lastWorldMousePosition;
	};
}
