#include "PathMesh.h"
#include "Node.h"
#include "../Assets.h"
#include "../Graphics.h"
#include "../TextureAsset.h"
#include "../Colliders/PathCollider.h"

namespace Monocle
{
	PathMesh::PathMesh()
		: Entity(), size(32), startNode(NULL), cells(1), texture(NULL), pathCollider(NULL)
	{
		///HACK
		//texture = Assets::RequestTexture("graphics/wallpieces.png");
	}
	
	PathMesh::PathMesh(const std::string &textureFilename, int cells, Node *startNode, int size)
		: Entity(), size(size), cells(cells), pathCollider(NULL)
	{
		//const std::string &textureFilename, 
		texture = Assets::RequestTexture(textureFilename);
		//
		SetStartNode(startNode);
	}

	void PathMesh::MakeCollision()
	{
		if (pathCollider)
			delete pathCollider;

		SetCollider(pathCollider = new PathCollider(startNode));
	}

	void PathMesh::SetStartNode(Node *node)
	{
		nodes.clear();

		this->startNode = node;
		if (pathCollider)
			pathCollider->startNode = node;

		Node *current = this->startNode;
		while (current)
		{
			nodes.push_back(current);
			//textures.push_back(current->tile);
			current = current->GetNext();
		}
	}

	void PathMesh::Render()
	{
		Graphics::SetColor(Color::white);
		//Graphics::SetColor(Color(0,0,1,0.5f));
		Graphics::BindTexture(texture);
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::Rotate(rotation, 0, 0, 1);
		Graphics::Scale(scale);
		if (nodes.size() > 0)
			Graphics::RenderPathMesh(nodes, cells, size);
		Graphics::PopMatrix();

		Entity::Render();

		if (startNode)
			SetStartNode(startNode);
	}

	void PathMesh::Save(FileNode *fileNode)
	{
		Entity::Save(fileNode);

		fileNode->Write("cells", cells);
		fileNode->Write("size", size);
		if (texture)
			fileNode->Write("image", texture->GetName());
	}

	void PathMesh::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("cells", cells);
		fileNode->Read("size", size);
		
		std::string image;
		fileNode->Read("image", image);

		if (texture)
		{
			texture->RemoveReference();
			texture = NULL;
		}

		if (!image.empty())
			texture = Assets::RequestTexture(image);

		startNode = NULL;
		//startNode = GetFirstChildOfType<Node>();
		Node *lastNode = NULL;
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			Node *node = dynamic_cast<Node*>(*i);
			if (node)
			{
				if (!startNode)
					startNode = node;
				if (lastNode)
				{
					lastNode->SetNext(node);
				}
				lastNode = node;
			}
		}

		MakeCollision();
	}
}