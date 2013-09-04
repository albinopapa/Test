/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.04.24											  *	
 *	Game.h																				  *
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
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"
#include "Timer.h"
#include "income.h"
#include "FrameTimer.h"
#include "SurfaceSequence.h"
#include "Surface.h"
#include "Unit.h"
#include "Income.h"
#include "Map.h"
#include "Text.h"
#include "Unitselector.h"
#include <ctime>







class Game
{
public:
	Game( HWND hWnd,const KeyboardServer& kServer,const MouseServer& mServer );
	~Game();
	void Go();
private:
	void ComposeFrame();
	/********************************/
	/*  User Functions              */

	/********************************/
private:
	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;
	/********************************/
	/*  User Variables    
	*/
	//Unit **UnitType;
	Unit *Player1[ NTotal ];
	Unit *Player2[ NTotal ];
	Marine marine;
	Marine enemymarine;
	Zergling zergling;
	Zergling enemyzergling;
	Tank tank;
	Tank enemytank;
	Sniper sniper;
	Sniper enemysniper;
	Marauder marauder;
	Marauder emarauder;
	Stalker stalker;
	Stalker estalker;
	Roach roach;
	Roach eroach;
	Hydralisk hydralisk;
	Hydralisk ehydralisk;
	Zealot zealot;
	Zealot ezealot;
	Income inc;
	Map map;
	Text text;
	FrameTimer FPS;
	Base ebase;
	Base base;

	Unitselector selector;
	//Unit &unit;





	
	/********************************/
};