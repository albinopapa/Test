#pragma once

#include "Surface.h"

class Font
{
	Font ( KeyedSurface* surface, const char* filename, int charWidth,int charHeight,int nCharsPerRow );
	~Font ();

	void DrawChar( char c,int x,int y, D3DCOLOR color );
	void DrawString( const char* string,int x,int y,Font* font,D3DCOLOR color );

	int charWidth;
	int charHeight;
	int nCharsPerRow;
	KeyedSurface *surface;
	const char* filename;
	
};