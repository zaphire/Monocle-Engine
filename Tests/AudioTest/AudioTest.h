#include <Monocle.h>

using namespace Monocle;

namespace AudioTest
{
	class Text: public Entity
	{
	public:
		Text(const std::string& text, FontAsset* font);

		void Render();

		void SetFont(FontAsset* font) { this->font = font; }
		void SetText(const std::string& text) { this->text = text; }

	protected:
		FontAsset* font;
		std::string text;
	};
    
    class Waveform: public Entity
    {
    public:
        Waveform( AudioDeck *deck, int type = 0 );
        
        void Render();
        
    protected:
        
        int type;
        AudioDeck *deck;
    };
    
    class GameScene : public Scene
	{
	public:
        
        Text *scText;

		void Begin();
		void End();
		void Update();
		
		void ReceiveNote(const std::string &note);
	};
}