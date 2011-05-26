#include "PathMesh.h"
#include "Node.h"
#include "../Assets.h"
#include "../Graphics.h"
#include "../TextureAsset.h"
#include "../Colliders/PathCollider.h"

namespace Monocle
{
	PathMesh::PathMesh()
		: Entity(), size(0.0f), startNode(NULL), cells(1), texture(NULL), pathCollider(NULL), flipX(false), flipY(false)
	{
		///HACK
		//texture = Assets::RequestTexture("graphics/wallpieces.png");
	}
	
	PathMesh::PathMesh(const std::string &textureFilename, int cells, Node *startNode, int size)
		: Entity(), size(size), cells(cells), pathCollider(NULL), flipX(false), flipY(false)
	{
		//const std::string &textureFilename, 
		texture = Assets::RequestTexture(textureFilename);
		//
		SetStartNode(startNode);
	}

	void PathMesh::MakeCollision(float radius)
	{
		if (pathCollider)
			delete pathCollider;

		SetCollider(pathCollider = new PathCollider(startNode, radius));
	}

	//void PathMesh::Adopted(Entity *entity)
	//{
	//	if (!startNode)
	//	{
	//		startNode = dynamic_cast<Node*>(entity);
	//	}
	//}

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
		if (texture != NULL || size != 0)
		{
			Graphics::SetColor(Color::white);
			//Graphics::SetColor(Color(0,0,1,0.5f));
			Graphics::BindTexture(texture);
			Graphics::PushMatrix();

			if (followCamera == Vector2::zero || (Debug::render && Debug::selectedEntity != this))
				Graphics::Translate(position.x, position.y, depth);
			else
				Graphics::Translate(scene->GetCamera()->position * followCamera + position * (Vector2::one - followCamera));

			Graphics::Rotate(rotation, 0, 0, 1);
			Graphics::Scale(scale);

			if (nodes.size() > 0)
				Graphics::RenderPathMesh(nodes, cells, size, flipX, flipY);

			Graphics::PopMatrix();

		}
		Entity::Render();

		// HACK: temporary
		// updates current list of nodes
		// should only do this when nodes change
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
		if (pathCollider && pathCollider->radius != 0.0f)
			fileNode->Write("radius", pathCollider->radius);

		if (flipX)
			fileNode->Write("flipX", flipX);
		if (flipY)
			fileNode->Write("flipY", flipY);

		Node *curNode = startNode;
		while (curNode)
		{
			fileNode->SaveChildNode("Node", curNode);
			curNode = curNode->GetNext();
		}
	}

	void PathMesh::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("cells", cells);
		fileNode->Read("size", size);
		
		std::string image;
		fileNode->Read("image", image);

		float radius = 0.0f;
		fileNode->Read("radius", radius);

		fileNode->Read("flipX", flipX);
		fileNode->Read("flipY", flipY);

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
		Node *firstNode = NULL;

		FileNode *curNode = fileNode->FirstChildNode("Node");
		while (curNode)
		{
			Node *newNode = scene->Create<Node>();
			newNode->Load(curNode);
			
			if (firstNode == NULL)
				firstNode = newNode;
			if (lastNode)
				lastNode->SetNext(newNode);
			lastNode = newNode;

			newNode->SetParent(this);

			curNode = fileNode->NextChildNode("Node");
		}

		startNode = firstNode;

		//for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		//{
		//	Node *node = dynamic_cast<Node*>(*i);
		//	if (node)
		//	{
		//		if (!startNode)
		//			startNode = node;
		//		if (lastNode)
		//		{
		//			lastNode->SetNext(node);
		//		}
		//		lastNode = node;
		//	}
		//}

		MakeCollision(radius);
	}
}