#include "Unitselector.h"

Unitselector::Unitselector()
	:
	x( 10.0f ),
	y( 10.0f ),
	speed( 4.0f ),
	currentpoint( 0 ),
	nextpoint( 0 ),
	numberofpoints( 0 ),
	IsSelected( false ),
	MPointX( 0 ),
	MPointY( 0 ),
	rise( 0 ),
	run( 0 ),
	slope( 0 ),
	RCLF( false ),
	Inc( true )
{
	for( int index = 0; index < 8; index++ )
	{
		PointX[ index ] = 0;
		PointY[ index ] = 0;
	}
}

void Unitselector::ChangeStates( MouseClient& mouse, KeyboardClient& kbd, D3DGraphics& gfx, Income& inc, Map& map, Unit **Player1 )
{

	if( kbd.KeyIsPressed( VK_F1 ) )
	{
	   MapNM = 1;
	   IsSelected = false;
	}

	if( kbd.KeyIsPressed( VK_F2 ) )
	{
		MapNM = 2;
	}
	if( MapNM == 2 )
	{
		Draw( gfx );
		if( mouse.GetMouseX() <= x + 32 && mouse.GetMouseX() >= x - 32 )
		{
			if( mouse.GetMouseY() <= y + 32 && mouse.GetMouseY() >= y - 32 )
			{
				if( mouse.LeftIsPressed() )
				{
					IsSelected = true;
				}
			}
		}
		if( mouse.LeftIsPressed() )
		{
			if( IsSelected )
			{
				state = Moving;
				for( int index = 0; index < 8; index++ )
				{
					PointX[ index ] = 0;
					PointY[ index ] = 0;
					numberofpoints = 0;
					currentpoint = 0;
					RCLF = false;
					Inc = true;
				}
			}
		}
		else if( kbd.KeyIsPressed( VK_CONTROL ) && mouse.RightIsPressed() )
		{
			state = Patroling;
		}
	}

	if( state == Moving )
	{
		Move( mouse );
	}

	else if( state == Patroling )
	{
		Patrol( mouse, kbd );
	}

	PurchaseUnit( inc, map, Player1, gfx );
}

void Unitselector::Move( MouseClient& mouse )
{
	
	if( mouse.LeftIsPressed() )
	{
		MPointX = mouse.GetMouseX();
		MPointY = mouse.GetMouseY();
		rise = FindRise( MPointY, y );
		run = FindRun( MPointX, x );
		slope = FindSlope( rise, run );
	}

	if( rise < run )
	{
		if( x < MPointX )
		{
			x += speed;
		}

		else
		{
			x -= speed;
		}

		if( y < MPointY )
		{
			y += speed * slope;
		}

		else
		{
			y -= speed * slope;
		}

	}

	else if( run < rise )
	{
		if( x < MPointX )
		{
			x += speed * slope;
		}

		else
		{
			x -= speed * slope;
		}

		if( y < MPointY )
		{
			y += speed;
		}

		else
		{
			y -= speed;
		}

	}

	else
	{
		if( x < MPointX )
		{
			x += speed;
		}

		else
		{
			x -= speed;
		}

		if( y < MPointY )
		{
			y += speed;
		}

		else
		{
			y -= speed;
		}
	}

}

