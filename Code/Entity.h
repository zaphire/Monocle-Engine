#pragma once

#include "Vector2.h"
#include "Scene.h"

#include <string>
#include <vector>
#include <list>

namespace Monocle
{
	class Scene;
	class Collider;
	class RectangleCollider;
	class CircleCollider;

	class Entity
	{
	public:
		Entity();
		~Entity();

		Vector2 position;
		float depth;

		//The scene that contains the entity
		Scene* scene;

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
		RectangleCollider *AddRectangleCollider(float width, float height, const Vector2 &offset = Vector2::zero);

		//very simple "message" sending
		void SendMessageToScene(const std::string &message);

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

		// only for use by Collision class
		void SetCollider(Collider *collider);
		Collider* GetCollider();
	private:
		Collider* collider;
		std::vector<std::string> tags;
		int layer;
	};
}