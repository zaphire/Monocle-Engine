#include <Monocle.h>
#include <Scene.h>
#include <Rect.h>
#include <Assets.h>
#include <FontAsset.h>
#include <Vector2.h>
#include <string>
#include <GL/gl.h>

class RectEntity : public Monocle::Entity
{
public:
    RectEntity() {}
    RectEntity(Monocle::Rect rect)
    {
        mRect = rect;
    }

    void Render()
    {
        Monocle::Graphics::PushMatrix();
        Monocle::Graphics::SetColor(Monocle::Color::orange);
        Monocle::Graphics::Translate( (mRect.bottomRight.x - mRect.topLeft.x) / 2, (mRect.bottomRight.y - mRect.topLeft.y) / 2, 0);
        Monocle::Graphics::RenderQuad( mRect.bottomRight.x - mRect.topLeft.x, mRect.bottomRight.y - mRect.topLeft.y );
        Monocle::Graphics::PopMatrix();
    }

private:
    Monocle::Rect mRect;
};

class TextRenderTest : public Monocle::Scene
{
public:
    void Begin()
    {
        font = Monocle::Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 24);
        textToRender = "This is just a test: Hello World";
        rectToRender = RectEntity( Monocle::Rect( Monocle::Vector2(0,0), Monocle::Vector2( font->GetTextWidth(textToRender), font->GetTextHeight(textToRender))) );

        Add(&rectToRender);
    }

    void Render()
    {
        Monocle::Graphics::PushMatrix();
        //Monocle::Graphics::Translate(50,50,0);
        Monocle::Scene::Render();

        float x,y;
        Monocle::Rect verts, texCoords;

        Monocle::Graphics::SetColor(Monocle::Color::yellow);

        glBegin(GL_QUADS);

        for (int i = 0; i < textToRender.size(); i++)
        {
            char c = textToRender[i];
			if ((c >= 32) && (c < 128))
			{
				font->GetGlyphData(c, &x, &y, verts, texCoords);

				glVertex2f(verts.topLeft.x, verts.topLeft.y);
				glVertex2f(verts.bottomRight.x, verts.topLeft.y);
				glVertex2f(verts.bottomRight.x, verts.bottomRight.y);
				glVertex2f(verts.topLeft.x, verts.bottomRight.y);
			}
        }

        glEnd();

        Monocle::Graphics::SetColor( Monocle::Color::red );
        //Monocle::Graphics::SetBlend(Monocle::BLEND_ALPHA);
        Monocle::Graphics::BindFont(font);
        Monocle::Graphics::RenderText( *font, textToRender, 0.f, font->GetTextHeight(textToRender) );
        Monocle::Graphics::PopMatrix();
    }
private:
    Monocle::FontAsset *font;
    std::string textToRender;
    RectEntity rectToRender;
};

//int main()
//{
//    Monocle::Game *game = new Monocle::Game();
//    game->SetScene(new TextRenderTest());
//
//    game->Main();
//    return 0;
//}
