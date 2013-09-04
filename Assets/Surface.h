#pragma once
#include "D3DGraphics.h"
#include <string>
#include <iostream>
#include <fstream>
#include <GdiPlus.h>
#pragma comment( lib,"gdiplus.lib" )

class Surface
{
public:
	Surface( std::wstring& filename )
	{
		if(filename == L"Marine" )
		{
			int a = 1;
		}
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup( &gdiplusToken,&gdiplusStartupInput,NULL );
		Gdiplus::Bitmap bitmap( filename.c_str() );
		Gdiplus::Color pixel;
		std::ifstream is;
		
		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		surface = new D3DCOLOR[ height * width ];

		for( unsigned int y = 0; y < bitmap.GetHeight(); y++ )
		{
			for( unsigned int x = 0; x < bitmap.GetWidth(); x++ )
			{
				bitmap.GetPixel( x,y,&pixel );
				surface[ x + y * bitmap.GetWidth() ] = 
					D3DCOLOR_ARGB( pixel.GetA(),pixel.GetR(),pixel.GetG(),pixel.GetB() );
			}
		}
	}

	virtual void Draw( int xoff,int yoff,D3DGraphics& gfx ) const
	{
		const int yStart = max( -yoff, 0 );
		const int xStart = max(-xoff, 0 );
		const int yEnd = min( SCREENHEIGHT - yoff, (int)height );
		const int xEnd = min( SCREENWIDTH - xoff, (int)width );

		for( int y = yStart; y < yEnd; y++ )
		{
			for( int x = xStart; x < xEnd; x++ )
			{
				gfx.PutPixel( x + xoff,y + yoff,surface[ x + y * width ] );
			}
		}
	}

	virtual void BackwardsDraw( int xoff,int yoff,D3DGraphics& gfx ) const
	{
		const int yStart = max( -yoff, 0 );
		const int xStart = max(-xoff, 0 );
		const int yEnd = min( SCREENHEIGHT - yoff, (int)height );
		const int xEnd = min( SCREENWIDTH - xoff, (int)width );

		for( int y = yStart; y < yEnd; y++ )
		{
			for( int x = xStart; x < xEnd; x++ )
			{
				gfx.PutPixel( xoff - x,y + yoff,surface[ x + y * width ] );
			}
		}
	}
	~Surface()
	{
		delete [] surface;
	}

public:
	unsigned int width;
	unsigned int height;
	D3DCOLOR* surface;
};

class KeyedSurface : public Surface
{
public:
	KeyedSurface( std::wstring& filename, D3DCOLOR key )
		: 
	Surface( filename ),
	key( key )
	{}
	virtual void Draw( int xoff,int yoff,D3DGraphics& gfx ) const
	{
		const int yStart = max( -yoff, 0 );
		const int xStart = max(-xoff, 0 );
		const int yEnd = min( SCREENHEIGHT - yoff, (int)height );
		const int xEnd = min( SCREENWIDTH - xoff, (int)width );

		for( int y = yStart; y < yEnd; y++ )
		{
			for( int x = xStart; x < xEnd; x++ )
			{
				D3DCOLOR c = surface[ x + y * width ];
				if( c != key )
				{
					gfx.PutPixel( x + xoff,y + yoff, c );
				}
			}
		}
	}
	virtual void BackwardsDraw( int xoff,int yoff,D3DGraphics& gfx ) const
	{
		const int yStart = max( -yoff, 0 );
		const int xStart = max(-xoff, 0 );
		const int yEnd = min( SCREENHEIGHT - yoff, (int)height );
		const int xEnd = min( SCREENWIDTH - (xoff + width), (int)width );

		for( int y = yStart; y < yEnd; y++ )
		{
			for( int x = xStart; x < xEnd; x++ )
			{
				D3DCOLOR c = surface[ x + y * width ];
				if( c != key )
				{
					gfx.PutPixel( xoff - x,y + yoff,surface[ x + y * width ] );
				}
			}
		}
	}
	
private:
	D3DCOLOR key;
};











