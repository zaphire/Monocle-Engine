#include <Monocle.h>
#include <TextureAsset.h>
#include <Graphics/Sprite.h>

#include <string>
#include <vector>

using namespace Monocle;

namespace Flash
{
	class Texture
	{
	public:
		Texture(){}
		std::string name;
		std::string path;
		int width, height, zIndex;
		Vector2 registrationPoint;

		bool operator < (const Texture& str) const
		{
			return (zIndex < str.zIndex);
		}
	};

	class TextureSheet
	{
	public:
		TextureSheet(){}
		Texture* GetTextureByName(const std::string &name);

		std::string name;
		std::vector<Texture> textures;
	};

	class Frame
	{
	public:
		Frame()
			: pos(Vector2::zero), scale(Vector2::one), alpha(1.0f), rotation(0.0f)
		{}
		Vector2 pos;
		Vector2 scale;
		float rotation;
		float alpha;
	};

	class Part
	{
	public:
		Part(const std::string &name)
			: name(name), entity(NULL), sprite(NULL)
		{}

		Entity *CreateEntity(TextureSheet &textureSheet);
		void Update(float frame);
		void ApplyFrameToEntity(int frame, Entity *entity=NULL);
		void CloneNewEndFrame();

		std::string name;
		std::vector<Frame> frames;

		Entity *entity;
		Sprite *sprite;
	};

	class Animation
	{
	public:
		Animation(const std::string &name, int frameCount)
			: name(name), frameCount(frameCount)
		{}
		int GetMaxFrames();
		std::string name;
		int frameCount;
		std::vector<Part> parts;
		
	};

	class OnionSkinning
	{
	public:
		OnionSkinning();

		void Make(Scene* scene);
		void CleanUp();

		std::vector<Entity*> entities;
	};

	/*
	class EntityEditor
	{
	public:
		void Update();

		Entity *currentEntity;
		std::string state;
		Vector2 offset;
	};
	*/

	// will eventually be split off into separate animation + animationEditor objects
	// temporary home here for testing/building
	class TestScene : public Scene
	{
	public:
		void Begin();
		void Update();
		
	private:
		void LoadAnimation(const std::string &filename);
		void LoadTextureSheet(const std::string &filename);
		void InitAnimation(Animation *animation, Entity *eParent);
		void Play(Animation *animation, float fps);
		void Pause();
		void Resume();
		void UpdateAnimation(Animation *animation);
		void OffsetFramesBy(const Vector2 &offset);
		void GoPrevFrame(int num=1);
		void GoNextFrame(int num=1);
		void ApplyFrame();
		void SelectPrevPart();
		void SelectNextPart();
		void CreateOnionSkins();
		void DeleteOnionSkins();
		int SafeFrameRange(int frame);
		void ApplyPrevPartFrame();
		void ApplyNextPartFrame();
		void RevertToBackupPartFrame();
		void StoreBackupPartFrame();
		void CloneNewEndFrame();
		void UpdateFrameNumberDisplay();
		Part *GetPartForEntity(Entity *entity);

		std::vector<Entity*> onionSkins;	// list of entities used to create onion skin effect
		std::vector<Animation> animations;	// list of animations
		Frame backupPartFrame;				// store a backup of one frame of one part
		TextureSheet textureSheet;			// the texture sheet, defines png files in the animation
		Animation *currentAnimation;		// stores current animation (for playing and editing)
		Entity *eAnimation;					// contains the animation
		bool isPlaying;						// are we currently playing the animation?
		bool isEditing;						// are we currently editing the animation?
		bool isRecording;					// are we recording the animation?
		int selectedPartIndex;				// current selected part index (only used for keyboard input selection now)
		float animationFrame;				// current animation frame
		float fps;							// frame rate at which animation updates

		Part *editPart;						// currently selected part

		// move into common object eventually
		Entity *currentEntity;
		std::string state;
		Vector2 offset;
	};
}
