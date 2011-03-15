#include "Entity.h"

namespace Monocle
{
	Entity::Entity()
		: tag("")
	{
		scene = NULL;
		layer = 0;
		depth = 0.0f;
	}

	Entity::Entity(string tag)
		: tag(tag)
	{
		scene = NULL;
		layer = 0;
		depth = 0.0f;
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

	const string& Entity::GetTag()
	{
		return tag;
	}

	void Entity::SetTag(string tag)
	{
		string oldTag = this->tag;
		this->tag = tag;
		scene->RetagEntity(this, oldTag);
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
}