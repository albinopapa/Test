#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "Unit.h"
#include "Map.h"
#include "Income.h"



class Unitselector
{
public:
	Unitselector();
	void ChangeStates( MouseClient& mouse, KeyboardClient& kbd, D3DGraphics& gfx, Income& inc, Map& map, Unit **unit );
	void Move( MouseClient& mouse );
	void Patrol( MouseClient& mouse, KeyboardClient& kbd );
	void PurchaseUnit( Income& inc, Map& map, Unit **Player1, D3DGraphics& gfx );
	void Draw( D3DGraphics& gfx );
	void DrawBeacon( D3DGraphics& gfx, int BX, int BY);

private:
	enum State
	{
		None,
		Moving,
		Patroling,
		Spawning
	};
	State state;
	bool IsSelected;
	int x,y;
	float speed;
	int PointX[ 8 ];
	int PointY[ 8 ];

	int MPointX;
	int MPointY;

	float rise;
	float run;
	float slope;

	int currentpoint;
	int nextpoint;
	int numberofpoints;

	bool RCLF;

	bool Inc;
};