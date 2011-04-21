#pragma once

#include "../Editor.h"
#include "../Input.h"
#include "Puppet.h"

namespace Monocle
{
    class Timeline : public Entity
    {
    public:
        Timeline();
        Animation *currentAnimation;
        void Render();
    };
    
	class PuppetEntity : public Entity
	{
	public:
		PuppetEntity();
		void Update();

		Puppet puppet;
	};

	class PuppetEditor : public CameraEditor
	{
	public:
		PuppetEditor();
		void Init(Scene *scene);
		void Update();

		PuppetEntity *puppetEntity;

		KeyCode keyTogglePause;

		KeyCode keyMoveLeft;
		KeyCode keyMoveRight;
		KeyCode keyMoveUp;
		KeyCode keyMoveDown;

		KeyCode keyRotateLeft;
		KeyCode keyRotateRight;

		KeyCode keyBackwards;
		KeyCode keyForwards;

		KeyCode keySetKeyFrame;

		KeyCode keyZero;

		KeyCode keyOffset;

		Timeline *timeline;

		Scene *scene;
	};

}
