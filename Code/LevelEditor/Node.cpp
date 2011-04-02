#include "Node.h"
#include "../Graphics.h"

namespace Monocle
{
	/// NODE

	Node::Node(const Node &node)
		: Entity(node), radius(node.radius), variant(node.variant), next(node.next), prev(node.prev)//, isIgnored(node.isIgnored)
	{
	}

	Node::Node()
		: Entity(), radius(0.0f), next(NULL), prev(NULL), variant(0)//, isIgnored(false)
	{
	}

	Node::Node(const Vector2 &position)
		: Entity(), radius(0.0f), next(NULL), prev(NULL), variant(0)//, isIgnored(false)
	{
		this->position = position;
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
		if (next)
			next->prev = prev;
		if (prev)
			prev->next = next;
	}

	void Node::Render()
	{
		Entity::Render();

		if (Debug::showBounds)
		{
			Graphics::BindTexture(NULL);
			if (variant == -1)
			{
				Graphics::SetColor(Color::grey);
			}
			else
			{
				Graphics::SetColor(Color::blue);
			}
			Graphics::RenderLineRect(position.x, position.y, 64, 64);
			if (next)
				Graphics::RenderLine(position, next->position);
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

	void Node::Copy(Node *node)
	{
		this->scale = node->scale;
		this->variant = node->variant;
		this->prev = node->prev;
		this->next = node->next;
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
	}

	void Node::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("variant", variant);
	}

}