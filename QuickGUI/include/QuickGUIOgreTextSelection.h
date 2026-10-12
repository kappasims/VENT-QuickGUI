#ifndef QUICKGUIOGRETEXTSELECTION_H
#define QUICKGUIOGRETEXTSELECTION_H

#include "QuickGUIExportDLL.h"
#include "QuickGUITextSelection.h"

#include "OgrePrerequisites.h"
#include "OgreTexture.h"

namespace QuickGUI
{
	/// The Ogre texture that is drawn behind highlighted text.
	class _QuickGUIExport OgreTextSelection :
		public TextSelection,
		public Ogre::TexturePtr
	{
	public:
		OgreTextSelection() {}
		OgreTextSelection(const Ogre::TexturePtr& p) : Ogre::TexturePtr(p) {}
	};
}

#endif
