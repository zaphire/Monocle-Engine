#include "../Vector2.h"
#include "../Entity.h"
#include "../Input.h"
#include "Node.h"
#include "../Editor.h"

namespace Monocle
{
	class FringeTile;
	class Scene;
	class Entity;
	class ImageBrowser;

	enum FringeTileEditorState
	{
		FTES_NONE=0,
		FTES_MOVE,
		FTES_ROTATE,
		FTES_SCALE,
		FTES_COMMAND,
		FTES_BROWSER,
		FTES_COLOR,
		FTES_FLIP,
		FTES_MAX
	};

	/*
	class Cursor : public Entity
	{
	public:
		Cursor();
		void Update();
		void Render();
	};
	*/

	class LevelEditor : public Editor, public CameraEditor
	{
	public:
		LevelEditor();
		void Enable();
		void Disable();
		void Update();
		FringeTileEditorState GetState()
		{
			return state;
		}

	protected:
		//void UpdateCamera();
		void UpdateSelect();
		void UpdateOpportunity();
		void UpdateMove();
		void UpdateRotate();
		void UpdateScale();
		void UpdateColor();
		void UpdateCommands();

		void UpdateNumberEntry();

		void ApplyGrid(Entity *entity, int gridSize);

		void CloneEntity(Entity *entity, const Vector2 &position);

		void SetState(FringeTileEditorState state);

		void Select(Entity *entity);

		Entity *selectedEntity;

		FringeTile *selectedFringeTile;
		Node *selectedNode;
		ImageBrowser *imageBrowser;

		Vector2 moveOffset;
		Vector2 moveStartPosition;
		float moveStartMagnitude;
		float startRotation;
		Vector2 startScale;
		Color startColor;

		bool waitForLMBRelease;

		FringeTileEditorState state;
		Vector2 lastWorldMousePosition;
		//Cursor *cursor;
		int moveAxisLock;
		int changeColorValue;

		KeyCode keyMove;
		KeyCode keyRotate;
		KeyCode keyScale;
		KeyCode keySelect;
		KeyCode keyColor;
		KeyCode keyFocus;
		KeyCode keyClone;
		KeyCode keyFlip;
		KeyCode keyFlipH;
		KeyCode keyFlipV;
		KeyCode keyPrevTile;
		KeyCode keyNextTile;
		KeyCode keyDelete;
		KeyCode keyScaleDown;
		KeyCode keyScaleUp;
		KeyCode keyOpenImageBrowser;

		bool isNumberEntry;
		std::string numberEntryString;
		float numberEntryValue;

		std::string text;

		/// NODE
		void CloneNode();
	};
}
