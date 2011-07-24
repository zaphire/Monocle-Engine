#include "Scene.h"
#include "Entity.h"
#include "Graphics.h"
#include "MonocleToolkit.h"
#include "Platform.h"

namespace Monocle
{
	Scene *Scene::instance = NULL;

	Scene::Scene()
		: isVisible(true), isPaused(false), activeCamera(NULL), mainCamera(NULL)
	{
		instance = this;
		Camera *camera = new Camera();
		camera->position = Graphics::GetScreenCenter();
		AddCamera(camera);
		SetMainCamera(camera);
	}

	Scene::~Scene()
	{
		RemoveAll();
		ResolveEntityChanges();
		DestroyAllCameras();
	}

	void Scene::Begin()
	{

	}

	void Scene::End()
	{
		RemoveAll();
		//Resolve adds and removes
		ResolveEntityChanges();
	}

	void Scene::Update()
	{
		if (!isPaused)
		{
			//Update all the entities
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				(*i)->Update();
			}
		}
		else
		{
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				if (!(*i)->IsDebugLayer())
				{
					(*i)->Update();
				}
			}
		}

		//Resolve adds and removes
		ResolveEntityChanges();
	}

	void Scene::Render()
	{
		if (isVisible)
		{
			//Render all the entities
			// sort by layer?

			//printf("\n\n****\n");


			for (std::list<Camera*>::iterator camera = cameras.begin(); camera != cameras.end(); ++camera)
			{
				activeCamera = *camera;

				Graphics::Viewport(activeCamera->viewport.x * Platform::GetWidth(), activeCamera->viewport.y * Platform::GetHeight(), Platform::GetWidth() * activeCamera->viewport.width, Platform::GetHeight() * activeCamera->viewport.height);
				//Graphics::Viewport(viewport.x * Platform::GetWidth(), viewport.y * Platform::GetHeight(), Platform::GetWidth() * viewport.width, Platform::GetHeight() * viewport.height);

				if (activeCamera->isVisible)
				{
					// set viewport

					activeCamera->ApplyMatrix();

					///HACK: optimize later so we don't run through all the layers
					// TODO sort entities into layer buckets? or one big sorted list?

					for (int layer = activeCamera->GetBackLayer(); layer >= activeCamera->GetFrontLayer(); layer--)
					{
						for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
						{
							if ((*i)->isVisible && (*i)->IsLayer(layer))
							{
								(*i)->Render();
							}
						}
					}
				}
			}

			activeCamera = NULL;
		}
	}

	void Scene::Add(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity is already in the scene
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Adding an entity to the scene that is already in the scene.");
				break;
			}
		}

		//Error: If the entity is already marked to be added
		for (std::list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Adding an entity to the scene that is already flagged to be added at the end of the frame.");
				break;
			}
		}
#endif
		if (entity->scene != NULL)
		{
			Debug::Log("ERROR: Trying to add an Entity to a Scene, but Entity is already in a Scene");
		}
		else
		{
			entity->scene = this;
			toAdd.push_back(entity);
		}
	}

	void Scene::Remove(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity isn't in the entity list
		bool in = false;
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) == entity)
			{
				in = true;
				break;
			}
		}
		if (!in)
			Debug::Log("ERROR: Removing an entity from the scene that isn't in the scene.");

		//Error: If the entity is already marked to be removed
		for (std::list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Removing an entity from the scene that is already flagged for removal at the end of the frame.");
				break;
			}
		}
