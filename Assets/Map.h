#pragma once
#include <vector>
#include "D3DGraphics.h"
#include "Funcs.h"

extern "C" int MapNM;

class Map
{

public:

   Map();
   ~Map();

   enum State
   {
      Empty,                                 
      Player2Unit,
      Player2Building,
      Player1Unit,
      Player1Building,
	  Wall
   };

   struct Coord
   {
	   int X, Y;
   };


   int GetCurrentCell(int& x, int& y );
   State CheckCell(int cellID);					//   Return the enumerated state of the cell
   void SetCell(int cellID, State State);		//   This function will set the state of the cell
   State CheckCurrentCell( int& x, int& y );
   Coord CellToGrid ( int& cellID );
   void CreateWall( int x1, int y1, int x2, int y2 );

   Coord CellXY;
   const int CellSize;								//   Number of pixels in each cell width.  Each cell will be square.
   int MapWidth, MapHeight;							//   Number of cells.
   int Cell[21 * 12];
   
   State CellState;
};