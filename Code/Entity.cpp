#include "Entity.h"
#include "Collision.h"

namespace Monocle
{
	Entity::Entity()
		: scene(NULL), collider(NULL), layer(0), depth(0.0f)
	{
	}

	Entity::~Entity()
	{

	}

	void Entity::Added()
	{

	}

	void Entity::Removed()
	{

	}

	void Entity::Update()
	{

	}

	void Entity::Render()
	{

	}

	const std::string& Entity::GetTag(int index)
	{
#ifdef DEBUG
		//Error: If the tag index to get is out of bounds
		if (index >= tags.size())
			Debug::Log("ERROR: Tag index out of bounds.");
#endif
		return tags[index];
	}

	void Entity::AddTag(const std::string& tag)
	{
#ifdef DEBUG
		//Error: If the entity already has that tag
		if (HasTag(tag))
			Debug::Log("ERROR: Duplicate tag added to entity.");
#endif
		tags.push_back(tag);
		if (scene != NULL)
			scene->EntityAddTag(this, tag);
	}

	void Entity::RemoveTag(const std::string& tag)
	{
#ifdef DEBUG
		//Error: If the entity doesn't have that tag
		if (!HasTag(tag))
			Debug::Log("ERROR: Removing tag from an entity that doesn't have that tag.");
#endif
		for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
		{
			if ((*i).compare(tag) == 0)
			{
				tags.erase(i);
				break;
			}
		}
		if (scene != NULL)
			scene->EntityRemoveTag(this, tag);
	}

	bool Entity::HasTag(const std::string& tag)
	{
		for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
		{
			if ((*i).compare(tag) == 0)
				return true;
		}

		return false;
	}

	int Entity::GetNumberOfTags()
	{
		return static_cast<int>(tags.size());
	}

	int Entity::GetLayer()
	{
		return layer;
	}

	void Entity::SetLayer(int layer)
	{
		this->layer = layer;

		//TODO: get scene to re-sort me
	}

	bool Entity::IsLayer(int layer)
	{
		return this->layer == layer;
	}

	void Entity::SetCollider(Collider *collider)
	{
		this->collider = collider;
	}

	Collider* Entity::GetCollider()
	{
		return collider;
	}

	Collider* Entity::Collide(const std::string &tag)
	{
		return Collision::Collide(this, tag);
	}

	RectangleCollider* Entity::AddRectangleCollider(float width, float height, const Vector2 &offset)
	{
		return Collision::AddRectangleCollider(this, width, height, offset);
	}

	void Entity::SendMessageToScene(const std::string &message)
	{
		if (scene)
		{
			scene->ReceiveMessage(message);
		}
	}
}
