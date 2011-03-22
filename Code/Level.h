#pragma once

#include <string>
#include <vector>
#include <list>

#include "Graphics.h"
#include "Tileset.h"
#include "Graphics/Tilemap.h"

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
		static void Load(const std::string &filename);
		static void Save();
		static void SaveAs(const std::string &filename);
		static void End(); // called on scene end

		std::string name;

		std::list<Tilemap*> tilemaps;
		std::list<Tileset> tilesets;

		int width, height;
	private:
		Tileset *GetTileset(const std::string &name);
		Scene *scene;
		static Level *instance;
		std::string filename;
	};
}
