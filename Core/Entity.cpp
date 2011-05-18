#include <cstdio>
#include "Entity.h"
#include "Collision.h"
#include "Graphics.h"
#include "FileNode.h"
#include "MonocleToolkit.h"
#include <sstream>

namespace Monocle
{
	InvokeData::InvokeData(void *me, void (*functionPointer) (void *), float delay)
		: functionPointer(functionPointer), delay(delay), isDone(false), me(me)
	{
	}

	void InvokeData::Update()
	{
		if (!isDone)
		{
			delay -= Monocle::deltaTime;
			if (delay < 0)
			{
				isDone = true;
				functionPointer(me);
			}
		}
	}

	Entity::Entity(const Entity &entity)
		: Transform(entity), isEnabled(true), followCamera(entity.followCamera), scene(NULL), collider(NULL), graphic(NULL), parent(NULL), depth(entity.depth), isVisible(entity.isVisible), color(entity.color), layer(entity.layer)//, tags(entity.tags)
	{
		std::vector<std::string> copyTags = entity.tags;
		for (std::vector<std::string>::iterator i = copyTags.begin(); i != copyTags.end(); ++i)
		{
			AddTag(*i);
		}
	}

	Entity::Entity()
		: Transform(), isEnabled(true), scene(NULL), collider(NULL), graphic(NULL), parent(NULL), layer(0), depth(0.0f), color(Color::white), isVisible(true)
		//, willDie(false)
	{
	}

	Entity::~Entity()
	{
	}

	Entity *Entity::Clone()
	{
		Debug::Log("Entity::Clone()");
		return new Entity(*this);
	}

	void Entity::Added()
	{
	}

	void Entity::Removed()
	{
	}

	void Entity::Destroyed()
	{
		if (collider)
		{
			Collision::RemoveCollider(collider);
			delete collider;
			collider = NULL;
		}
		if (graphic)
		{
			delete graphic;
			graphic = NULL;
		}

		DestroyChildren();

		// clean up invokes
		for (std::list<InvokeData*>::iterator i = invokes.begin(); i != invokes.end(); ++i)
		{
			delete (*i);
		}
		invokes.clear();
	}

	void Entity::DestroyChildren()
	{
		// clean up children
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Destroyed();
			delete (*i);
		}

