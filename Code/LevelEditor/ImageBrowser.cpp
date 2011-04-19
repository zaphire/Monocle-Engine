#include "ImageBrowser.h"
#include "../Input.h"
#include "MonocleToolkit.h"

namespace Monocle
{
	SelectionImage::SelectionImage(const std::string &image, int width, int height)
		: Entity()
	{
	}

	void SelectionImage::Update
	{
		Entity::Update();
	}

	/// IMAGE BROWSER
	ImageBrowser::ImageBrowser()
		: Entity(), selectionWidth(64), selectionHeight(64), hasContent(false)
	{
	}

	void ImageBrowser::Update()
	{
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
		}
	}

	void ImageBrowser::ScanDirectory(const std::string &directory)
	{
		DestroyChildren();
		ForEachFile(directory, "bmp|png|jpg|gif|psd", FileCallback(), (intptr_t)this);
	}

	void ImageBrowser::FileCallback(const std::string &filename, intptr_t pointer)
	{
		ImageBrowser *imageBrowser = (ImageBrowser*)pointer;
		imageBrowser->NewSelectionImage(filename);
	}

	void ImageBrowser::NewSelectionImage(const std::string &filename)
	{
		SelectionImage *selectionImage = new SelectionImage(filename, selectionWidth, selectionHeight);
		Add(selectionImage);
		selectionImages.push_back(selectionImage);
	}
}
