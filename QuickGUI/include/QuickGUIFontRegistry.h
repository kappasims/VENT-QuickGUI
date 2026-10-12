#ifndef QUICKGUIFONTREGISTRY_H
#define QUICKGUIFONTREGISTRY_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIFont.h"

#include "OgreString.h"

#include <map>
#include <vector>

namespace QuickGUI
{
	/// Owns the fonts QuickGUI draws with and finds them by name.
	class _QuickGUIExport FontRegistry
	{
	public:
		/// The default font is the one widgets use when none is given.
		explicit FontRegistry(const Ogre::String& defaultFontName);
		~FontRegistry();

		/// Returns the font registered under a name, loading it from Ogre the first time. Throws if there is none.
		const Font& getFont(const Ogre::String& fontName);
		const Font& getDefaultFont();
		Ogre::String getDefaultFontName() const;
		/// Makes another font the default. Throws if there is no such font.
		void setDefaultFontName(const Ogre::String& fontName);
		/// Registers a font that the caller keeps alive for as long as the registry.
		void registerFont(const Ogre::String& fontName, const Font& font);

	private:
		FontRegistry(const FontRegistry&);
		FontRegistry& operator=(const FontRegistry&);

		Ogre::String mDefaultFontName;
		std::map<Ogre::String,const Font*> mFonts;
		std::vector<Font*> mOwnedFonts;
	};
}

#endif
