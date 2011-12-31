#pragma once

#include "Vector2.h"
#include "Scene.h"
#include "Color.h"
#include "FileNode.h"
#include "Transform.h"

#include <string>
#include <vector>
#include <list>

namespace Monocle
{
	const float ENTITY_CONTROLPOINT_SIZE = 20.0f;

	class Scene;
	class Collision;
	class Collider;
	class RectangleCollider;
	class CircleCollider;
	class Graphics;
	class Graphic;
	class CollisionData;
    class Camera;

	class InvokeData
	{
	public:
		InvokeData(void *me, void (*functionPointer)(void *), float delay);
		void Update();

		void (*functionPointer) (void *);
		float delay;
		bool isDone;
		void *me;
	};


	class EntityTagData
	{
	public:
		EntityTagData();
		EntityTagData(const std::string &name, bool save);
		EntityTagData(const EntityTagData &);

		std::string name;
		bool save;
	};

	typedef std::vector<EntityTagData> EntityTags;

	//! \brief An object that can Update, Render and be derived to perform other actions.
	//!
	//! Examples of what you might use entities for: players, enemies, items, menus.
	//!
	//! You would do this by deriving a new type, e.g.:
	//!
	//!		class Player : public Entity
	//! 
	//! Entities must be Added to a Scene using Scene::Add. Otherwise they will do nothing.
	//! 
	//! e.g.: in Scene::Begin()...
	//!
	//!		Add(new Player)
	//!
	class Entity : public Transform
	{
	public:
		Entity(const Entity &entity);
		Entity();
		virtual ~Entity();
		virtual Entity *Clone();

		//! Enable this object. Set isEnabled to true. Each derived Entity may decide how to handle isEnabled.
		virtual void Enable();
		//! Disable this object. Set isEnabled to false. Each derived Entity may decide how to handle isEnabled.
		virtual void Disable();
		//! \return isEnabled Is this Entity enabled?
		bool IsEnabled();

		//! Remove from Scene or Entity parent. May cause our deletion.
		void RemoveSelf();
	
		//! Called by the scene when the entity should update its game logic
		virtual void Update();

		//! Called by the scene when the entity should render
		virtual void Render();

		//!  from Transform:: used to save/load properties
		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		//! Called by the scene when the entity is added to that scene
		virtual void Added();
		//! Called by the scene when the entity is removed from that scene
		virtual void Removed();
		//! Called when Entity is destroyed
		virtual void Destroyed();
        
        //! Called to determine if the entity should be drawn
        virtual bool IsOnCamera( Camera *camera );

		//! Check our collider against all entities that have "tag"
		Collider* Collide(const std::string &tag, CollisionData *collisionData=NULL);
		//! Check our collider against all entities that have "tag" - warping us to atPosition first, then back to our original position after
		Collider* CollideAt(const std::string &tag, const Vector2& atPosition, CollisionData *collisionData=NULL);
		//! Do a circle collision
		Collider *CollideWith(Collider *collider, const std::string &tag, CollisionData *collisionData=NULL);

		//! Associates this entity with the given tag
		void AddTag(const std::string& tag, bool save=false);
		//! Checks whether this entity is associated with a given tag
		bool HasTag(const std::string& tag);
		//! Removes a tag from this Entity
		void RemoveTag(const std::string& tag);
		//! Gets the tag at the given offset from the list of tags
		const std::string& GetTag(int index);
		//! Gets the number of tags associated with this entity
		int GetNumberOfTags();

		//! Checks if this entity is on the given layer
		bool IsLayer(int layer);
		//! Gets the layer that this entity is on
		//! \return our layer number
		int GetLayer();
		//! Moves the entity to the given layer
		void SetLayer(int layer);
		//! Moves the entity to the layer determined by the given offset, relative to the layer it
		//! is currently on
		void AdjustLayer(int layerAdjustAmount);

		//! is our layer number in the debug render range?
		bool IsDebugLayer();

		void SetCollider(Collider *collider);
		void SetGraphic(Graphic *graphic);

		//! set parent entity
		void SetParent(Entity *parent);
		//! return pointer to parent entity
		Entity *GetParent();
		//! return pointer to the Scene we are currently in
		Scene *GetScene();

		// used by editors
		bool IsPositionInGraphic(const Vector2 &position);
		
		Vector2 GetWorldPosition(const Vector2 &position=Vector2::zero);
		Vector2 GetWorldScale(const Vector2 &scale);
		Vector2 GetLocalPosition(const Vector2 &worldPosition);
		void Invoke(void (*functionPointer)(void*), float delay);

		float depth;
		bool isVisible;
		Vector2 followCamera;

		Color color;

	protected:
		//void DestroyChildren();

		friend class Scene;

		//! The scene that contains the entity
		Scene* scene;

		bool isEnabled;

		void ApplyMatrix();
		void MatrixChain();

	private:
		int id;

		Entity *parent;

		// only for use by Collision class
		friend class Collision;
		Collider* GetCollider();
		Collider* collider;

		// only for use by graphics
		friend class Graphics;
		Graphic* GetGraphic();
		Graphic* graphic;

		// only for use by scene
		//friend class Scene;
		
		std::vector<EntityTagData> tags;
		int layer;

		std::list<InvokeData*> invokes;
		std::list<InvokeData*> removeInvokes;

        Vector2 cachedWorldPosition;
        Vector2 lastPositionWhenCached;
	public:
		//Entity* GetChildEntityAtPosition(const Vector2 &position);
		//template <class T>
		//inline T *GetFirstChildOfType()
		//{
		//	T *t = NULL;
		//	for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		//	{
		//		t = dynamic_cast<T*>(*i);
		//		if (t)
		//		{
		//			return t;
		//		}
		//	}
		//	return NULL;
		//}

		//const std::list<Entity*>* GetChildren();
	};
}
