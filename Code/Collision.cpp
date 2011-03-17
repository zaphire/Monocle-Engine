#include "Collision.h"

#include <stdio.h>

#include "Entity.h"

namespace Monocle
{
	Collision* Collision::instance = NULL;

	Collision::Collision()
	{
		instance = this;
	}

	void Collision::Init()
	{
	}
	
	//OPTION: refactor to add multiple colliders? (consider carefully)
	//FORNOW: limit to one collider per entity (simpler, makes more sense)
	//LATER: rename these to "Set" or something else?

	CircleCollider* Collision::AddCircleCollider(Entity *entity, float radius, Vector2 offset)
	{
		if (entity->GetCollider() == NULL)
		{
			CircleCollider* circleCollider = new CircleCollider(radius, offset);

			RegisterColliderWithEntity((Collider*)circleCollider, entity);

			return circleCollider;
		}

		Debug::Log("ERROR: Cannot add more than one Collider to an Entity.");
		return NULL;
	}

	RectangleCollider* Collision::AddRectangleCollider(Entity *entity, float width, float height, Vector2 offset)
	{
		if (entity->GetCollider() == NULL)
		{
			RectangleCollider *rectangleCollider = new RectangleCollider(width, height, offset);

			RegisterColliderWithEntity((Collider*)rectangleCollider, entity);

			return rectangleCollider;
		}

		Debug::Log("ERROR: Cannot add more than one Collider to an Entity.");
		return NULL;
	}

	void Collision::RegisterColliderWithEntity(Collider *collider, Entity *entity)
	{
		collider->SetEntity(entity);
		entity->SetCollider(collider);
		instance->colliders.push_back(collider);
	}

	void Collision::RemoveCollider(Collider *collider)
	{
		instance->colliders.remove(collider);
	}

	Collider* Collision::Collide(Entity *entity, const std::string &tag)
	{
		Collider* passedCollider = entity->GetCollider();

		if (passedCollider != NULL)
		{
			for (std::list<Collider*>::iterator i = instance->colliders.begin(); i != instance->colliders.end(); ++i)
			{
				Entity* otherEntity = (*i)->GetEntity();
				if (otherEntity != NULL && otherEntity != entity)
				{
					Collider* otherCollider = otherEntity->GetCollider();
					if (otherCollider != NULL && otherEntity->HasTag(tag))
					{
						if (Collider::Collide(passedCollider, otherCollider))
						{
							return otherCollider;
						}
					}
				}
			}
		}

		return NULL;
	}

}