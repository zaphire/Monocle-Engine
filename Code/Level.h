#pragma once

#include <string>
#include <vector>
#include <list>

#include "Graphics.h"

namespace Monocle
{
	class Scene;

	class Tileset
	{
	public:
		Tileset(const std::string &name, const std::string &filename, float tileWidth, float tileHeight);
		
		std::string name;				// name of the set
		float tileWidth, tileHeight;		// how big is a tile in the set
		TextureAsset *texture;			// texture ref for the set
	};

	class Tilemap : public Graphic
	{
	public:
		Tilemap(Tileset *tileset, int width, int height, int tileWidth, int tileHeight);

		void Resize(int width, int height);

		void GetWidthHeight(int *width, int *height);

		bool IsTile(int tx, int ty, int tileID);
		int GetTile(int tx, int ty);
		void SetTile(int tx, int ty, int tileID);

		int GetTileAtWorldPosition(const Vector2 &position);
		void SetTileAtWorldPosition(const Vector2 &position, int tileID);

		void WorldToTile(const Vector2 &position, int *tx, int *ty);

		void Update();
		void Render();

		TextureAsset *texture;

	private:
		Tileset *tileset;
		int width, height;
		int tileWidth, tileHeight;
		std::vector<int> tiles;
	};

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
	};
}