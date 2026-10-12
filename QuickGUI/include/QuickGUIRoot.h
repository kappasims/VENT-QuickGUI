#ifndef QUICKGUIROOT_H
#define QUICKGUIROOT_H

#include "QuickGUIEventHandlerManager.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIFont.h"
#include "QuickGUIFontRegistry.h"
#include "QuickGUIManager.h"

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"

#include <unordered_map>

#include <map>

namespace QuickGUI
{
	// forward declarations
	class Sheet;

	class _QuickGUIExport Root :
		public Ogre::Singleton<Root>
	{
	public:
		friend class Sheet;
		// Needs access to first available GUIManager
		friend class ComboBox;
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class ListItem;
		friend class Menu;
		friend class MenuItem;
		friend class TabControl;
		friend class ToolBarItem;
		friend class Widget;
	public:
		Root();
		~Root();

		static Root& getSingleton(void); 
		static Root* getSingletonPtr(void);

		GUIManager* createGUIManager(GUIManagerDesc& d);

		void destroyGUIManager(GUIManager* gm);
		void destroyGUIManager(const std::string& name);

		GUIManager* getGUIManager(const std::string& name);
		/**
		* Gets the default color used by widgets on initialization.
		*/
		ColourValue getDefaultColor();
		/**
		* By default, the name of the first font found on initialization of Root is returned.
		* The default font name can be changed using "setDefaultFontName"
		*/
		Ogre::String getDefaultFontName();
		/**
		* Gets a font by the name it was registered with. Throws if there is no such font.
		*/
		const Font& getFont(const Ogre::String& fontName);
		/**
		* Gets the font that is used by widgets on initialization, see "getDefaultFontName".
		*/
		const Font& getDefaultFont();
		/// Returns the height in pixels of any glyph of the named font.
		float getFontHeight(const Ogre::String& fontName);
		/// Returns the name of the texture that holds the glyph bitmaps of the named font.
		Ogre::String getFontTextureName(const Ogre::String& fontName);
		float getGlyphHeight(const Ogre::String& fontName, Ogre::UTFString::code_point cp);
		UVRect getGlyphUVCoords(const Ogre::String& fontName, Ogre::UTFString::code_point cp);
		Size getGlyphSize(const Ogre::String& fontName, Ogre::UTFString::code_point cp);
		float getGlyphAdvance(const Ogre::String& fontName, Ogre::UTFString::code_point cp);
		float getGlyphBearing(const Ogre::String& fontName, Ogre::UTFString::code_point cp);
		float getTextWidth(const Ogre::String& fontName, const Ogre::UTFString& s);
		void saveFontTextureToFile(const Ogre::String& fontName, const Ogre::String& fileName);
		/// Returns the registry that owns the fonts.
		FontRegistry& getFontRegistry();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getDefaultHoverTime();

		/**
		* Sets the default color used by widgets on initialization.
		*/
		void setDefaultColor(const ColourValue& cv);
		QUICKGUI_DEPRECATED("The default font is chosen when the Root is created.")
		void setDefaultFontName(const Ogre::String& fontName);
		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setDefaultHoverTime(float seconds);

		/**
		* Set GUI size for those silly HiDPI monitors 
		*/
		void setGUIScale(float scale);

		/**
		* Get GUI size for those silly HiDPI monitors 
		*/
		float getGUIScale();

	protected:
		ColourValue mDefaultColor;
		FontRegistry mFontRegistry;

		int mGUIManagerCounter;

		float dpiScale;

#if USEHASHMAPS
		std::unordered_map<std::string,GUIManager*> mGUIManagers;
#else
		std::map<std::string,GUIManager*> mGUIManagers;
#endif
		void notifyWidgetDestroyed(Widget* w);
		void notifySheetDestroyed(Sheet* sheet);

		// The amount of time the cursor has to hover over a widget before
		// the ON_HOVER event is fired. (3 seconds by default)
		float mDefaultHoverTime;
	};
}

#endif
