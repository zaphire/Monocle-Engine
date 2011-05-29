#include "Transform.h"
#include "FileNode.h"

#include "MonocleToolkit.h"
#include <math.h>
#include <stdio.h>

namespace Monocle
{
	Transform::Transform()
		: position(Vector2::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{
	}

	void Transform::LerpTransform(Transform *prev, Transform *next, float percent)
	{
		position = next->position*percent + prev->position*(1.0f - percent);
		rotation = next->rotation*percent + prev->rotation*(1.0f - percent);
		scale = next->scale*percent + prev->scale*(1.0f-percent);
	}

	void Transform::Save(FileNode *fileNode)
	{
		if (position != Vector2::zero)
			fileNode->Write("position", position);
		if (rotation != 0)
			fileNode->Write("rotation", rotation);
		if (scale != Vector2::one)
			fileNode->Write("scale", scale);
	}

	void Transform::Load(FileNode *fileNode)
	{
		fileNode->Read("position", position);
		fileNode->Read("rotation", rotation);
		fileNode->Read("scale", scale);
	}
    
    Vector2 Transform::GetDirectionVector()
    {
        return Vector2(sin(rotation*pi / 180.0),cos(rotation*pi / 180.0)*-1.0);
    }
}