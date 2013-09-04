/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.04.24											  *	
 *	Game.cpp																			  *
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
#include "Game.h"

Game::Game( HWND hWnd,const KeyboardServer& kServer,const MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer ),
	base( std::wstring( L"Base" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 0,0,0 ), 1 ),
	ebase( std::wstring( L"Base" ), 1, 1, 1, 1,D3DCOLOR_XRGB( 0,0,0 ), 2 ),	
	marine( std::wstring( L"Marine" ), 16, 1, 1, 2, D3DCOLOR_XRGB( 255,255,255 ), 1 ),
	enemymarine( std::wstring( L"Marine" ),16, 1, 1, 2,D3DCOLOR_XRGB( 255,255,255 ), 2  ),
	zergling( std::wstring( L"Zergling" ), 1, 1, 1, 1,D3DCOLOR_XRGB( 255,255,255 ), 1  ),
	enemyzergling( std::wstring( L"Zergling" ), 1, 1, 1, 1,D3DCOLOR_XRGB( 255,255,255 ), 2  ),
	tank( std::wstring( L"Tank" ),1,1,D3DCOLOR_XRGB( 0,0,0 ), 1  ),
	enemytank( std::wstring( L"Tank" ),1,1,D3DCOLOR_XRGB( 0,0,0 ), 2 ),
	sniper( std::wstring( L"Sniper" ), 1, 1, 1, 1,D3DCOLOR_XRGB( 0,0,0 ), 1  ),
	enemysniper( std::wstring( L"Sniper" ), 1, 1, 1, 1,D3DCOLOR_XRGB( 0,0,0 ), 2  ),
	marauder( std::wstring( L"Marauder" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 0,0,0 ), 1 ),
	emarauder( std::wstring( L"Marauder" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 0,0,0 ), 2 ),
	stalker( std::wstring( L"Stalker" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 0,0,0 ), 1 ),
	estalker( std::wstring( L"Stalker" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 0,0,0 ), 2 ),
	roach( std::wstring( L"Roach" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 1 ),
	eroach( std::wstring( L"Roach" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 2 ),
	hydralisk( std::wstring( L"Hydralisk" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 1 ),
	ehydralisk( std::wstring( L"Hydralisk" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 2 ),
	zealot( std::wstring( L"Zealot" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 1 ),
	ezealot( std::wstring( L"Zealot" ), 1, 1, 1, 1, D3DCOLOR_XRGB( 255,255,255 ), 2 ),
	text( std::wstring( L"fixedsys16x28.bmp" ), D3DCOLOR_XRGB( 255,255,255 ),16,28,32 ),
	selector()


	
{
	srand( (unsigned int)time( NULL ) );
	map.CreateWall( 500, 0, 500, 600 );
}

Game::~Game()
{
	
}

void Game::Go()
{
	FPS.StartFrame();

	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();
	
	FPS.StopFrame();

	
}

void Game::ComposeFrame()
{
	inc.GetIncome();

	char buffer[ 255 ];

	Player1[0] = &zergling;
	Player1[1] = &marine;
	Player1[2] = &roach;
	Player1[3] = &hydralisk;
	Player1[4] = &zealot;
	Player1[5] = &stalker;
	Player1[6] = &marauder;
	Player1[7] = &sniper;
	Player1[8] = &tank;
	Player1[9] = &base;

	Player2[0] = &enemyzergling;
	Player2[1] = &enemymarine;
	Player2[2] = &eroach;
	Player2[3] = &ehydralisk;
	Player2[4] = &ezealot;
	Player2[5] = &estalker;
	Player2[6] = &emarauder;
	Player2[7] = &enemysniper;
	Player2[8] = &enemytank;
	Player2[9] = &ebase;

	selector.ChangeStates( mouse, kbd, gfx, inc, map, Player1 );
	int randN = rand() % (NTotal - 1);

	if( inc.BankComp < Player2[ randN ]->BankCost )
	{
		randN = rand() % ( NTotal - 1 );
	}
	else
	{
		if(Player2[ randN ]->AddUnit(map) == true)
		{
			inc.BankComp -= Player2[ randN ]->BankCost;
		}
	}

	for( int index = 0; index < NumberOfUnitType; index++ )
	{
		Player1[ index ]->TransitionState( Player2, Player1, map, gfx );
		Player2[ index ]->TransitionState( Player1, Player2, map, gfx );
	}
	base.TransitionState( Player2, map, gfx );
	ebase.TransitionState( Player1, map, gfx );
		
	
	for (int y = 31; y < 767; y += 64)
	{
		for ( int x = 31; x < 1365; x += 64)
		{
			int CurrentCell = map.GetCurrentCell(x, y);
			if( map.CheckCell( CurrentCell ) == map.Empty)
			{
				sprintf( buffer, "E");
				text.DrawString(buffer, x, y, D3DCOLOR_XRGB(192,192,192),gfx);
			}
			if( map.CheckCell( CurrentCell ) == map.Player1Unit || map.CheckCell( CurrentCell ) == map.Player1Building )
			{
				sprintf( buffer, "P1");
				text.DrawString(buffer, x, y, D3DCOLOR_XRGB(192,192,192),gfx);
			}
			if( map.CheckCell( CurrentCell ) == map.Player2Unit || map.CheckCell( CurrentCell ) == map.Player2Building )
			{
				sprintf( buffer, "P2");
				text.DrawString(buffer, x, y, D3DCOLOR_XRGB(192,192,192),gfx);
			}
		}
	}

	
	gfx.DrawLine( 500, 0, 500, 600, 0, 0, 0 );
	unsigned int fps = FPS.GetfpsAvg();
	sprintf( buffer, "FPS AVG: %d", fps);
	text.DrawString(buffer,0,600, D3DCOLOR_XRGB(0,0,0), gfx );
	
}
