#include "QuickGUIFontRegistry.h"
#include "QuickGUIException.h"
#include "QuickGUIOgreFont.h"

#include "OgreFontManager.h"

namespace QuickGUI
{
	FontRegistry::FontRegistry(const Ogre::String& defaultFontName) :
		mDefaultFontName(defaultFontName)
	{
	}

	FontRegistry::~FontRegistry()
	{
		for(std::vector<Font*>::iterator it = mOwnedFonts.begin(); it != mOwnedFonts.end(); ++it)
		{
			OGRE_DELETE_T((*it),Font,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	const Font& FontRegistry::getFont(const Ogre::String& fontName)
	{
		std::map<Ogre::String,const Font*>::iterator it = mFonts.find(fontName);
		if(it != mFonts.end())
		{
			return *(it->second);
		}

		Ogre::FontPtr fp = static_cast<Ogre::FontPtr>(Ogre::FontManager::getSingleton().getByName(fontName));
		if(fp.isNull())
		{
			throw Exception(Exception::ERR_ITEM_NOT_FOUND,"No font with name \"" + fontName + "\" exists!","FontRegistry::getFont");
		}

		Font* font = OGRE_NEW_T(OgreFont,Ogre::MEMCATEGORY_GENERAL)(fp);
		mOwnedFonts.push_back(font);
		mFonts[fontName] = font;

		return *font;
	}

	const Font& FontRegistry::getDefaultFont()
	{
		return getFont(mDefaultFontName);
	}

	Ogre::String FontRegistry::getDefaultFontName() const
	{
		return mDefaultFontName;
	}

	void FontRegistry::setDefaultFontName(const Ogre::String& fontName)
	{
		getFont(fontName);
		mDefaultFontName = fontName;
	}

	void FontRegistry::registerFont(const Ogre::String& fontName, const Font& font)
	{
		mFonts[fontName] = &font;
	}
}
