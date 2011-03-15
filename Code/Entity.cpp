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

	const string& Entity::getTag()
	{
		return tag;
	}

	void Entity::setTag(string tag)
	{
		string oldTag = this->tag;
		this->tag = tag;
		scene->RetagEntity(this, oldTag);
	}
}