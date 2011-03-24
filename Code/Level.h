#pragma once

#include <string>
#include <vector>
#include <list>

#include "Graphics.h"
#include "Tileset.h"
#include "Graphics/Tilemap.h"
#include "LevelEditor/FringeTile.h"

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

		static FringeTile* AddFringeTile(FringeTileset *fringeTileset, int tileID, int layer, const Vector2 &position, const Vector2 &scale, int rotation);
		static void RemoveFringeTile(FringeTile* fringeTile);

		std::string name;

		std::list<Tilemap*> tilemaps;
		std::list<Tileset> tilesets;

		std::list<FringeTile*> fringeTiles;
		std::list<FringeTileset> fringeTilesets;

		int width, height;

	private:
		Tileset *GetTilesetByName(const std::string &name);
		FringeTileset *GetFringeTilesetByName(const std::string &name);

		Scene *scene;
		static Level *instance;
		std::string filename;
	};
}
