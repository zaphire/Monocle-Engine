#pragma once

#include "Vector2.h"

namespace Monocle
{
	class FileNode;

	//!
	//! \brief Describes a transformation in 2D space, via position (horizontal (x) position, vertical (y) position), rotation (an angle) and scale (horizontal (x) size, vertical (y) size).
	//!
	class Transform
	{
	public:
		Transform();
		Transform(const Transform &transform);

		virtual void Save(FileNode *fileNode);
		virtual void Load(FileNode *fileNode);

		//! Interpolate this Transform from prev and next, based on percent. (0.0 ... 1.0)
		void LerpTransform(Transform *prev, Transform *next, float percent);

		Vector2 position;
		float rotation;
		Vector2 scale;
        
        //! Retrieves a direction vector based on the rotation value.
        Vector2 GetDirectionVector();
	};
}