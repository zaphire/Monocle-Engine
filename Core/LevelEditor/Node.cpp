#include "Node.h"
#include "../Graphics.h"

///HACK:
#include "PathMesh.h"

namespace Monocle
{
	/// NODE

	Node::Node(const Node &node)
		: Entity(node), variant(node.variant), next(node.next), prev(node.prev)//, isIgnored(node.isIgnored)
	{
	}

	Node::Node()
		: Entity(), next(NULL), prev(NULL), variant(0)//, isIgnored(false)
	{
	}

	Node::Node(const Vector2 &position)
		: Entity(),next(NULL), prev(NULL), variant(0)//, isIgnored(false)
	{
		this->position = position;
	}

	Entity* Node::Clone()
	{
		return new Node(*this);
	}

	void Node::Update()
	{
		Entity::Update();
	}

	void Node::Disconnect()
	{
		if (next)
			next->prev = NULL;
		next = NULL;
		if (prev)
			prev->next = NULL;
		prev = NULL;
	}

	void Node::TakeOut()
	{
		if (GetParent())
		{
			/// HACK:
			PathMesh *pathMesh = dynamic_cast<PathMesh*>(GetParent());
			if (prev)
			{
				pathMesh->SetStartNode(prev);
			}
			else if (next)
			{
				pathMesh->SetStartNode(next);
			}
			else
			{
				pathMesh->SetStartNode(NULL);
			}
		}

		if (next)
			next->prev = prev;
		if (prev)
			prev->next = next;

		prev = NULL;
		next = NULL;
	}

	void Node::Render()
	{
		Entity::Render();

		///HACK:
		//if (Debug::showBounds)
		if (true)
		{
			Graphics::BindTexture(NULL);

			if (variant == -1)
			{
				Graphics::SetColor(Color::grey);
			}
			else if (GetParent() && GetParent()->HasTag("solid"))
			{
				// hack
				Graphics::SetColor(Color::red);
			}
			else
			{
				Graphics::SetColor(Color::blue);
			}

			Vector2 position = GetWorldPosition();
			Graphics::RenderLineRect(position.x, position.y, 64, 64);
			if (next)
				Graphics::RenderLine(position, next->GetWorldPosition());
		}
	}

	void Node::SetNext(Node *node)
	{
		this->next = node;
		node->prev = this;
	}

	void Node::InsertNext(Node *node)
	{
		Node *oldNext = this->next;

		if (oldNext)
			node->SetNext(oldNext);

		this->SetNext(node);
	}

	Node *Node::GetNext()
	{
		return next;
	}

	Node *Node::GetPrev()
	{
		return prev;
	}

	void Node::Save(FileNode *fileNode)
	{
		Entity::Save(fileNode);

		if (variant != 0)
			fileNode->Write("variant", variant);
		/*
		if (radius != 0)
			fileNode->Write("radius", radius);
		*/
	}

	void Node::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("variant", variant);
		//fileNode->Read("radius", radius);
	}

}