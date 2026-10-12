#include "QuickGUICharacter.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	Character::Character(Ogre::UTFString::code_point cp, const Font& f, ColourValue cv) :
		codePoint(cp),
		colorValue(cv.r,cv.g,cv.b,cv.a),
		font(&f),
		mHighlighted(false),
		mAdvance(0),
		mBearing(0)
	{
		setFont(f);
	}

	bool Character::getHighlighted()
	{
		return mHighlighted;
	}

	ColourValue Character::getHighlightColor()
	{
		return colorValue;
	}

	ColourValue Character::getHighlightedTextColor()
	{
		return ColourValue(1.0 - colorValue.r,1.0 - colorValue.g,1.0 - colorValue.b,1.0);
	}

	bool Character::isWhiteSpace()
	{
		return Text::isWhiteSpace(codePoint);
	}

	void Character::setBaseline(float baseline)
	{
		dimensions.position.x = baseline + mBearing;
	}

	void Character::setFont(const Font& f)
	{
		float baseline = getBaseline();

		font = &f;
		mBearing = 0;
		mAdvance = font->getGlyphAdvance(codePoint);

		if(Text::isWhiteSpace(codePoint))
		{
			dimensions.size = Size(mAdvance,font->getGlyphHeight('0'));
		}
		else
		{
			uvCoords = font->getGlyphUVCoords(codePoint);
			dimensions.size = font->getGlyphBitmapSize(codePoint);
			mBearing = font->getGlyphBearing(codePoint);
		}

		dimensions.size.roundUp();
		setBaseline(baseline);
	}

	void Character::setHighlighted(bool highlighted)
	{
		mHighlighted = highlighted;
	}
}
