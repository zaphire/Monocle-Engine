#pragma once

#include "Vector2.h"
#include "Scene.h"
#include <string>
#include <vector>

namespace Monocle
{
	class Scene;

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

		//Tagging API
		void AddTag(const std::string& tag);
		bool HasTag(const std::string& tag);
		void RemoveTag(const std::string& tag);
		const std::string& GetTag(int index);
		int GetNumberOfTags();

		bool IsLayer(int layer);
		int GetLayer();
		void SetLayer(int layer);
	private:
		std::vector<std::string> tags;
		int layer;
	};
}