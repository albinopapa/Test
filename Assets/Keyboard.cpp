/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.04.24											  *	
 *	Keyboard.cpp																		  *
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
#include "Keyboard.h"


KeyboardClient::KeyboardClient( const KeyboardServer& kServer )
	: server( kServer )
{}

bool KeyboardClient::KeyIsPressed( const char keycode ) const
{
	return server.keystates[ keycode ];
}




KeyboardServer::KeyboardServer()
{
	for ( int index = 0; index < nKeys; index++)
	{
		keystates[ index ] = false;
	}
}

void KeyboardServer::OnKeyPressed( unsigned char keycode)
{
	keystates[ keycode ] = true;
}
void KeyboardServer::OnKeyReleased(unsigned char keycode)
{
	keystates[ keycode ] = false;
}
