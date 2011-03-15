#pragma once

#include "Vector2.h"
#include "Scene.h"
#include <string>

using std::string;

namespace Monocle
{
	class Scene;

	class Entity
	{
	public:
		Entity();
		Entity(string tag);
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

		//Get and set the tag
		const string& GetTag();
		void SetTag(string tag);
	private:
		string tag;
	};
}