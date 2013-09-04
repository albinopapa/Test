#pragma once
#include <string>
#include "Surface.h"
#include <sstream>
#include <iomanip>
#include "Timer.h"
#include "Mouse.h"

class SurfaceSequence 
{
public:
	SurfaceSequence( std::wstring basename, unsigned int nSurfaces, unsigned int nHoldFrames, D3DCOLOR key = D3DCOLOR_XRGB( 255,255,255 ) )
		:
	nSurfaces( nSurfaces ),
	nHoldFrames( nHoldFrames ),
	curHoldCount( 0 )
	{
		surfaces = new KeyedSurface*[ nSurfaces ];
		for( unsigned int index = 0; index < nSurfaces; index++ )
		{
			std::wstringstream s;
			s << std::setw( 2 ) << std::setfill( L'0' ) << index;
			surfaces[ index ] = new KeyedSurface( basename + s.str() + std::wstring( L".bmp" ), key );
		}

	}
	~SurfaceSequence()
	{
		for( unsigned int index = 0; index < nSurfaces; index++ )
		{
			delete surfaces[ index ];
		}
		
		delete [] surfaces;
	}
	void Draw( int x, int y, D3DGraphics& gfx, int iCurSurface ) const
	{
		surfaces[ iCurSurface ]->Draw( x,y,gfx ); 
	}
	void BackwardsDraw( int x, int y, D3DGraphics& gfx, int iCurSurface ) const
	{
		surfaces[ iCurSurface ]->BackwardsDraw( x,y,gfx ); 
	}
	int Advance(int iCurSurface)
	{
		curHoldCount++;
		if( curHoldCount >= nHoldFrames )
		{
			iCurSurface++;
			iCurSurface %= nSurfaces;
			curHoldCount = 0;
		}
		return iCurSurface;
	}

protected:
	KeyedSurface** surfaces;
	const unsigned int nSurfaces;
	const unsigned int nHoldFrames;
	unsigned int curHoldCount;
};











