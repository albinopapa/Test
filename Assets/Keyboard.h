/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.04.24											  *	
 *	Keyboard.h																			  *
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

class KeyboardServer;

class KeyboardClient
{
public:
	KeyboardClient( const KeyboardServer& kServer );
	bool KeyIsPressed( const char keycode ) const;
private:
	const KeyboardServer& server;
};

class KeyboardServer
{
	friend KeyboardClient;
public:
	KeyboardServer();

	void OnKeyPressed( unsigned char keycode );
	void OnKeyReleased( unsigned char keycode );
private:
	static const int nKeys = 256;
	bool keystates[nKeys];
};