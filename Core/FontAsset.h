#pragma once

#include "Asset.h"
#include "Rect.h"

namespace Monocle
{
	//! \brief An base class for assets encapsulating fonts.
	//! Classes inheriting FontAsset should provide a Load function taking the parameters necessary for 
	//! initialization in addition to the required abstract functions.
	class FontAsset : public Asset
	{
	public:
        FontAsset(): Asset(ASSET_FONT) {}

		//! Determines the bounds of a character.
		//! \param c [in] The character to compute bounds for
		//! \param x [in,out] The x location that this character will be rendered at
		//! \param y [in,out] The y location that this character will be rendered at
		//! \param verts [out] The rectangle specifying the bounds of the character
		//! \param texCoords [out] The rectangle specifying the texture coordinates used to render the character
		//! \remark GetGlyphData may be used to determine how to manually render individual characters.
		//! The x and y coordinates passed to the function determine the location of the baseline of the text (equivalent to
		//! the lines on a sheet of line paper, where the tails of y's and p's hang below the baseline).  Both x and
		//! y parameters will be suitable for re-passing to another GetGlyphData call as they will be set to the position
		//! at which the character following the current character should be rendered (including a small offset if padding
		//! is required).  This means GetGlyphData in a loop
		//! with non-local x and y variables can be used to compute the bounds of full strings
		virtual void GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const = 0;
		//! Determines the width a string of text occupies if rendered.
		//! \param text [in] The string to compute bounds for
		//! \remark This function should not render text.  A font asset should have the ability to measure a string by
		//! computing the character bounds in memory.
		virtual float GetTextWidth(const std::string &text) = 0;
		//! Determines the height a string of text occupies if rendered.  This function measures height from the top of
		//! the highest character to the bottom of the character that spans the lowest.  For example, if given the string
		//! "The lazy fox," it should return the height from the top of the 'T' to the bottom of the 'y.'
		//! \param text [in] The string to compute bounds for
		//! \remark This function should not render text.  A font asset should have the ability to measure a string by
		//! computing the character bounds in memory.
		//! Special attention should be paid if you need strict metrics for font positioning.  This function does not
		//! return the height from the baseline, so basing the rendering position on output from this function (ie
		//! rendering text at GetTextHeight()/2 to attempt to center the baseline) will cause the baseline position
		//! to fluctuate between strings like "flies" and "fly" when the y changes the overall string height.
		virtual float GetTextHeight(const std::string &text) = 0;

		unsigned int texID;
	};
}