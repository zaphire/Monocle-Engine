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
		void UpdateCommands();

		void ApplyGrid(Entity *entity, int gridSize);

		template <class T>
		void CloneEntity(T *t);

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

		bool waitForLMBRelease;

		FringeTileEditorState state;
		Vector2 lastWorldMousePosition;
		//Cursor *cursor;
		int moveAxisLock;

		KeyCode keyMove;
		KeyCode keyRotate;
		KeyCode keyScale;
		KeyCode keySelect;
		KeyCode keyFocus;
		KeyCode keyClone;
		KeyCode keyFlipH;
		KeyCode keyFlipV;
		KeyCode keyPrevTile;
		KeyCode keyNextTile;
		KeyCode keyDelete;
		KeyCode keyScaleDown;
		KeyCode keyScaleUp;
		KeyCode keyOpenImageBrowser;

		/// NODE
		void CloneNode();
	};
}
