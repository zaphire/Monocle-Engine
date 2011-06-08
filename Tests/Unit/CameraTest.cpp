#include <cpptest/cpptest-assert.h>

#include "CameraTest.h"
#include <Camera.h>

namespace Monocle
{
	namespace Test
	{
		namespace Unit
		{
			Camera::Camera()
			{
				TEST_ADD(Camera::Visibility);
				TEST_ADD(Camera::VisibleLayers);
			}
			
			void Camera::setup()
			{
				camera = new Monocle::Camera();
			}
			
			void Camera::tear_down()
			{
				delete camera;
			}
			
			void Camera::Visibility()
			{
				
			}
			
			void Camera::VisibleLayers()
			{
				camera->SetLayers(10,10);
				TEST_ASSERT(camera->GetBackLayer() == 10);
				TEST_ASSERT(camera->GetFrontLayer() == 10);
				
				camera->SetLayers(2,2);
				TEST_ASSERT(camera->GetBackLayer() == 2);
				TEST_ASSERT(camera->GetFrontLayer() == 2);
			}
		}
	}
}
