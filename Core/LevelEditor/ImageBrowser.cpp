#include "ImageBrowser.h"
#include "../Input.h"
#include "../MonocleToolkit.h"
#include "../Graphics/Sprite.h"
#include "../Game.h"

namespace Monocle
{
	SelectionImage::SelectionImage(const std::string &image, int size)
		: Entity()
	{
		Sprite *sprite = NULL;
		SetGraphic(sprite = new Sprite(image));
		if (sprite->width > sprite->height)
		{
			sprite->height = (sprite->height/sprite->width) * size;
			sprite->width = size;
		}
		else
		{
			sprite->width = (sprite->width/sprite->height) * size;
			sprite->height = size;
		}
	}

	void SelectionImage::Update()
	{
		Entity::Update();

		Vector2 diff = Input::GetWorldMousePosition() - GetWorldPosition();//position - Graphics::GetScreenCenter();
		float mag = (1.0f-(diff.GetMagnitude()/256.0f)) * 1.0f;
		if (mag < 0.5f) mag = 0.5f;
		if (mag > 1.25f) mag = 1.25f;
		scale = Vector2::one * mag;
		printf("scale %f, %f\n", scale.x, scale.y);
	}

	/// IMAGE BROWSER
	ImageBrowser::ImageBrowser()
		: Entity(), selectionImageSize(128), hasContent(false)
	{
		SetLayer(-85);
		followCamera = Vector2::one;
	}

	void ImageBrowser::Update()
	{
		scale = Game::GetScene()->GetCamera()->scale;
		scale = Vector2(1/scale.x, 1/scale.y);

		Entity::Update();

		if (hasContent)
		{
			for (int key = (int)KEY_A; key <= (int)KEY_Z; key++)
			{
				if (Input::IsKeyPressed((KeyCode)key))
				{
					// select next image with name starting with first letter "key"
					// i.e. if key A is pressed selected Aardvark
					// if key A is pressed again, select Aardvark2
				}
			}

			grid->position += Platform::mouseScroll * Vector2::down * 0.25f;
			printf("position %f, %f\n", grid->position.x, grid->position.y);
		}
	}

	void ImageBrowser::ScanDirectory(const std::string &directory)
	{
		hasContent = false;
		//DestroyChildren();
		selectionImages.clear();

		Entity *bg = new Entity();
		scene->Add(bg);
		bg->SetParent(this);
		bg->SetGraphic(new Sprite("", 128, 600));
		bg->color = Color::black;
		bg->color.a = 0.75f;
		

		grid = new Entity();
		scene->Add(grid);
		grid->SetParent(this);

		ForEachFile(directory, "png", &ImageBrowser::FileCallback, (intptr_t)this);
	}

	void ImageBrowser::FileCallback(const std::string &filename, intptr_t pointer)
	{
		ImageBrowser *imageBrowser = (ImageBrowser*)pointer;
		imageBrowser->NewSelectionImage(filename);
	}

	void ImageBrowser::SetImages(const std::vector<std::string> &images)
	{
		//DestroyChildren();
		selectionImages.clear();

		for (int i = 0; i < images.size(); i++)
		{
			NewSelectionImage(images[i]);
		}
	}

	void ImageBrowser::NewSelectionImage(const std::string &filename)
	{
		SelectionImage *selectionImage = new SelectionImage(filename, selectionImageSize);
		scene->Add(selectionImage);
		selectionImage->SetParent(this);
		//grid->Add(selectionImage);
		selectionImage->position = Vector2(0.0f, selectionImageSize * selectionImages.size());
		
		selectionImages.push_back(selectionImage);

		hasContent = true;
	}
}
