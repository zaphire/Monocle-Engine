#pragma once

#include "Vector2.h"
#include "Scene.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Monocle
{
	class Scene;

	class Entity
	{
	public:
		Entity();
		~Entity();

		Vector2 position;
		int layer;
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
		void AddTag(const string& tag);
		bool HasTag(const string& tag);
		void RemoveTag(const string& tag);
		const string& GetTag(int index);
		int GetNumberOfTags();
	private:
		vector<string> tags;
	};
}