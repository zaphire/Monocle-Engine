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
	const int ENTITY_CONTROLPOINT_SIZE = 20;

	class Scene;
	class Collision;
	class Collider;
	class RectangleCollider;
	class CircleCollider;
	class Graphics;
	class Graphic;
	class CollisionData;

	class Entity : public Transform
	{
	public:
		Entity(const Entity &entity);
		Entity();
		~Entity();

		float depth;
		bool isVisible;
		Vector2 followCamera;

		Color color; // <- may move this later (material system?)
		

		// remove from Scene or Entity parent
		void RemoveSelf();

		//The scene that contains the entity
		Scene* scene;
		
		virtual void Destroy();

		//Called by the scene when the entity should update its game logic
		virtual void Update();

		//Called by the scene when the entity should render
		virtual void Render();

		//Called by the scene when the entity is added to that scene
		virtual void Added();

		//Called by the scene when the entity is removed from that scene
		virtual void Removed();

		// from Transform:: used to save/load properties
		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		//Call to check our collider against all entities that have "tag"
		Collider* Collide(const std::string &tag, CollisionData *collisionData=NULL);
		//RectangleCollider *AddRectangleCollider(float width, float height, const Vector2 &offset = Vector2::zero);

		//Tagging API
		void AddTag(const std::string& tag);
		bool HasTag(const std::string& tag);
		void RemoveTag(const std::string& tag);
		const std::string& GetTag(int index);
		int GetNumberOfTags();

		//Layer
		bool IsLayer(int layer);
		int GetLayer();
		void SetLayer(int layer);
		void AdjustLayer(int layerAdjustAmount);

		void SetCollider(Collider *collider);
		void SetGraphic(Graphic *graphic);

		// add or remove entities from list of children
		void Add(Entity *entity);
		void Remove(Entity *entity);
		Entity *GetParent();

		// used by editors
		bool IsPositionInGraphic(const Vector2 &position);
		Entity* GetChildEntityAtPosition(const Vector2 &position);
		Vector2 GetWorldPosition(const Vector2 &position=Vector2::zero);
		Vector2 GetWorldScale(const Vector2 &scale);

		// enqueue destruction of this entity
		//void Die();

		template <class T>
		inline T *GetFirstChildOfType()
		{
			T *t = NULL;
			for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
			{
				t = dynamic_cast<T*>(*i);
				if (t)
				{
					return t;
				}
			}
			return NULL;
		}

		const std::list<Entity*>* GetChildren();

	protected:
		friend class Scene;

		Entity *GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity, float &smallestSqrMag);
		// notes are very simple "messages"
		void SendNoteToScene(const std::string &note);
		// send a note to all entites with tag "tag"
		void SendNote(const std::string &tag, const std::string &note);
		virtual void ReceiveNote(const std::string &tag, const std::string &note);
		
		std::list<Entity*> children;

	private:
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
		
		
		
		std::vector<std::string> tags;
		int layer;

		// is Death enqueued? scene will clean up if so
		//bool willDie;
	};
}