#endif

		toRemove.push_back(entity);
	}

	void Scene::RemoveAll()
	{
		toRemove.clear();
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			toRemove.push_back(*i);
	}

	void Scene::ResolveEntityChanges()
	{
        // Remove any duplicates...
        toRemove.sort();
        toRemove.unique();
        
		//Resolve removes
		for (std::list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
		{
			(*i)->Destroyed();

			entities.remove(*i);

			//If the tag is set, remove the entity from the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityRemoveTag(*i, (*i)->GetTag(j));

			(*i)->scene = NULL;
			(*i)->Removed();

			delete (*i);
		}
		toRemove.clear();

		//Resolve adds
		for (std::list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
		{
			entities.push_back(*i);

			//If the tag is set, add the entity to the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityAddTag(*i, (*i)->GetTag(j));

			(*i)->scene = this;
			(*i)->Added();
		}
		toAdd.clear();

		//TODO: Sort the entity list based on layer
	}

	void Scene::SaveLevel(FileNode *fileNode)
	{
	}

	void Scene::LoadLevel(FileNode *fileNode)
	{
	}

	void Scene::AddCamera(Camera *camera)
	{
		instance->cameras.push_back(camera);
	}

	Camera *Scene::GetCamera(int cameraIndex)
	{
		int c = 0;
		for (std::list<Camera*>::iterator i = instance->cameras.begin(); i != instance->cameras.end(); i++)
		{
			if (c == cameraIndex)
				return *i;
		}
		return NULL;
	}

	Camera *Scene::GetActiveCamera()
	{
		return instance->activeCamera;
	}

	Camera *Scene::GetMainCamera()
	{
		return instance->mainCamera;
	}
	
	void Scene::SetMainCamera(Camera *camera)
	{
		instance->mainCamera = camera;
	}

	void Scene::DestroyAllCameras()
	{
		for (std::list<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++)
		{
			delete *i;
		}
		cameras.clear();
	}

	void Scene::EntityAddTag(Entity* entity, const std::string& tag)
	{
		tagMap[tag].push_back(entity);
	}

	void Scene::EntityRemoveTag(Entity* entity, const std::string& tag)
	{
		tagMap[tag].remove(entity);
	}

	Entity *Scene::CreateEntity(const std::string &entityTypeName)
	{
		return NULL;
	}

	void Scene::SaveEntities(TiXmlElement *element)
	{
	}

	void Scene::LoadEntities(TiXmlElement *element)
	{
	}

	Entity* Scene::GetFirstEntityWithTag(const std::string &tag)
	{
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i)->HasTag(tag))
			{
				return (*i);
			}
		}
		return NULL;
	}

	Entity* Scene::GetNearestEntity(const Vector2 &position, Entity *ignoreEntity)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) != ignoreEntity)
			{
				Vector2 diff = (*i)->position - position;
				float sqrMag = diff.GetSquaredMagnitude();
				if (smallestSqrMag <= -1 || sqrMag < smallestSqrMag)
				{
					nearestEntity = (*i);
				}
			}
		}

		return nearestEntity;
	}

	Entity* Scene::GetNearestEntityWithTag(const Vector2 &position, const std::string &tag)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		for (std::list<Entity*>::iterator i = tagMap[tag].begin(); i != tagMap[tag].end(); ++i)
		{
			Vector2 diff = (*i)->position - position;
			float sqrMag = diff.GetSquaredMagnitude();
			if (smallestSqrMag <= -1 || sqrMag < smallestSqrMag)
			{
				nearestEntity = (*i);
			}
		}

		return nearestEntity;
	}

	Entity* Scene::GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		std::list<Entity*> *entities = &this->entities;

		if (tag != "")
		{
			entities = GetAllTag(tag);
		}

		if (entities != NULL)
		{
			for (std::list<Entity*>::iterator i = entities->begin(); i != entities->end(); ++i)
			{
				if ((*i) != ignoreEntity)
				{
					Vector2 diff = (*i)->GetWorldPosition() - position;
					if (diff.IsInRange(ENTITY_CONTROLPOINT_SIZE))
					{
						float sqrMag = diff.GetSquaredMagnitude();
						if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
						{
							smallestSqrMag = sqrMag;
							nearestEntity = (*i);
						}
					}
				}

				//Entity *nearestChild = (*i)->GetNearestEntityByControlPoint(position, tag, ignoreEntity, smallestSqrMag);
				//if (nearestChild)
				//{
				//	nearestEntity = nearestChild;
				//}
			}
		}


		return nearestEntity;
	}

	Entity* Scene::GetFirstTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0 || tagMap[tag].size() == 0)
			return NULL;
		
		return tagMap[tag].front();
	}

	std::list<Entity*>* Scene::GetAllTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0)
			return NULL;
		else
			return &tagMap[tag];
	}

	int Scene::GetAmountTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0) 
			return 0;
		
		return static_cast<int>(tagMap[tag].size());
	}

	const std::list<Entity*>* Scene::GetEntities()
	{
		return &entities;
	}

	Entity* Scene::GetEntityAtPosition(const Vector2 &position)
	{
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i)->IsPositionInGraphic(position))
			{
				return *i;
			}
		}
		//if (searchType == SEARCH_TOP)
		//{

		//}
		//else if (searchType == SEARCH_RECURSIVE)
		//{
		//	Debug::Log("recursive");
		//	for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		//	{
		//		Entity *entity = (*i)->GetChildEntityAtPosition(position);
		//		if (entity)
		//		{
		//			return entity;
		//		}
		//	}
		//}
		return NULL;
	}



	/*
	Entity *Scene::GetFirstEntity()
	{
		entityIterator = entities.begin();
		return *entityIterator;
	}

	Entity *Scene::GetNextEntity()
	{
		++entityIterator;
		if (entityIterator != entities.end())
			return *entityIterator;
		return NULL;
	}
	*/
}
