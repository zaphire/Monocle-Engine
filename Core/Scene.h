#pragma once

#include "Debug.h"
#include <list>
#include <map>
#include <string>
#include <vector>
#include "Camera.h"

///TODO: Replace with xml wrapper
class TiXmlElement;

namespace Monocle
{
	class Game;
	class Entity;
	

	//enum SearchType
	//{
	//	SEARCH_TOP = 0,
	//	SEARCH_RECURSIVE
	//};


	//! Base class that describes a scene in a Game.
	/*!
	Scenes may be levels, title screens, etc.
	Start by extending the Begin function and adding new Entity objects using the Add function.
	If an Entity is added to a Scene using the Add function, that Entity will be automatically deleted.
	*/
	class Scene
	{
	public:
		Scene();
		~Scene();

		// render?
		bool isVisible;
		// update?
		bool isPaused;

		//! Called when this scene becomes the currently active scene
		virtual void Begin();

		//! Called when this scene is ended and a different one becomes the active scene
		virtual void End();

		//! Called by the main game loop every time the scene should update game logic
		virtual void Update();

		//! Called by the main game loop every time the scene should render
		virtual void Render();

		//! create a new entity of type T and add it to the scene
		template<class T>
		T* Create(Entity *parent=NULL)
		{
			T *t = new T();
			Add(t);
			if (parent)
				t->SetParent(parent);
			return t;
		}

		//! Add an entity to the scene
		void Add(Entity* entity);

		//! Remove an entity from the scene
		void Remove(Entity* entity);

		//! Remove all entities in the scene
		void RemoveAll();

		//! Called by an entity when its tag has changed; the scene must reposition that entity in the tag map
		void EntityAddTag(Entity* entity, const std::string& tag);
		void EntityRemoveTag(Entity* entity, const std::string& tag);

		//Tag API
		Entity* GetFirstTag(const std::string& tag);
		std::list<Entity*>* GetAllTag(const std::string& tag);

		int GetAmountTag(const std::string& tag);

		//Entity *GetFirstEntity();
		//Entity *GetNextEntity();

		Entity* GetNearestEntity(const Vector2 &position, Entity *ignoreEntity=NULL);
		//Entity* GetNearestEntityContaining(const Vector2 &position, Entity *ignoreEntity=NULL);
		Entity* GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity=NULL);

		Entity* GetEntityAtPosition(const Vector2 &position);

		Entity* GetFirstEntityWithTag(const std::string &tag);

		const std::list<Entity*>* GetEntities();

		//! Add a new camera
		static void AddCamera(Camera *camera);
		//! Get camera by index number
		static Camera *GetCamera(int cameraIndex=0);
		//! Return the camera that is currently being used to render, or NULL
		//static Camera *GetActiveCamera();

		static Camera *GetMainCamera();
		static void SetMainCamera(Camera *camera);

		///TODO: replace TiXml with wrapper
		virtual Entity *CreateEntity(const std::string &entityTypeName);
		virtual void SaveEntities(TiXmlElement *element);
		virtual void LoadEntities(TiXmlElement *element);
		virtual void SaveLevel(FileNode *fileNode);
		virtual void LoadLevel(FileNode *fileNode);

	protected:
		// not sure if we need to pass scene or not yet
		// or if we'll use this later
		//void SendNoteToGame(const std::string &note);

		friend class Entity;
		virtual void ReceiveNote(const std::string &note);

		friend class Level;
		//Resolves all entities to be added or removed
		void ResolveEntityChanges();


	private:
		static Scene *instance;

		friend class Game;
		
		// scene has a game pointer
		// so that it can request scene switches
		Game *game;

		void RelayNoteTo(const std::string &tag, const std::string &note);

		//Holds all the entities currently in the scene
		std::list<Entity*> entities;

		//All the entities that will be added at the end of the frame
		std::list<Entity*> toAdd;

		//All the entities that will be removed at the end of the frame
		std::list<Entity*> toRemove;

		//The map of entities sorted by tag
		std::map<std::string, std::list<Entity*> > tagMap;



		// for GetFirstEntity and GetNextEntity
		//std::list<Entity*>::iterator entityIterator;

		std::list<Camera*> cameras;
		void DestroyAllCameras();

		Camera *activeCamera;
		Camera *mainCamera;
	};
}
