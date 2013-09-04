#include "Font.h"

Font::Font( KeyedSurface* surface, const char* filename, int charWidth,int charHeight,int nCharsPerRow )
	:	surface ( surface),
	filename (filename),
	charWidth (surface->width),
	charHeight(surface->height),
	nCharsPerRow(nCharsPerRow)
{
}