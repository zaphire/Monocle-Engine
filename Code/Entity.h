#pragma once

#include "Vector2.h"
#include "Scene.h"
#include "Color.h"

#include <string>
#include <vector>
#include <list>

namespace Monocle
{
	class Scene;
	class Collision;
	class Collider;
	class RectangleCollider;
	class CircleCollider;
	class Graphics;
	class Graphic;

	class Entity
	{
	public:
		Entity();
		~Entity();

		Vector2 position;
		Vector2 scale;
		float rotation;
		float depth;
		bool isVisible;

		Color color; // <- may move this later (material system?)
		

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

		//Call to check our collider against all entities that have "tag"
		Collider* Collide(const std::string &tag);
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

		// used by editors
		bool IsPositionInGraphic(const Vector2 &position);
		Entity* GetChildEntityAtPosition(const Vector2 &position);
		Vector2 GetWorldPosition(const Vector2 &position);
		Vector2 GetWorldScale(const Vector2 &scale);

		// enqueue destruction of this entity
		//void Die();

	protected:
		friend class Scene;
		// notes are very simple "messages"
		void SendNoteToScene(const std::string &note);
		// send a note to all entites with tag "tag"
		void SendNote(const std::string &tag, const std::string &note);
		virtual void ReceiveNote(const std::string &tag, const std::string &note);

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
		
		std::list<Entity*> children;
		
		std::vector<std::string> tags;
		int layer;

		// is Death enqueued? scene will clean up if so
		//bool willDie;
	};
}