void Unitselector::Patrol( MouseClient& mouse, KeyboardClient& kbd )
{
	if( mouse.RightIsPressed() && kbd.KeyIsPressed( VK_CONTROL) && RCLF == false )
	{
		RCLF = true;
		
		if( numberofpoints < 8 )
		{
			PointX[ numberofpoints ] = mouse.GetMouseX();
			PointY[ numberofpoints ] = mouse.GetMouseY();
			numberofpoints++;
		}
	}
	else if(!mouse.RightIsPressed())
	{
		RCLF = false;
	}
		
	rise = FindRise( PointY[ currentpoint ], y  );
	run = FindRun( PointX[ currentpoint ], x );
	slope = FindSlope( rise, run );
	float dist = FindDistance( PointX[ currentpoint ], PointY[ currentpoint ], x, y );

	if( dist <= 5 )
	{
		if( currentpoint == 0 )
		{
			Inc = true;
		}

		if( Inc )
		{
			currentpoint++;
		}

		if( currentpoint == numberofpoints )
		{
			Inc = false;
		}

		if( !Inc )
		{
			currentpoint--;
		}
	}

	if( rise < run )
	{
		if( x < PointX[ currentpoint ] )
		{
			x += speed;
		}

		else
		{
			x -= speed;
		}

		if( y < PointY[ currentpoint ] )
		{
			y += speed * slope;
		}

		else
		{
			y -= speed * slope;
		}

	}

	else if( run < rise )
	{
		if( x < PointX[ currentpoint ] )
		{
			x += speed * slope;
		}

		else
		{
			x -= speed * slope;
		}

		if( y < PointY[ currentpoint ] )
		{
			y += speed;
		}

		else
		{
			y -= speed;
		}
	}

	else
	{
		if( x < PointX[ currentpoint ] )
		{
			x += speed;
		}

		else
		{
			x -= speed;
		}

		if( y < PointY[ currentpoint ] )
		{
			y += speed;
		}

		else
		{
			y -= speed;
		}
	}
}

void Unitselector::PurchaseUnit( Income& inc, Map& map, Unit **Player1, D3DGraphics& gfx )
{ 
	int indexY = 0;
	int bx = x / 50;
	int by = y / 50;
	
	int beacons = bx + (by * 27);
	for( int indexX = 0;  indexX < NumberOfUnitType * 50; indexX += 50 )
	{
		if(MapNM == 2)
		{
			DrawBeacon( gfx, indexX, indexY );
		}
		int beacon = ( indexX / 50 );
		if( beacons == beacon)
		{
			if( inc.Bank >= Player1[ beacons ]->BankCost )
			{
				if(Player1[ beacons ]->AddUnit(map) == true)
				{
					inc.Bank -= Player1[ beacons ]->BankCost;
				}
			}
		}
	}	


	//for( int beacons = 0; beacons < NumberOfUnitType; beacons++ )
	//{
	//	for( int indexY = 0; indexY < SCREENHEIGHT; indexY += 50 )
	//	{
	//		for( int indexX = 0;  indexX < SCREENWIDTH; indexX += 50 )
	//		{
	//			if( x > indexX && x < indexX + 50 )
	//			{
	//				if( y > indexY && y < indexY + 50 )
	//				{
	//					// beacons 0 - 3, indexX / 50 0 - 27, indexY / 50 0 - 15, 1350 / 50 = 27
	//					// 
	//					int beacon = ( indexX / 50 ) + ( ( indexY / 50 )  * ( 1350 / 50 ) );
	//					if( beacon == 4 )
	//					{
	//						int a = 1;
	//					}
	//					if( beacons == beacon)
	//					{
	//						DrawBeacon( gfx, ( indexX / 50 ), ( indexY / 50) );
	//						if( inc.Bank >= Player1[ beacons ]->BankCost )
	//						{
	//							if(Player1[ beacons ]->AddUnit(map) == true)
	//							{
	//								inc.Bank -= Player1[ beacons ]->BankCost;
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}	
	//	}
	//}
}

void Unitselector::Draw( D3DGraphics& gfx )
{
	gfx.DrawDisc( x, y, 5, 255,0,0 );
	for( int index = 0; index < numberofpoints; index++ )
	{
		gfx.DrawDisc( PointX[ index ], PointY[ index ], 5, 0, 255, 0 );
	}
}

void Unitselector::DrawBeacon( D3DGraphics& gfx, int BX, int BY)
{
		gfx.DrawDisc( BX+25, BY+25, 5, 255, 255, 0 );
}

