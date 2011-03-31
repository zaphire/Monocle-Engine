#pragma once

#include "../Entity.h"

namespace Monocle
{
	class Node : public Entity
	{
	public:
		Node();
		Node(const Vector2 &position);
		void Update();
		void Render();
		void Disconnect();
		void TakeOut();

		void SetNext(Node *node);
		void InsertNext(Node *node);

		void Copy(Node *node);

		Node *GetNext();
		Node *GetPrev();

		int variant;

	private:
		Node *next;
		Node *prev;
	};
}

