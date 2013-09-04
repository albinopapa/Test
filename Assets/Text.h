#pragma once
#include "Surface.h"



class Text
{

protected:
	unsigned int width;
	unsigned int height;
	D3DCOLOR key;
public:
	struct Font
	{
		int charWidth;
		int charHeight;
		int nCharsPerRow;
	};
	Font font;
public:
	KeyedSurface keyedsurface;
	
	Text( std::wstring& filename, D3DCOLOR key, int charWidth, int charHeight, int nCharsPerRow  )
		:
	keyedsurface( filename, key ),
	key( key )
	{
		font.charWidth = charWidth;
		font.charHeight = charHeight;
		font.nCharsPerRow = nCharsPerRow;
	}

	~Text()
	{
		delete [] keyedsurface.surface;
	}
	

void DrawChar( unsigned char c,int xoff,int yoff,D3DCOLOR color, D3DGraphics& gfx )
{
	if( c < ' ' || c > '~' )
	return;

	const int sheetIndex = c - ' ';
	const int sheetCol = sheetIndex % font.nCharsPerRow;
	const int sheetRow = sheetIndex / font.nCharsPerRow;
	const int xStart = sheetCol * font.charWidth;
	const int yStart = sheetRow * font.charHeight;
	const int xEnd = xStart + font.charWidth;
	const int yEnd = yStart + font.charHeight;
	const int surfWidth = font.charWidth * font.nCharsPerRow;


	
	for( int y = yStart; y < yEnd; y++ )
	{
		for( int x = xStart; x < xEnd; x++ )
		{
			if( keyedsurface.surface[ x + y * surfWidth ] == D3DCOLOR_XRGB( 0,0,0 ) )
			{
				gfx.PutPixel( x + xoff - xStart,y + yoff - yStart, color );
			}
		}
	}
}

void DrawString( const char* string,int xoff,int yoff, D3DCOLOR color, D3DGraphics& gfx )
{
	for( int index = 0; string[ index ] != '\0'; index++ )
	{
		DrawChar( string[ index ],xoff + index * font.charWidth,yoff, color, gfx );
	}
}

};
