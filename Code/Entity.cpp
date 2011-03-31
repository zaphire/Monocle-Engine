#include <cstdio>
#include "Entity.h"
#include "Collision.h"
#include "Graphics.h"
#include "FileNode.h"

namespace Monocle
{
	Entity::Entity(const Entity &entity)
		: scene(NULL), collider(NULL), graphic(NULL), position(entity.position), scale(entity.scale), rotation(entity.rotation), depth(entity.depth), isVisible(entity.isVisible), color(entity.color), layer(entity.layer), tags(entity.tags)
	{
	}

	Entity::Entity()
		: scene(NULL), collider(NULL), graphic(NULL), layer(0), depth(0.0f), scale(Vector2::one), rotation(0.0f), color(Color::white), parent(NULL), isVisible(true)
		//, willDie(false)
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

	void Entity::Destroy()
	{
		if (collider)
		{
			delete collider;
			collider = NULL;
		}
		if (graphic)
		{
			delete graphic;
			graphic = NULL;
		}

		// clean up children
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Destroy();
			delete (*i);
		}
	}

	void Entity::Update()
	{
		for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Update();
		}
		
		/*
		if (graphic != NULL)
		{
			graphic->Update();
		}
		*/
	}

	void Entity::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position.x, position.y, depth);
		if (rotation != 0.0f)
			Graphics::Rotate(rotation, 0, 0, 1);
		Graphics::Scale(scale);

		if (graphic != NULL)
		{
			Graphics::SetColor(color);
			graphic->Render(this);
		}

		for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Render();
		}

		Graphics::PopMatrix();

		if (Debug::showBounds)
		{
			Graphics::BindTexture(NULL);

			Graphics::PushMatrix();
			Graphics::Translate(position.x, position.y, depth);

			if (Debug::selectedEntity == this)
				Graphics::SetColor(Color::orange);
			else
				Graphics::SetColor(Color(0.9f,0.9f,1.0f,0.8f));

			Graphics::RenderLineRect(0, 0, 20, 20);

			if (Debug::selectedEntity != this)
				Graphics::SetColor(Color(0.0f,0.0f,0.25f,0.8f));

			Graphics::RenderLineRect(0, 0, 15, 15);

			Graphics::PopMatrix();
		}

		
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

	void Entity::AdjustLayer(int layerAdjustAmount)
	{
		this->layer += layerAdjustAmount;
	}

	bool Entity::IsLayer(int layer)
	{
		return this->layer == layer;
	}

	///TODO: enqueue for safety
	// add an entity as a child
	void Entity::Add(Entity *entity)
	{
		entity->parent = this;
		children.push_back(entity);
	}

	///TODO: enqueue for safety
	void Entity::Remove(Entity *entity)
	{
		entity->parent = NULL;
		children.remove(entity);
	}

	void Entity::SetCollider(Collider *collider)
	{
		if (collider == NULL && this->collider != NULL)
		{
			Collision::RemoveCollider(collider);
			collider = NULL;
		}
		else if (this->collider != NULL)
		{
			Debug::Log("Error: Entity already has a collider.");
		}
		else
		{
			this->collider = collider;
			Collision::RegisterColliderWithEntity(collider, this);
		}
	}

	Collider* Entity::GetCollider()
	{
		return collider;
	}

	Collider* Entity::Collide(const std::string &tag)
	{
		return Collision::Collide(this, tag);
	}

	/*
	RectangleCollider* Entity::AddRectangleCollider(float width, float height, const Vector2 &offset)
	{
		return Collision::AddRectangleCollider(this, width, height, offset);
	}
	*/

	void Entity::SendNoteToScene(const std::string &note)
	{
		if (scene)
		{
			scene->ReceiveNote(note);
		}
	}

	void Entity::SendNote(const std::string &tag, const std::string &note)
	{
		if (scene)
		{
			scene->RelayNoteTo(tag, note);
		}
	}

	void Entity::ReceiveNote(const std::string &tag, const std::string &note)
	{

	}

	void Entity::SetGraphic(Graphic *graphic)
	{
		// not sure yet
		/*
		if (graphic == NULL && this->graphic)
		{
			delete this->graphic;
		}
		*/
		
		// not sure if we want this yet, sets our graphic's entity pointer to NULL
		// if we're about to set the graphic pointer to NULL
		if (graphic == NULL && this->graphic != NULL)
		{
			//this->graphic->entity = NULL;
		}

		if (this->graphic != NULL)
		{
			Debug::Log("Error: Entity already has a graphic.");
		}
		else
		{
			this->graphic = graphic;
			//graphic->entity = this;
		}
	}

	Graphic* Entity::GetGraphic()
	{
		return graphic;
	}

	bool Entity::IsPositionInGraphic(const Vector2 &point)
	{
		Graphic* graphic = GetGraphic();
		if (graphic != NULL)
		{
			int width, height;
			graphic->GetWidthHeight(&width, &height);
			Vector2 ul = GetWorldPosition(Vector2( - width * 0.5f, - height * 0.5f));
			Vector2 lr = GetWorldPosition(Vector2( + width * 0.5f, + height * 0.5f));
			printf("p(%d, %d) ul(%d, %d) lr(%d, %d)\n", (int)point.x, (int)point.y, (int)ul.x, (int)ul.y, (int)lr.x, (int)lr.y);
			return (point.x > ul.x && point.x < lr.x && point.y > ul.y && point.y < lr.y);
		}
		return false;
	}

	Vector2 Entity::GetWorldPosition(const Vector2 &position)
	{
		Vector2 returnPos;
		Graphics::PushMatrix();
		Graphics::IdentityMatrix();

		std::list<Entity*> entityChain;
		
		Entity *current = this;
		while (current)
		{
			entityChain.push_back(current);
			current = current->parent;
		}

		for (std::list<Entity*>::iterator i = entityChain.begin(); i != entityChain.end(); ++i)
		{
			Graphics::Translate((*i)->position);
			Graphics::Rotate((*i)->rotation, 0, 0, 1);
			Graphics::Scale((*i)->scale);
		}

		Graphics::Translate(position);

		returnPos = Graphics::GetMatrixPosition();

		Graphics::PopMatrix();

		return returnPos;
	}

	Entity* Entity::GetChildEntityAtPosition(const Vector2 &position)
	{
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			Entity *entity = (*i)->GetChildEntityAtPosition(position);
			if (entity)
				return entity;
		}
		
		if (IsPositionInGraphic(position))
			return this;

		return NULL;
	}

	void Entity::Save(FileNode *fileNode)
	{
		if (position != Vector2::zero)
			fileNode->Write("position", position);
		if (rotation != 0)
			fileNode->Write("rotation", rotation);
		if (scale != Vector2::one)
			fileNode->Write("scale", scale);
		if (layer != 0)
			fileNode->Write("layer", layer);
		if (color != Color::white)
			fileNode->Write("color", color);
	}

	void Entity::Load(FileNode *fileNode)
	{
		fileNode->Read("position", position);
		fileNode->Read("rotation", rotation);
		fileNode->Read("scale", scale);
		int newLayer =0;
		fileNode->Read("layer", newLayer);
		SetLayer(newLayer);
		fileNode->Read("color", color);
	}

	Entity *Entity::GetParent()
	{
		return parent;
	}


	/// TODO: make recursive
	Entity *Entity::GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity, float &smallestSqrMag)
	{
		Entity *nearestChild = NULL;

		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i) != ignoreEntity)
			{
				Vector2 wp = (*i)->GetWorldPosition();
				Vector2 diff = wp - position;
				printf("wp: %f, %f\n", wp.x, wp.y);
				printf("diff: %f, %f\n", diff.x, diff.y);
				if (diff.IsInRange(90))
				{
					float sqrMag = diff.GetSquaredMagnitude();
					if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
					{
						smallestSqrMag = sqrMag;
						nearestChild = (*i);
					}
				}
			}
		}

		return nearestChild;
	}

	const std::list<Entity*>* Entity::GetChildren()
	{
		return &children;
	}

	/*
	void Entity::Die()
	{
		if (scene)
		{
			willDie = true;
			scene->Remove(this);
		}
		else
		{
			Debug::Log("Error: Entity is not in a scene. 'Die' will not function.");
		}
	}
	*/

	/*
	void Entity::CleanUp()
	{
		std::list<Entity*> toKill;
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->willDie)
			{
				toKill.push_back(*i);
			}
			(*i)->CleanUp();
		}

		for (std::list<Entity*>::iterator i = toKill.begin(); i != toKill.end(); ++i)
		{
			children.remove(*i);
		}
	}
	*/
}
