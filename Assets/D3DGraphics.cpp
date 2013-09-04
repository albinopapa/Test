/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.04.24											  *	
 *	D3DGraphics.cpp																		  *
 *	Copyright 2012 PlanetChili.net														  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "D3DGraphics.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

D3DGraphics::D3DGraphics( HWND hWnd )
	:
pDirect3D( NULL ),
pDevice( NULL ),
pBackBuffer( NULL ),
pSysBuffer( NULL )
{
	HRESULT result;

	backRect.pBits = NULL;
	
	pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert( pDirect3D != NULL );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp,sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice );
	assert( !FAILED( result ) );

	result = pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer );
	assert( !FAILED( result ) );

	pSysBuffer = new D3DCOLOR[ SCREENWIDTH * SCREENHEIGHT ];
}

D3DGraphics::~D3DGraphics()
{
	if( pDevice )
	{
		pDevice->Release();
		pDevice = NULL;
	}
	if( pDirect3D )
	{
		pDirect3D->Release();
		pDirect3D = NULL;
	}
	if( pBackBuffer )
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	if( pSysBuffer )
	{
		delete pSysBuffer;
		pSysBuffer = NULL;
	}
}

void D3DGraphics::PutPixel( int x,int y,int r,int g,int b )
{	
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	pSysBuffer[ x + SCREENWIDTH * y ] = D3DCOLOR_XRGB( r,g,b );
}

void D3DGraphics::PutPixel( int x,int y,D3DCOLOR c )
{	
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	pSysBuffer[ x + SCREENWIDTH * y ] = c;
}

D3DCOLOR D3DGraphics::GetPixel( int x,int y )
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	return pSysBuffer[ x + SCREENWIDTH * y ];
}

void D3DGraphics::BeginFrame()
{
	memset( pSysBuffer,FILLVALUE,sizeof( D3DCOLOR ) * SCREENWIDTH * SCREENHEIGHT );
}

void D3DGraphics::EndFrame()
{
	HRESULT result;

	result = pBackBuffer->LockRect( &backRect,NULL,NULL );
	assert( !FAILED( result ) );

	for( int y = 0; y < SCREENHEIGHT; y++ )
	{
		memcpy( &((BYTE*)backRect.pBits)[ backRect.Pitch * y ],&pSysBuffer[ SCREENWIDTH * y ],sizeof( D3DCOLOR ) * SCREENWIDTH );
	}

	result = pBackBuffer->UnlockRect();
	assert( !FAILED( result ) );

	result = pDevice->Present( NULL,NULL,NULL,NULL );
	assert( !FAILED( result ) );
}

void D3DGraphics::DrawGrid()
{
	for (int height = 63; height < SCREENHEIGHT; height += 64)
	{
		for (int width = 0; width < SCREENWIDTH; width++)
		{
			PutPixel(width, height, 0, 0, 0);
		}
	}
	for (int height = 0; height < SCREENHEIGHT; height++)
	{
		for (int width = 63; width < SCREENWIDTH; width += 64)
		{
			PutPixel(width, height, 0, 0, 0);
		}
	}

}

void D3DGraphics::DrawDisc( int cx,int cy,int r,int rd,int g,int b )
{
	for( int x = cx - r; x < cx + r; x++ )
	{
		for( int y = cy - r; y < cy + r; y++ )
		{
			if( sqrt( (float)( (x - cx)*(x - cx) + (y - cy)*(y - cy) ) ) < r )
			{
				PutPixel( x,y,rd,g,b );
			}
		}
	}
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2,int r,int g,int blu )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,r,g,blu );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
}

void D3DGraphics::DrawCircle( int centerX,int centerY,int radius,int r,int g,int b )
{
	int rSquared = radius*radius;
	int xPivot = (int)(radius * 0.707107f + 0.5f);
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - x*x ) ) + 0.5f);
		PutPixel( centerX + x,centerY + y,r,g,b );
		PutPixel( centerX - x,centerY + y,r,g,b );
		PutPixel( centerX + x,centerY - y,r,g,b );
		PutPixel( centerX - x,centerY - y,r,g,b );
		PutPixel( centerX + y,centerY + x,r,g,b );
		PutPixel( centerX - y,centerY + x,r,g,b );
		PutPixel( centerX + y,centerY - x,r,g,b );
		PutPixel( centerX - y,centerY - x,r,g,b );
	}
}

void D3DGraphics::DrawButton( int x, int y, int r, int g, int b )
{
	DrawLine( x, y, x + 100, y, r, g, b );
	DrawLine( x, y + 50, x + 100, y + 50, r, g, b );
	DrawLine( x, y, x , y + 50, r, g, b );
	DrawLine( x + 100, y, x + 100 , y + 50, r, g, b );
}

void D3DGraphics::DrawEnemyFlag( int x, int y, int r, int g, int b )
{
	DrawLine( x, y, x + 35, y, r, g, b );
	DrawLine( x, y + 25, x + 35, y + 25, r, g, b );
	DrawLine( x, y, x , y + 25, r, g, b );
	DrawLine( x + 35, y, x + 35 , y + 25, r, g, b );
	DrawLine( x, y + 25, x, y + 50, r, g, b );
}

void D3DGraphics::DrawStatusLifeBar( int x, int y, int StartingHP, float HP, int Player, int Xoff  )
{
	if( Player == 1 )
	{
		int Off = Xoff / 2;
		DrawLine( x + 20 - Off, y + 5, x + 40 - Off, y + 5, 0, 255 , 0 );
		DrawLine( x + 20 - Off, y + 5, x + 20 - Off, y , 0, 255 , 0 );
		DrawLine( x + 20 - Off ,y, x + 40 - Off, y , 0, 255 , 0 );
		DrawLine( x + 40 - Off, y, x + 40 - Off, y + 5 , 0, 255 , 0 );
		for( int PixelY = y - 5; PixelY < y; PixelY++ )
		{
			float Multiple = ( HP/StartingHP );
			for( float PixelX = x + 20; PixelX < ( ( x + 20 ) + (20 * Multiple ) ); PixelX++ )
			{
				PutPixel( PixelX - Off, PixelY + 5, 0, 255, 0 );
			}
		}
	}
	else
	{
		DrawLine( x - 20, y + 5, x - 40, y + 5, 0, 255 , 0 );
		DrawLine( x - 20, y + 5, x - 20, y , 0, 255 , 0 );
		DrawLine( x - 20 ,y, x - 40, y , 0, 255 , 0 );
		DrawLine( x - 40, y, x - 40, y + 5 , 0, 255 , 0 );
		for( int PixelY = y - 5; PixelY < y; PixelY++ )
		{
			float Multiple = ( HP/StartingHP );
			for( float PixelX = x - 20; PixelX > ( ( x - 20 ) - (20 * Multiple ) ); PixelX-- )
			{
				PutPixel( PixelX, PixelY + 5, 0, 255, 0 );
			}
		}
	}
}

void D3DGraphics::DrawAmmo( int x, int y, int size )
{
	for(int starty = 0; starty <= (size / .75); starty++)
	{
		for( int startx = 0; startx <= size; startx++)
		{
			PutPixel(startx + x, starty + y, 10,10,10);
		}
	}
}

void D3DGraphics::DrawState(int x, int y, int r, int g, int b)
{
	int cx = x;
	int cy = y;
	int rd = r;
	int radius = 20;
	DrawDisc(  cx, cy, radius, rd, g, b );
}
