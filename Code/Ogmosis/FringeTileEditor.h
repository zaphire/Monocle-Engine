#include "../Vector2.h"

namespace Monocle
{
	class FringeTile;
	class Scene;
	class Entity;


	enum FringeTileEditorState
	{
		FTES_NONE=0,
		FTES_MOVE,
		FTES_ROTATE,
		FTES_SCALE,
		FTES_MAX
	};

	class FringeTileEditor
	{
	public:
		FringeTileEditor();
		void Init(Scene *scene);
		void Enable();
		void Disable();
		void Update();

	protected:
		void UpdateCamera();
		void UpdateSelect();
		void UpdateOpportunity();
		void UpdateMove();
		void UpdateRotate();
		void UpdateScale();

		void Clone();

		void SetState(FringeTileEditorState state);

		void Select(Entity *entity);

		Entity *selectedEntity;
		FringeTile *selectedFringeTile;
		Scene *scene;
		bool isOn;

		Vector2 moveOffset;
		Vector2 moveStartPosition;

		FringeTileEditorState state;
	};
}