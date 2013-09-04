#include "Unit.h"
#include <math.h>
//#include "Text.h"
#define Spacing 1
#define indexMax 63
#include "Surface.h"



Unit::Unit( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:	Move_ani(std::wstring(L"Images\\") + basename + std::wstring(L"\\") + basename + std::wstring(L"_Run"),NMoveS,nHoldFrames,key),
		Attack_ani(std::wstring(L"Images\\") + basename + std::wstring(L"\\") +basename + std::wstring(L"_Attack"),NAttackS,nHoldFrames,key),
		Idle_ani(std::wstring(L"Images\\") + basename + std::wstring(L"\\") + basename + std::wstring(L"_Idle"),NIdleS,nHoldFrames,key),			
		Player( Player ),
		UnitCount (0),
		SpawnPointY( 32 ),
		TotalUnits(NTotal)
	{
		if( Player == 1 )
		{
			SpawnPointX = 32;
		}
		else
		{
			SpawnPointX = 1334 - 32;
		}
		Intel.EnemyBaseCell = -1;
		Intel.FoundEnemyBase = false;
		Intel.TeamTarget = -1;
		for(int i = 0; i < NTargets; i++)
		{
			Enemy.cIndex[ i ] = -1;
			Enemy.uIndex[ i ] = -1;
		}
		Enemy.TargetFound = false;
		Enemy.UnitsInCell = 0;
		
	}

Unit::~Unit()
{
}

void Unit::Draw( D3DGraphics& gfx, Map& map, int& index )
{	
	if( MapNM == 1 )
	{
		if( Player == 1 )
		{
			switch (Units[index].UnitState)
			{
				case Moving:
					//CurrentSurface = Move_ani.Advance(Units[ index ].CurrentSurface);
					Move_ani.Draw( Units[index].x - DrawXoff, Units[index].y - DrawYoff, gfx, Move_ani.Advance(Units[ index ].CurrentSurface)); 
					break;

				case Attacking:
					//CurrentSurface = Attack_ani.Advance(Units[ index ].CurrentSurface);
					Attack_ani.Draw(Units[index].x - DrawXoff, Units[index].y - DrawYoff, gfx, Attack_ani.Advance(Units[index].CurrentSurface));
					break;

				case Assessing:
				case Choosing:
				case Searching:
					//CurrentSurface = Idle_ani.Advance(Units[ index ].CurrentSurface);
					Idle_ani.Draw(Units[index].x - DrawXoff, Units[index].y - DrawYoff, gfx, Idle_ani.Advance(Units[index].CurrentSurface) );
					break;
			}
			gfx.DrawStatusLifeBar( Units[ index ].x - LifeXoff , Units[ index ].y - LifeYoff, StartingHealth, Units[ index ].Health, 1, LifeXoff );
		}
		else
		{
			switch (Units[index].UnitState)
			{
				case Moving:
					//CurrentSurface = Move_ani.Advance(Units[ index ].CurrentSurface);
					Move_ani.BackwardsDraw( Units[index].x + DrawXoff, Units[index].y - DrawYoff, gfx, Move_ani.Advance(Units[index].CurrentSurface) ); 
					break;

				case Attacking:
					//CurrentSurface = Attack_ani.Advance(Units[ index ].CurrentSurface);
					Attack_ani.BackwardsDraw(Units[index].x + DrawXoff, Units[index].y - DrawYoff, gfx, Attack_ani.Advance(Units[index].CurrentSurface) );
					break;
				case Assessing:
				case Choosing:
				case Searching:
					//CurrentSurface = Idle_ani.Advance(Units[ index ].CurrentSurface);
					Idle_ani.BackwardsDraw(Units[index].x + DrawXoff, Units[index].y - DrawYoff, gfx, Idle_ani.Advance(Units[index].CurrentSurface) );
					break;
			}
			gfx.DrawStatusLifeBar( Units[index].x + LifeXoff, Units[index].y - LifeYoff, StartingHealth, Units[ index ].Health, 2, LifeXoff );
		}
	}
}

void Unit::Move( Map& map, int& index)
{
	
	Map::Coord CellXY;
	Map::Coord NavXY;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	Units[ index ].CurrentCell = map.GetCurrentCell(UX, UY);
	NavXY = map.CellToGrid(Units[ index ].NavPoint);
	int NX = NavXY.X * map.CellSize + 32;
	int NY = NavXY.Y * map.CellSize + 32;
	float Rise = FindRise(UY, NY);
	float Run = FindRun(UX, NX);
	float Slope = FindSlope(Rise, Run);

	if(Player == 1 && map.CheckCell(Units[ index ].CurrentCell) != map.Player1Building)
	{
		map.SetCell(Units[ index ].CurrentCell, map.Player1Unit);
	}
	else if(Player == 2 && map.CheckCell(Units[ index ].CurrentCell) != map.Player2Building)
	{
		map.SetCell(Units[ index ].CurrentCell, map.Player2Unit);
	}

	if(Rise < Run)
	{
		if(UX < NX)
		{
			UX += Movement;
		}
		else
		{
			UX -= Movement;
		}
		if(UY < NY)
		{
			UY += Movement * Slope;
		}
		else
		{
			UY -= Movement * Slope;
		}
	}
	else if(Rise > Run)
	{
		if(UX < NX)
		{
			UX += Movement * Slope;
		}
		else
		{
			UX -= Movement * Slope;
		}
		if(UY < NY)
		{
			UY += Movement;
		}
		else
		{
			UY -= Movement;
		}
	}
	else
	{
		if(UX < NX)
		{
			UX += Movement;
		}
		else if(UX > NX)
		{
			UX -= Movement;
		}
		if(UY < NY)
		{
			UY += Movement;
		}
		else if(UY > NY)
		{
			UY -= Movement;
		}
	}

	if(UX < 32)
	{
		UX = 32;
	}
	else if(UX > SCREENWIDTH - 32)
	{
		UX = SCREENWIDTH - 32;
	}

	if(UY < 32)
	{
		UY = 32;
	}
	else if(UY > SCREENHEIGHT - 32)
	{
		UY = SCREENHEIGHT - 32;
	}

	int NextCell = map.GetCurrentCell(UX, UY);

	if(Units[ index ].CurrentCell == NextCell)
	{
		Units[ index ].UnitState = Moving;
	}	
	else
	{		
		Units[ index ].UnitState = Assessing;

		if(Player == 1)
		{
			if(map.CheckCell(NextCell) != map.Player1Building)
			{
				map.SetCell(Units[ index ].CurrentCell, map.Empty);
				map.SetCell(NextCell, map.Player1Unit);
			}
		}
		else
		{
			if(map.CheckCell(NextCell) != map.Player2Building)
			{
				map.SetCell(Units[ index ].CurrentCell, map.Empty);
				map.SetCell(NextCell, map.Player2Unit);
			}
		}
	}

	if(NextCell == Units[ index ].NavPoint)
	{
		Units[ index ].NavPoint = 0;
		Units[ index ].UnitState = Assessing;
	}
	Units[ index ].x = UX;
	Units[ index ].y = UY;
	Units[ index ].CurrentCell = NextCell;
}

void Unit::GiveIntel(Unit** Friend)
{
	if(Intel.TeamTarget >= 0 && Intel.TeamTarget <= (21 * 12))
	{
		for(int classloop = 0; classloop < NTotal; classloop++)
		{
			if(this != Friend[classloop])
			{
				Friend[ classloop ]->Intel = Intel;
			}
		}
	}
	else
	{
		for(int classloop = 0; classloop < NTotal; classloop++)
		{
			Friend[ classloop ]->Intel.TeamTarget = -1;
		}
	}
}

float Unit::FindCellDistance( int& Cell1, int& Cell2, Map &map)
{
	Map::Coord Cell_1 = map.CellToGrid(Cell1);
	Map::Coord Cell_2 = map.CellToGrid(Cell2);
	float X_Dist = Cell_1.X - Cell_2.X;
	float Y_Dist = Cell_1.Y - Cell_2.Y;
	return sqrt(( X_Dist * X_Dist) + (Y_Dist * Y_Dist) );

}

void Unit::FindUnitInCell( int& TestCell, Unit** Foe, Map &map)
{
	int oppCell = -1;
	int cIndex = -1;
	int uIndex = -1;
	for(int a = 0; a < NTargets; a++)
	{
		Enemy.cIndex[ a ] = -1;
		Enemy.uIndex[ a ] = -1;
	}
	Enemy.TargetFound = false;
	Enemy.UnitsInCell = 0;
	int FX = 0;
	int FY = 0;	

	for( cIndex = 0; cIndex < TotalUnits; cIndex++ )
	{				
		for ( uIndex = 0; uIndex < Foe[cIndex]->UnitCount; uIndex++)
		{
			FX = Foe[ cIndex ]->Units[ uIndex ].x;
			FY = Foe[ cIndex ]->Units[ uIndex ].y;
			
			oppCell = map.GetCurrentCell(FX, FY);
			
			if (oppCell == TestCell)
			{
				if(Foe[ cIndex ]->Units[ uIndex ].Health > 0)
				{
					Enemy.TargetFound = true;
					if(cIndex >= NTotal)
					{
						int a = 1;
					}
					Enemy.cIndex[ Enemy.UnitsInCell ] = cIndex;
					Enemy.uIndex[ Enemy.UnitsInCell ] = uIndex;
					Enemy.UnitsInCell++;
				}
				else
				{
					Foe[ cIndex ]->Units[ uIndex ].UnitState = Dead;
					if(Intel.TeamTarget == oppCell)
					{
						Intel.TeamTarget = -1;
					}
				}
			}
		}// <- end of uIndex to UnitCount loop					
	}// <- end of cIndex loop	
}

float Unit::CalculateDamage(int& index, Unit** Foe)
{
	float DamageDealt = AttackDamage;

	for(int i = 0; i < Enemy.UnitsInCell; i++)
	{
		int cIndex = Enemy.cIndex[ i ];
		if(cIndex > NTotal)
		{
			int a = 1;
		}
		DamageDealt = AttackDamage - Foe[ Enemy.cIndex[ i ] ]->Armor;
		if( Foe[ Enemy.cIndex[ i ] ]->Flags == Foe[ Enemy.cIndex[ i ] ]->Light )
		{
			DamageDealt += UnitBonuses.LightBonus;
		}
		if( Foe[ Enemy.cIndex[ i ] ]->Flags == Foe[ Enemy.cIndex[ i ] ]->Armored )
		{
			DamageDealt += UnitBonuses.ArmoredBonus;
		}
	}
	return DamageDealt;
}

void Unit::Attack(Map& map, Unit **Foe, int& index, D3DGraphics &gfx )
{
	int target = Units[index].NavPoint;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	int CurrentCell = map.GetCurrentCell(UX, UY);
	bool TargetFound = false;
	int DamageDealt = AttackDamage;
	if(Player == 1)
	{
		map.SetCell(CurrentCell, map.Player1Unit);
	}
	else
	{
		map.SetCell(CurrentCell, map.Player2Unit);
	}

	FindUnitInCell(target, Foe, map);

	if(Enemy.TargetFound == true)
	{
		if(IsReadyToAttack( index ))
		{
			DamageDealt = CalculateDamage( index, Foe );
			if(Foe[ Enemy.cIndex[0] ]->Units[ Enemy.uIndex[0] ].Health > 0)
			{
				for( int index = 0; index < NAttacks; index++ )
				{
					Foe[ Enemy.cIndex[0] ]->Units[ Enemy.uIndex[0] ].Health -= DamageDealt;
				}
				Units[ index ].UnitState = Attacking;
			}
			else
			{
				Foe[ Enemy.cIndex[0] ]->Units[ Enemy.uIndex[0] ].UnitState = Dead;
				map.SetCell(target, map.Empty);
				
				if(Intel.TeamTarget != Intel.EnemyBaseCell)
				{					
					Intel.TeamTarget = -1;
				}
				Units[ index ].NavPoint = 0;
				Units[ index ].UnitState = Assessing;
			}				
		}// <- end of Timer check
	}// <- end of TargetFound
	else
	{
		Units[ index ].UnitState = Assessing;
	}
}

bool Unit::IsReadyToAttack(int &index)
{
	if(Units[ index ].AttackTimerStarted)
	{
		if(Units[ index ].Attacktimer.GetTimeMilli() >= AttackSpeed * 1000)
		{
			Units[ index].Attacktimer.StopWatch();
			Units[ index ].AttackTimerStarted = false;
			return true;
		}
	}
	else
	{
		Units[ index ].Attacktimer.StartWatch();
		Units[ index ].AttackTimerStarted = true;
	}
	return false;
}

bool Unit::AddUnit( Map &map )
{
	
	if( SpawnPointY >= SCREENHEIGHT - 32 )
	{
		SpawnPointY = 32;
	}
	
	int CurrentCell = map.GetCurrentCell( SpawnPointX, SpawnPointY );

	if( Player == 1 )
	{
		if (map.CheckCell( CurrentCell ) != map.Player2Unit )
		{
			Units.push_back(TypeOfUnit);
			Units[UnitCount].Health = StartingHealth;
			Units[UnitCount].x = SpawnPointX;
			Units[UnitCount].y = SpawnPointY;
			Units[UnitCount].UnitState = Assessing;
			Units[UnitCount].CurrentSurface = 0;
			Units[UnitCount].ClosestEnemyCell = -1;
			Units[UnitCount].NavPoint = 0;
			Units[UnitCount].CurrentCell = CurrentCell;
			SpawnPointY = SpawnPointY + map.CellSize;
			Units[UnitCount].AttackTimerStarted = false;
		
			map.SetCell(CurrentCell, map.Player1Unit);
			UnitCount = Units.size();
			return true;
		}	

		else
		{
			SpawnPointY = SpawnPointY + map.CellSize;
		}
	}

	else if( Player == 2 )
	{
		if (map.CheckCell( CurrentCell ) != map.Player1Unit )
		{
			Units.push_back(TypeOfUnit);
			Units[UnitCount].Health = StartingHealth;
			Units[UnitCount].x = SpawnPointX;
			Units[UnitCount].y = SpawnPointY;
			Units[UnitCount].UnitState = Assessing;
			Units[UnitCount].CurrentSurface = 0;
			Units[UnitCount].ClosestEnemyCell = -1;
			Units[UnitCount].NavPoint = 0;
			Units[UnitCount].CurrentCell = CurrentCell;
			SpawnPointY = SpawnPointY + map.CellSize;
			Units[UnitCount].AttackTimerStarted = false;
		
			map.SetCell(CurrentCell, map.Player2Unit);
			UnitCount = Units.size();
			return true;
		}	

		else
		{
			SpawnPointY = SpawnPointY + map.CellSize;
		}
	}
	
	return false;
}

void Unit::UnitIsAlive( Map& map, int& index )
{	
	int UX = Units[index].x;
	int UY = Units[index].y;
	int CurrentCell = map.GetCurrentCell( UX, UY);
	map.SetCell( CurrentCell, map.Empty );
	Units.erase(Units.begin() + index);
	UnitCount = Units.size();
}

void Unit::FindCellsInRange(Map &map, int& index, Unit **unit, D3DGraphics &gfx )
{
	Units[index].ClosestEnemyCell = -1;
	Units[index].UnitState = Moving;
	int cellID = 0;
	Map::Coord CellXY;
	Map::Coord NavXY;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	int CurrentCell = map.GetCurrentCell(UX, UY);
	CellXY = map.CellToGrid(CurrentCell);
	int curCellX = CellXY.X;
	int curCellY = CellXY.Y;
	int tarCellX = 0;
	int tarCellY = 0;
	int index2 = 0;
	int prevRange = 100;
	int curRange = 0;

	for( int y = -AttackRange; y <= AttackRange; y++)
	{
		if(CellXY.Y + y >= 0 && CellXY.Y + y < map.MapHeight)
		{
			for( int x = -AttackRange; x <=AttackRange; x++)
			{
				if(CellXY.X + x >= 0 && CellXY.X + x < map.MapWidth)
				{
					tarCellX = curCellX + x;
					tarCellY = curCellY + y;
					curRange = FindDistance(curCellX, curCellY, tarCellX, tarCellY);
					
					if( curRange <= AttackRange && curRange <= prevRange)
					{

						cellID = tarCellX + (tarCellY * map.MapWidth);
						if(Player == 1)
						{
							if(map.CheckCell(cellID) == map.Player2Unit)
							{
								prevRange = curRange;
								Units[ index ].UnitState = Attacking;
								Units[ index ].ClosestEnemyCell = cellID;
								Units[ index ].NavPoint = cellID;
	if(Units[ index ].NavPoint > 252)
	{
		int a = 1;
	}

								if(Intel.TeamTarget == -1)
								{
									Intel.TeamTarget = cellID;
								}
							}
							if(map.CheckCell(cellID) == map.Player2Building)
							{
								Units[ index ].UnitState = Attacking;
								Intel.FoundEnemyBase = true;
								Intel.EnemyBaseCell = cellID;
							}
						}
						else
						{
							if(map.CheckCell(cellID) == map.Player1Unit)
							{
								prevRange = curRange;
								Units[ index ].UnitState = Attacking;
								Units[ index ].ClosestEnemyCell = cellID;
								Units[ index ].NavPoint = cellID;
	if(Units[ index ].NavPoint > 252)
	{
		int a = 1;
	}
								if(Intel.TeamTarget == -1)
								{
									Intel.TeamTarget = cellID;
								}
							}
							if(map.CheckCell(cellID) == map.Player1Building)
							{
								Units[ index ].UnitState = Attacking;
								Intel.FoundEnemyBase = true;
								Intel.EnemyBaseCell = cellID;
							}
						}
					}
				}
			}
		}
	}
}

void Unit::Assess(Unit** Friend, Unit** Foe, Map& map, int& index)
{
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	Units[ index ].CurrentCell = map.GetCurrentCell(UX, UY);
	int distance = -1;

	if(Intel.TeamTarget != -1)
	{
		distance = FindCellDistance(Intel.TeamTarget, Units[ index ].CurrentCell, map);
		if(distance <= AttackRange)
		{
			FindUnitInCell(Intel.TeamTarget, Foe, map);
			if(Enemy.UnitsInCell <= 0)
			{
				Intel.TeamTarget = -1;
			}
		}
		for (int cIndex = 0; cIndex < NTotal; cIndex++)
		{
			if(Friend[ cIndex ]->Intel.FoundEnemyBase)
			{
				Intel.FoundEnemyBase = Friend[ cIndex ]->Intel.FoundEnemyBase;
				Intel.EnemyBaseCell = Friend[ cIndex ]->Intel.EnemyBaseCell;
			}
		}
	}
	if(Foe[ 9 ]->UnitCount < 1)
	{
		Intel.EnemyBaseCell = -1;
		Intel.FoundEnemyBase = false;
	}
}

void Unit::Choose( int& index )
{
	if(Units[ index ].ClosestEnemyCell != -1)
	{
		Units[ index ].NavPoint = Units[ index ].ClosestEnemyCell;
	if(Units[ index ].NavPoint > 252)
	{
		int a = 1;
	}
	}
	else
	{
		if(Intel.TeamTarget != -1)
		{
			Units[ index ].NavPoint = Intel.TeamTarget;
	if(Units[ index ].NavPoint > 252)
	{
		int a = 1;
	}
		}
		else
		{
			if(Intel.FoundEnemyBase)
			{
				Units[ index ].NavPoint = Intel.EnemyBaseCell;
			}
	if(Units[ index ].NavPoint > 252)
	{
		int a = 1;
	}
			else
			{
				if(Units[ index ].NavPoint <= 0 || Units[ index ].NavPoint > (12 * 21))
				{
					Units[ index ].NavPoint = rand() % (21 * 12);
					if(Units[ index ].NavPoint > 12 * 21)
					{
						int a = 1;
					}
				}
			}
		}
	}
	Units[ index ].UnitState = Searching;
}

void Unit::TransitionState(Unit **Foe, Unit** Friend,  Map &map, D3DGraphics &gfx)
{
	for (int index = 0; index < Units.size();index++) 
	{
		if(Units[ index ].Health <= 0)
		{
			Units[ index ].UnitState = Dead;
		}
		switch (Units[ index ].UnitState)
		{
		case Assessing:
			{
				Assess(Friend, Foe, map, index);
			}
		case Choosing:
			{
				Choose(index);
			}
			Draw( gfx, map, index );
			break;
		case Searching:
			FindCellsInRange(map, index, Foe, gfx);
			Draw( gfx, map, index );
			break;

		case Moving:
			Move(map, index);
			Draw( gfx, map, index );
			break;

		case Attacking:			
			Attack(map, Foe, index, gfx);
			Draw( gfx, map, index );	
			break;

		case Dead:
			UnitIsAlive(map, index);
			break;

		}

		GiveIntel(Friend);
	}
}

Marine::Marine( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	
	BankCost = 50;
	AttackDamage = 6; 
	NAttacks = 1;
	AttackSpeed = .78;
	Movement = 4;
	AttackRange = 3;
	Armor = 0;
	StartingHealth = 45;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = 32;
	LifeYoff = LifeXoff;
	
	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;

	Flags = Light;
}

Zergling::Zergling( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	
	BankCost = 25;
	AttackDamage = 5; 
	NAttacks = 1;
	AttackSpeed = .86;
	Movement = 5;
	AttackRange = 1;
	Armor = 0;
	StartingHealth = 35;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = 32;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;

	Flags = Light;
}

Sniper::Sniper( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	
	BankCost = 150;
	AttackDamage = 15; 
	NAttacks = 1;
	AttackSpeed = 1.5;
	Movement = 3;
	AttackRange = 4;
	Armor = 0;
	StartingHealth = 85;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 15;

	Flags = Light;
}

Tank::Tank( std::wstring basename, unsigned int nSurfaces, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, 1,1,1,nHoldFrames, key, Player ),
		surf1(std::wstring(L"Tank0000.bmp"), D3DCOLOR_XRGB(0,0,0)),
		surf2(std::wstring(L"Tank0001.bmp"), D3DCOLOR_XRGB(255,255,255))
{
	
	BankCost = 400;
	AttackDamage = 20; 
	NAttacks = 1;
	AttackSpeed = 1.8;
	Movement = 2;
	AttackRange = 8;
	Armor = 1;
	StartingHealth = 90;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	TransitionDelay = 3.0f;
	SplashRadius = 1;
	SplashDamage = 0.2f;

	UnitBonuses.ArmoredBonus = 5;
	UnitBonuses.LightBonus = 0;
	TypeOfUnit.TState = TypeOfUnit.Mobile;
	TypeOfUnit.Attacktimer.StopWatch();
	TypeOfUnit.AttackTimerStarted = false;

	Flags = Armored;
}

void Tank::Draw( D3DGraphics& gfx, Map& map, int& index )
{	
	if( MapNM == 1 )
	{
		if(Units[index].TState == Units[index].Mobile)
		{
			if( Player == 1 )
			{
				if( Units[ index ].Health > 0 )
				{
					surf1.Draw( Units[index].x - DrawXoff, Units[index].y - DrawYoff, gfx ); 
					gfx.DrawStatusLifeBar( Units[ index ].x - 16, Units[ index ].y - DrawYoff, StartingHealth, Units[ index ].Health, 1, DrawXoff );
				}
			}
			else
			{
				if( Units[ index ].Health > 0 )
				{
					surf1.BackwardsDraw( Units[index].x + DrawXoff, Units[index].y - DrawYoff, gfx ); 
					gfx.DrawStatusLifeBar( Units[index].x + DrawXoff, Units[index].y - DrawYoff, StartingHealth, Units[ index ].Health, 2, DrawXoff );
				}
			}
		}
		else
		{
			if( Player == 2 )
			{
				if( Units[ index ].Health > 0 )
				{
					surf2.Draw( Units[index].x - DrawXoff, Units[index].y - DrawYoff, gfx ); 
					gfx.DrawStatusLifeBar( Units[ index ].x - 16, Units[ index ].y - DrawYoff, StartingHealth, Units[ index ].Health, 1, DrawXoff );
				}
			}
			else
			{
				if( Units[ index ].Health > 0 )
				{
					surf2.BackwardsDraw( Units[index].x + DrawXoff, Units[index].y - DrawYoff, gfx ); 
					gfx.DrawStatusLifeBar( Units[index].x + DrawXoff, Units[index].y - DrawYoff, StartingHealth, Units[ index ].Health, 2, DrawXoff );
				}
			}
		}
		if( index >= Units.size())
		{
			index = Units.size();
		
		}
	}
}

void Tank::Move( Map& map, int& index )
{

	Map::Coord CellXY;
	Map::Coord NavXY;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	int CurrentCell = map.GetCurrentCell(UX, UY);
	CellXY = map.CellToGrid(CurrentCell);
	int CellX = (CellXY.X * map.CellSize) + 32;
	int CellY = (CellXY.Y * map.CellSize) + 32;
	NavXY = map.CellToGrid(Units[ index ].NavPoint);
	int NX = NavXY.X * (map.CellSize) + 32;
	int NY = NavXY.Y * (map.CellSize) + 32;
	float Rise = FindRise(CellY, NY);
	float Run = FindRun(CellX, NX);
	float Slope = FindSlope(Rise, Run);

	Transition( index, Units[ index ].Mobile);

	if( Units[ index ].TState == Units[ index ].Mobile)
	{
		if(Player == 1 && map.CheckCell(CurrentCell) != map.Player1Building)
		{
			map.SetCell(CurrentCell, map.Player1Unit);
		}
		else if(Player == 2 && map.CheckCell(CurrentCell) != map.Player2Building)
		{
			map.SetCell(CurrentCell, map.Player2Unit);
		}

		if(Rise < Run)
		{
			if(CellX < NX)
			{
				UX += Movement;
			}
			else
			{
				UX -= Movement;
			}
			if(CellY < NY)
			{
				UY += Movement * Slope;
			}
			else
			{
				UY -= Movement * Slope;
			}
		}
		else if(Rise > Run)
		{
			if(CellX < NX)
			{
				UX += Movement * Slope;
			}
			else
			{
				UX -= Movement * Slope;
			}
			if(CellY < NY)
			{
				UY += Movement;
			}
			else
			{
				UY -= Movement;
			}
		}
		else
		{
			if(CellX < NX)
			{
				UX += Movement;
			}
			else
			{
				UX -= Movement;
			}
			if(CellY < NY)
			{
				UY += Movement;
			}
			else
			{
				UY -= Movement;
			}

		}
		if(UX < 32)
		{
			UX = 32;
		}
		else if(UX > SCREENWIDTH - 32)
		{
			UX = SCREENWIDTH - 32;
		}

		if(UY < 32)
		{
			UY = 32;
		}
		else if(UY > SCREENHEIGHT - 32)
		{
			UY = SCREENHEIGHT - 32;
		}

		int NextCell = map.GetCurrentCell(UX, UY);

		if(CurrentCell == NextCell)
		{
			Units[ index ].UnitState = Moving;
		}
		else
		{		
			Units[ index ].UnitState = Searching;
			map.SetCell(CurrentCell, map.Empty);

			if(Player == 1)
			{
				if(map.CheckCell(NextCell) == map.Empty)
				{
					map.SetCell(NextCell, map.Player1Unit);
				}				
			}
			else
			{
				if(map.CheckCell(NextCell) == map.Empty)
				{
					map.SetCell(NextCell, map.Player2Unit);
				}
			}
		}
		if(NextCell == Units[ index ].NavPoint)
		{
			Units[ index ].NavPoint = 0;
		}
		Units[ index ].x = UX;
		Units[ index ].y = UY;
		Units[ index ].CurrentCell = NextCell;
	}

	
}

void Tank::Transition(int& index, UnitStats::TankState DesiredState)
{
	if(DesiredState != Units[ index ].TState)
	{
		if(Units[ index ].TransitionStarted)
		{
			if(Units[ index ].transTimer.GetTimeMilli() >= TransitionDelay * 1000)
			{
				Units[ index ].TState = DesiredState;
				Units[ index ].TransitionStarted = false;
				Units[ index ].transTimer.StopWatch();
			}
		}
		else
		{
			Units[ index ].transTimer.StartWatch();
			Units[ index ].TransitionStarted = true;
		}
	}
}

void Tank::Attack(Map& map, Unit **Foe, int& index, D3DGraphics &gfx )
{
	int index2;
	int classloop;

	int target = Units[index].NavPoint;
	int SplashTargets[indexMax];
	int TargetIndex = 0;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	Units[ index ].CurrentCell = map.GetCurrentCell(UX, UY);
	bool TargetFound[indexMax];
	int oppCell;
	int DamageDealt = 0;
	int UnitNumber[indexMax];
	int UnitIndex[indexMax];
	int Count;
	if(Player == 1)
	{
		map.SetCell(Units[ index ].CurrentCell, map.Player1Unit);
	}
	else
	{
		map.SetCell(Units[ index ].CurrentCell, map.Player2Unit);
	}
	
	
	Transition( index, Units[ index ].Siege);
	
	if( Units[index].TState == Units[ index ].Siege)
	{

		if(IsReadyToAttack( index ))
		{
			FindUnitInCell(target, Foe, map);
			if(Enemy.UnitsInCell == 0)
			{
				Units[ index ].UnitState = Assessing;
			}
			else
			{
				for(int i = 0; i < Enemy.UnitsInCell; i++)
				{
				
					DamageDealt += (CalculateDamage( index, Foe ) *  SplashDamage) - Foe[ Enemy.cIndex[ i ] ]->Armor;

					if(Foe[ Enemy.cIndex[ i ] ]->Units[ Enemy.uIndex[ i ] ].Health > 0)
					{
						Foe[ Enemy.cIndex[ i ] ]->Units[ Enemy.uIndex[ i ] ].Health -= DamageDealt;
						Units[ index ].UnitState = Attacking;
					}
					else
					{
						Foe[ Enemy.cIndex[ i ] ]->Units[ Enemy.uIndex[ i ] ].UnitState = Dead;
						if( i == Enemy.UnitsInCell)
						{
							map.SetCell(target, map.Empty);
							Units[ index ].UnitState = Searching;
						}
					}					
				}
			}
		}
	}
}

Base::Base( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	BankCost = 0;
	AttackDamage = 25; 
	AttackSpeed = 1.8;
	NAttacks = 1;
	AttackRange = 5;
	Armor = 1;
	StartingHealth = 1000;
	if(Player == 1)
	{
		DrawXoff = 32;
		DrawYoff = 32;
		LifeXoff = -20;
		LifeYoff = 10;
	}
	else
	{
		DrawXoff = 96;
		DrawYoff = 32;
		LifeXoff = -20;
		LifeYoff = 10;
	}
	Units.push_back(TypeOfUnit);
	UnitCount = Units.size();
	for( int index = 0; index < Units.size(); index++ )
	{
		Units[index].UnitState = Searching;
		Units[index].Health = StartingHealth;
		if( Player == 1 )
		{
			Units[index].x = 64;
		}

		else
		{
			Units[index].x = 1248;
		}
		Units[index].y = 350;
		Units[ index ].CurrentSurface = 0;
	}

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;
	Flags = Armored;
}

void Base::TransitionState(Unit **unit, Map &map, D3DGraphics &gfx)
{
	for ( int index = 0; index < UnitCount; index++)
	{
		int UX = Units[ index ].x;
		int UY = Units[ index ].y;
		if(Player == 1)
		{
			map.SetCell(map.GetCurrentCell(UX, UY), map.Player1Building);	
		}
		else
		{
			map.SetCell(map.GetCurrentCell(UX, UY), map.Player2Building);
		}
		switch (Units[ index ].UnitState)
		{
		case Assessing:
		case Choosing:
		case Searching:
			FindCellsInRange(map, index, unit, gfx);
			Draw( gfx, map, index );
			break;
			
		case Moving:
			Units[ index ].UnitState = Assessing;
			break;

		case Attacking:			
			Attack(map, unit, index, gfx);
			Draw( gfx, map, index );	
			break;

		case Dead:
			UnitIsAlive(map, index);
			break;

		}
	}
}

void Base::FindCellsInRange(Map &map, int& index, Unit **unit, D3DGraphics &gfx )
{
	Units[index].ClosestEnemyCell = 0;
	int cellID = 0;
	Map::Coord CellXY;
	int UX = Units[ index ].x;
	int UY = Units[ index ].y;
	Units[ index ].CurrentCell = map.GetCurrentCell(UX, UY);
	CellXY = map.CellToGrid(Units[ index ].CurrentCell);
	int curCellX = CellXY.X;
	int curCellY = CellXY.Y;
	int tarCellX = 0;
	int tarCellY = 0;
	int prevRange = 100;
	int curRange = 0;

	for( int y = -AttackRange; y <= AttackRange; y++)
	{
		if(CellXY.Y + y >= 0 && CellXY.Y + y < map.MapHeight)
		{
			for( int x = -AttackRange; x <=AttackRange; x++)
			{
				if(CellXY.X + x >= 0 && CellXY.X + x < map.MapWidth)
				{
					tarCellX = curCellX + x;
					tarCellY = curCellY + y;
					curRange = FindDistance(curCellX, curCellY, tarCellX, tarCellY);
					
					if( curRange <= AttackRange && curRange <= prevRange)
					{
						prevRange = curRange;
						cellID = tarCellX + (tarCellY * map.MapWidth);
						if(Player == 1)
						{
							if(map.CheckCell(cellID) == map.Player2Unit)
							{
								Units[ index ].UnitState = Attacking;
								Units[ index ].ClosestEnemyCell = cellID;
								Units[ index ].NavPoint = cellID;
							}
						}
						else
						{
							if(map.CheckCell(cellID) == map.Player1Unit)
							{
								Units[ index ].UnitState = Attacking;
								Units[ index ].ClosestEnemyCell = cellID;
								Units[ index ].NavPoint = cellID;
							}
						}
					}
				}
			}
		}
	}
}

Marauder::Marauder( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{	
	BankCost = 125;
	AttackDamage = 10;
	NAttacks = 1;
	AttackSpeed = 1.2;
	Movement = 2.5;
	AttackRange = 3;
	Armor = 1;
	StartingHealth = 95;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 10;
	UnitBonuses.LightBonus = 0;

	Flags = Armored;
}

Stalker::Stalker( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	BankCost = 125;
	AttackDamage = 12; 
	NAttacks = 1;
	AttackSpeed = 1.44;
	Movement = 2.7;
	AttackRange = 4;
	Armor = 1;
	StartingHealth = 105;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 6;
	UnitBonuses.LightBonus = 0;

	Flags = Armored;
}

Roach::Roach( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	BankCost = 75;
	AttackDamage = 16; 
	NAttacks = 1;
	AttackSpeed = 1.44;
	Movement = 2.7;
	AttackRange = 2;
	Armor = 1;
	StartingHealth = 75;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;

	Flags = Armored;
}

Hydralisk::Hydralisk( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	BankCost = 100;
	AttackDamage = 12; 
	NAttacks = 1;
	AttackSpeed = .76;
	Movement = 3;
	AttackRange = 4;
	Armor = 0;
	StartingHealth = 60;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;

	Flags = Light;
}

Zealot::Zealot( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player )
	:
	Unit( basename, NMoveS, NAttackS, NIdleS, nHoldFrames, key, Player )
{
	BankCost = 100;
	AttackDamage = 12; 
	NAttacks = 2;
	AttackSpeed = 1.2;
	Movement = 2.5;
	AttackRange = 1;
	Armor = 0;
	StartingHealth = 120;
	DrawXoff = 32;
	DrawYoff = DrawXoff;
	LifeXoff = 0;
	LifeYoff = LifeXoff;
	LifeXoff = DrawXoff;
	LifeYoff = LifeXoff;

	UnitBonuses.ArmoredBonus = 0;
	UnitBonuses.LightBonus = 0;

	Flags = Light;
}


