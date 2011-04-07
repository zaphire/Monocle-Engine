#include "../../Monocle.h"
#include "../../TextureAsset.h"

using namespace Monocle;

namespace Pong
{
    class Text: public Entity
    {
    public:
        Text(const std::string& text, FontAsset* font);
        
		void Render();
        
        void SetFont(FontAsset* font) { mFont = font; }
        void SetText(const std::string& text) { mText = text; }
        
    protected:
        FontAsset* mFont;
        std::string mText;
    };
    
	class Paddle : public Entity
	{
	public:
		Paddle();
		void Update();
		void Render();

		KeyCode keyUp;
		KeyCode keyDown;

		float speed;
	};

	class Ball : public Entity
	{
	public:
		Ball();
		void Update();
		void Render();
		TextureAsset *texture;
		Vector2 velocity;
	};

	class GameScene : public Scene
	{
	public:
		Ball *ball;
		Paddle *paddle1, *paddle2;
        Text *scoreText;
        
        int p1Score, p2Score;

		void ResetBall();
        const std::string GetScoreString();


		void Begin();
		void End();
		//void Update();
		
		void ReceiveNote(const std::string &note);
	};
}