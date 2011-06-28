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

			if (this == pathMesh->GetStartNode())
			{
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
		}

		if (next)
		{
			next->prev = prev;
		}
		
		if (prev)
		{
			prev->next = next;
		}

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
			
			PathMesh *pathMesh = dynamic_cast<PathMesh*>(GetParent());
			if (pathMesh)
			{
				Color color = pathMesh->color;
				color.a = 1;
				if (variant == -1)
				{
					Graphics::SetColor(color * 0.5f);
				}
				else
				{
					Graphics::SetColor(pathMesh->color);
				}
			}


			Vector2 pos = GetWorldPosition();
			Graphics::RenderLineRect(pos.x, pos.y, 64, 64);
			if (next)
			{
				Vector2 nextPos = next->GetWorldPosition();
				Vector2 diff = (nextPos - pos);
				Vector2 halfPos = diff * 0.5f + pos;
				Vector2 dir = diff.GetNormalized();
				Graphics::RenderLine(pos, next->GetWorldPosition());
				Graphics::RenderLine(halfPos, halfPos + dir.GetPerpendicularLeft() * 10.0f - dir * 10.0f);
				Graphics::RenderLine(halfPos, halfPos + dir.GetPerpendicularRight() * 10.0f - dir * 10.0f);
			}
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