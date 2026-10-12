#ifndef QUICKGUITEXTSELECTION_H
#define QUICKGUITEXTSELECTION_H

#include "QuickGUIExportDLL.h"

#include <string>

namespace QuickGUI
{
	/// The texture that is drawn behind highlighted text.
	class _QuickGUIExport TextSelection
	{
	public:
		virtual ~TextSelection() {}

		/// Returns the name the texture is registered under.
		virtual std::string getName() const = 0;
		/// Returns the name of the group the texture belongs to.
		virtual std::string getGroup() const = 0;
		/// Returns the width in pixels.
		virtual float getWidth() const = 0;
		/// Returns the height in pixels.
		virtual float getHeight() const = 0;
		/// Returns true if there is no texture.
		virtual bool empty() const = 0;
	};
}

#endif
