#include "Map.h"
#include <stdlib.h>
#include <math.h>


int MapNM = 1;


Map::Map()
   :   CellSize(64),                           //   Each cell is 64x64 pixels
      MapWidth(21),                           //   This sets the MapWidth to 21 which takes up (21 * CellSize = 1344) pixels on the screen.
      MapHeight(12)
{
   //   Allocate a space in memory for all of the cells
   //Cell = new int [MapWidth * MapHeight];

   //   Start all of the cells as Empty
   for( int index = 0; index < (MapWidth * MapHeight); index++ )
   {
      SetCell( index, Empty );
   }
}

Map::~Map()
{
}

void Map::SetCell(int CellID, State cellState)
{   
   Cell[CellID] = cellState;
}

Map::State Map::CheckCell(int CellID)
{
   //   Lookup for the state of the Cell at cellID
   switch (Cell[CellID])
   {
   case Empty:
        {
           return Empty;
        }
   case Player2Unit:
        {
           return Player2Unit;
        }
   case Player2Building:
        {
           return Player2Building;
        }
   case Player1Unit:
        {
           return Player1Unit;
        }
   case Player1Building:
        {
           return Player1Building;
        }
   case Wall:
        {
           return Wall;
        }
   }
}

int Map::GetCurrentCell(int& x, int& y)
{
   int cellID = (x / CellSize) + (y / CellSize * MapWidth);
   return cellID;
}



Map::Coord Map::CellToGrid ( int& cellID )
{
	CellXY.X = cellID % MapWidth;
	CellXY.Y = cellID / MapWidth;
	return CellXY;
}

void Map::CreateWall( int x1, int y1, int x2, int y2 )
{
	Coord StartCell = {x1 / CellSize, y1 / CellSize };
	Coord EndCell = { x2/ CellSize, y2 / CellSize };
	float Rise = FindRise( StartCell.Y, EndCell.Y );
	float Run = FindRun( StartCell.X, EndCell.Y );
	float Slope = FindSlope( Rise, Run );
	int Current_Cell = GetCurrentCell( x1, y1 );
	int End_Cell = GetCurrentCell( x2, y2 );
	
	while ( Current_Cell != End_Cell )
	{
		if( Rise < Run )
		{
			if( StartCell.X < EndCell.X )
			{
				StartCell.X++;
			}
			else
			{
				StartCell.X--;
			}
			if( StartCell.Y < EndCell.Y )
			{
				StartCell.Y += Slope + 0.5f;
			}
			else
			{
				StartCell.Y -= Slope + 0.5f;
			}			
		}
		else if( Rise > Run )
		{
			if( StartCell.X < EndCell.X )
			{
				StartCell.X += Slope + 0.5f;
			}
			else
			{
				StartCell.X -= Slope + 0.5f;
			}
			if( StartCell.Y < EndCell.Y )
			{
				StartCell.Y++;
			}
			else
			{
				StartCell.Y--;
			}			
		}
		else
		{
			if( StartCell.X < EndCell.X )
			{
				StartCell.X++;
			}
			else
			{
				StartCell.X--;
			}
			if( StartCell.Y < EndCell.Y )
			{
				StartCell.Y++;
			}
			else
			{
				StartCell.Y--;
			}		
		}
		Current_Cell = StartCell.X + StartCell.Y * MapWidth;
		SetCell(Current_Cell, Wall);
	}
}
