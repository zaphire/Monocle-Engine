#pragma once

#include "../Entity.h"

namespace Monocle
{
	class TextureAsset;
	class Node;
	class PathCollider;

	class PathMesh : public Entity
	{
	public:
		PathMesh();
		PathMesh(const std::string &textureFilename, int cells, Node *startNode, int size);
		void SetStartNode(Node *node);
		void Render();
		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);
		void MakeCollision(float radius);
		
		int cells;
		bool flipX, flipY;

	private:
		//std::list<Node*> nodes;
		TextureAsset *texture;
		int size;
		Node *startNode;
		std::vector<Node*> nodes;
		PathCollider *pathCollider;
	};
}
