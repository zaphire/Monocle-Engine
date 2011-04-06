#pragma once

#include "Vector2.h"

namespace Monocle
{
	class FileNode;

	class Transform
	{
	public:
		Transform();
		Transform(const Transform &transform);

		virtual void Save(FileNode *fileNode);
		virtual void Load(FileNode *fileNode);

		void LerpTransform(Transform *prev, Transform *next, float percent);

		Vector2 position;
		float rotation;
		Vector2 scale;
	};
}