		children.clear();
	}

	void Entity::Update()
	{
		for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Update();
		}

		// clean up invokes
		for(std::list<InvokeData*>::iterator i = invokes.begin(); i != invokes.end(); ++i)
		{
			(*i)->Update();

			if ((*i)->isDone)
			{
				removeInvokes.push_back(*i);
			}
		}

		for (std::list<InvokeData*>::iterator i = removeInvokes.begin(); i != removeInvokes.end(); ++i)
		{
			delete *i;
			invokes.remove(*i);
		}
	}

	void Entity::RemoveSelf()
	{
		if (parent)
			parent->Remove(this);
		else if (scene)
			scene->Remove(this);
	}

	void Entity::Enable()
	{
		isEnabled = true;
	}

	void Entity::Disable()
	{
		isEnabled = false;
	}

	bool Entity::IsEnabled()
	{
		return isEnabled;
	}

	void Entity::Render()
	{
        const int MIN_LAYER = -100;
        const int MAX_LAYER = 100;
        
        Graphics::PushMatrix();
        
		if (followCamera == Vector2::zero || (Debug::render && Debug::selectedEntity != this && IsDebugLayer()))
			Graphics::Translate(position.x, position.y, depth);
		else
			Graphics::Translate(scene->GetCamera()->position * followCamera + position * (Vector2::one - followCamera));
        
		if (rotation != 0.0f)
			Graphics::Rotate(rotation, 0, 0, 1);
        
		Graphics::Scale(scale);
        
        for (int layer = MAX_LAYER; layer > 0; layer--)
		{
			for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
			{
				if ((*i)->IsLayer(layer) && (*i)->isVisible)
				{
					(*i)->Render();
				}
			}
		}

		if (graphic != NULL)
		{
			Graphics::SetColor(color);
			graphic->Render(this);
		}

		for (int layer = 0; layer >= MIN_LAYER; layer--)
		{
			for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
			{
				if ((*i)->IsLayer(layer) && (*i)->isVisible)
				{
					(*i)->Render();
				}
			}
		}
        
        Graphics::PopMatrix();
		
		if (Debug::showBounds && IsDebugLayer())
		{
			Vector2 offset;
			if (parent)
				offset = Vector2::one * 2;
            
			Graphics::BindTexture(NULL);
            
			Graphics::PushMatrix();
			
			if (followCamera == Vector2::zero || Debug::render)
				Graphics::Translate(position.x, position.y, depth);
			else
				Graphics::Translate(scene->GetCamera()->position * followCamera + position * (Vector2::one - followCamera));
            
			if (Debug::selectedEntity == this)
				Graphics::SetColor(Color::orange);
			else
				Graphics::SetColor(Color(0.9f,0.9f,1.0f,0.8f));
            
			Graphics::RenderLineRect(offset.x, offset.y, ENTITY_CONTROLPOINT_SIZE, ENTITY_CONTROLPOINT_SIZE);
            
			if (Debug::selectedEntity != this)
				Graphics::SetColor(Color(0.0f,0.0f,0.25f,0.8f));
            
			Graphics::RenderLineRect(offset.x, offset.y, ENTITY_CONTROLPOINT_SIZE * 0.75f, ENTITY_CONTROLPOINT_SIZE * 0.75f);
            
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
		if (!HasTag(tag))
		{
			tags.push_back(tag);
			if (scene != NULL)
				scene->EntityAddTag(this, tag);
		}
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

	bool Entity::IsDebugLayer()
	{
		if (parent)
			return parent->IsDebugLayer();

		return layer > Debug::layerMin && layer < Debug::layerMax;
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

	Collider* Entity::Collide(const std::string &tag, CollisionData *collisionData)
	{
		return Collision::Collide(this, tag, collisionData);
	}

	Collider* Entity::CollideAt(const std::string &tag, const Vector2 &atPosition, CollisionData *collisionData)
	{
		Vector2 oldPosition = position;
		position = atPosition;
		Collider *collider = Collision::Collide(this, tag, collisionData);
		position = oldPosition;
		return collider;
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

    /// TODO: write our own matrix functions to replace this stuff
	Vector2 Entity::GetWorldPosition(const Vector2 &position)
	{
		Vector2 returnPos;
		Graphics::PushMatrix();
		Graphics::IdentityMatrix();
        //Graphics::ResolutionMatrix();

		std::list<Entity*> entityChain;
		
		Entity *current = this;
		while (current)
		{
			entityChain.push_back(current);
			current = current->parent;
		}

		for (std::list<Entity*>::reverse_iterator i = entityChain.rbegin(); i != entityChain.rend(); ++i)
		{
			Graphics::Translate(scene->GetCamera()->position * (*i)->followCamera + (*i)->position * (Vector2::one - (*i)->followCamera));
			Graphics::Rotate((*i)->rotation, 0, 0, 1);
			Graphics::Scale((*i)->scale);
		}

		Graphics::Translate(position);

		returnPos = Graphics::GetMatrixPosition();

		Graphics::PopMatrix();

		return returnPos;
	}

	/// TODO: write our own matrix functions to replace this stuff
	Vector2 Entity::GetLocalPosition(const Vector2 &worldPosition)
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

		Graphics::Translate(worldPosition);

		for (std::list<Entity*>::iterator i = entityChain.begin(); i != entityChain.end(); ++i)
		{
			Graphics::Scale(1.0f/(*i)->scale);
			Graphics::Rotate(-(*i)->rotation, 0, 0, 1);
			Graphics::Translate(-(*i)->position);
		}

		

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
		Transform::Save(fileNode);

		if (layer != 0)
			fileNode->Write("layer", layer);
		if (color != Color::white)
			fileNode->Write("color", color);
		if (tags.size() != 0)
		{
			std::ostringstream os;
			for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
			{
				os << (*i) << " ";
			}
			fileNode->Write("tags", os.str());
		}
		if (followCamera != Vector2::zero)
			fileNode->Write("followCamera", followCamera);
	}

	void Entity::Load(FileNode *fileNode)
	{
		Transform::Load(fileNode);

		int newLayer =0;
		fileNode->Read("layer", newLayer);
		SetLayer(newLayer);
		fileNode->Read("color", color);
		std::string tags;
		fileNode->Read("tags", tags);
		if (tags.size() > 0)
		{
			std::string tag;
			std::istringstream is(tags);
			while (is >> tag)
			{
				//printf("loading tag: %s\n", tag.c_str());
				AddTag(tag);
			}
		}
		fileNode->Read("followCamera", followCamera);
	}

	//void Entity::SetParent(Entity *parent)
	//{
	//	this->parent = parent;
	//}

	Entity *Entity::GetParent()
	{
		return parent;
	}

	Scene* Entity::GetScene()
	{
		return scene;
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
				//printf("wp: %f, %f\n", wp.x, wp.y);
				//printf("diff: %f, %f\n", diff.x, diff.y);
				if (diff.IsInRange(ENTITY_CONTROLPOINT_SIZE))
				{
					float sqrMag = diff.GetSquaredMagnitude();
					if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
					{
						smallestSqrMag = sqrMag;
						nearestChild = (*i);
					}
				}
			}

			Entity *newNearestChild = (*i)->GetNearestEntityByControlPoint(position, tag, ignoreEntity, smallestSqrMag);
			if (newNearestChild)
			{
				nearestChild = newNearestChild;
			}
		}

		return nearestChild;
	}

	const std::list<Entity*>* Entity::GetChildren()
	{
		return &children;
	}

	
	void Entity::Invoke(void (*functionPointer)(void*), float delay)
	{
		invokes.push_back(new InvokeData((void*)this, functionPointer, delay));
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
