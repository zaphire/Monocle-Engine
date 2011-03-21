#pragma once
#include <string>
#include "../Scene.h"
#include "../Graphics.h"

namespace Monocle
{

	class Tileset
	{
	public:
		Tileset(const std::string &name, const std::string &filename);
		std::string name;
		TextureAsset *texture;
	};

	class Tilemap : public Graphic
	{
	public:
		Tilemap(const std::string &filename, int width, int height, int tileWidth, int tileHeight);

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
		int width, height;
		int tileWidth, tileHeight;
		std::vector<int> tiles;
	};

	class Level
	{
	public:
		Level();

		void SetScene(Scene *scene);
		void Load(const std::string &filename);
		void Save();
		void SaveAs(const std::string &filename);

		std::string name;

		std::list<Tilemap*> tilemaps;
		std::list<Tileset*> tilesets;

	private:
		Scene *scene;
	};

	class Ogmosis : public Scene
	{
	public:
		void Begin();
		void Update();
		void End();

		Level level;
	};
}