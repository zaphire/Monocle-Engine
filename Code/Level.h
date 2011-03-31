#pragma once

#include <string>
#include <vector>
#include <list>

#include "Graphics.h"
#include "Tileset.h"
#include "Graphics/Tilemap.h"
#include "LevelEditor/FringeTile.h"

// temp, replace with generic file interface later
class TiXmlElement;

namespace Monocle
{
	class Scene;

	class Level
	{
	public:
		Level();
		void Init();

		static void SetScene(Scene *scene);
		static void LoadProject(const std::string &filename);
		static void Load(const std::string &filename, Scene* scene=NULL);
		static void Save();
		static void SaveAs(const std::string &filename);
		static void End(); // called on scene end

		static FringeTileset* GetCurrentFringeTileset();

		std::string name;

		std::list<Tilemap*> tilemaps;
		std::list<Tileset> tilesets;

		std::list<FringeTileset> fringeTilesets;

		int width, height;

	private:
		Tileset *GetTilesetByName(const std::string &name);
		FringeTileset *GetFringeTilesetByName(const std::string &name);

		Scene *scene;
		static Level *instance;
		std::string filename;

		FringeTileset *fringeTileset;

		void SaveEntities(TiXmlElement *element);
		void LoadEntities(TiXmlElement *element);

		template <class T> void SaveEntitiesOfType(const std::string &name, TiXmlElement *element);
		template <class T> void LoadEntitiesOfType(const std::string &name, TiXmlElement *element);
	};
}
