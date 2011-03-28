#include "Node.h"
#include "../Graphics.h"

namespace Monocle
{
	/// NODE
	Node::Node()
		: Entity(), next(NULL), prev(NULL)
	{
	}

	Node::Node(const Vector2 &position)
		: Entity(), next(NULL), prev(NULL)
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

		Graphics::BindTexture(NULL);
		Graphics::SetColor(Color::blue);
		Graphics::RenderLineRect(position.x, position.y, 64, 64);
		if (next)
			Graphics::RenderLine(position, next->position);
	}

	void Node::SetNext(Node *node)
	{
		this->next = node;
		node->prev = this;
	}

	Node *Node::GetNext()
	{
		return next;
	}

	Node *Node::GetPrev()
	{
		return prev;
	}

}