#ifndef QUICKGUIMEASURABLETEXT_H
#define QUICKGUIMEASURABLETEXT_H

#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	/// Text that takes up room along a line, a single character or a whole line.
	class _QuickGUIExport MeasurableText
	{
	public:
		virtual ~MeasurableText() {}

		/// Returns the width in pixels it takes along the line.
		virtual float getWidth() = 0;
		/// Returns the height in pixels of the tallest font it is drawn with.
		virtual float getHeight() = 0;
		/// Returns the distance in pixels from the top of its tallest glyph to the baseline.
		virtual float getMaxBearingY() = 0;
	};
}

#endif
