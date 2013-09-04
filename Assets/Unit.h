#pragma once
#include "SurfaceSequence.h"
#include "Timer.h"
#include <vector>
#include "D3DGraphics.h"
#include "Map.h"
#include "Funcs.h"

#define NumberOfUnitType 9
#define NumberOfBuildings 1
#define NTotal NumberOfUnitType + NumberOfBuildings
#define NTargets 100
 

class Unit
{
public:

	Unit( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
	~Unit();

	// Enumerations
	enum State
	{
		Assessing,
		Choosing,
		Searching,
		Moving,
		Attacking,
		Dead
	};

	enum UnitFlags
	{
		Light,
		Armored
	};

	// Sturctures
	struct TeamStats
	{
		int EnemyBaseCell;
		int TeamTarget;
		bool FoundEnemyBase;
	};

	struct UnitStats
	{	
		int x;
		int y;
		float Health;
		int ClosestEnemyCell;
		int CurrentCell;
		State UnitState;
		Timer Attacktimer;
		Timer transTimer;
		int CurrentSurface;
		int NavPoint;
		bool AttackTimerStarted;

		enum TankState
		{
			Mobile,
			Siege
		};
		TankState TState;
		bool TransitionStarted;

	};

	struct UnitBonuses
	{
		int LightBonus;
		int ArmoredBonus;
	};

	struct Helper
	{
		int uIndex[NTargets];
		int cIndex[NTargets];
		bool TargetFound;
		int UnitsInCell;
	};

	// Functions
	virtual void Draw( D3DGraphics& gfx, Map& map, int& index);
	virtual void Move( Map& map, int& index);
	virtual void Attack(Map& map, Unit **Foe, int& index, D3DGraphics &gfx );
	void Assess( Unit** Friend, Unit** Foe, Map& map, int &index);
	void Choose( int& index );
	bool IsReadyToAttack( int &index );
	float CalculateDamage(int& index, Unit** Foe);
	void FindUnitInCell(int& TestCell, Unit** Foe, Map &map);
	void GiveIntel(Unit** Friend);
	void ChoosePath(Unit** Foe, Map& map, int& index);
	bool AddUnit(Map &map );
	void UnitIsAlive( Map& map, int& index );
	virtual void FindCellsInRange( Map &map, int& index, Unit **Foe, D3DGraphics &gfx );
	virtual void TransitionState(Unit **Foe, Unit** Friend, Map &map, D3DGraphics &gfx);

	float FindCellDistance( int& Cell1, int& Cell2, Map &map);

	// Members
protected:
	float AttackDamage;
	float AttackSpeed;
	float Movement;
	float StartingHealth;
	int DrawXoff, DrawYoff;
	int LifeXoff, LifeYoff;
	int SpawnPointX, SpawnPointY;
	TeamStats Intel;
	Helper Enemy;
	int TotalUnits;

public:

	std::vector<UnitStats> Units;
	int AttackRange;	
	int NAttacks;
	UnitFlags Flags;
	UnitStats TypeOfUnit;
	SurfaceSequence Move_ani;
	SurfaceSequence Attack_ani;
	SurfaceSequence Idle_ani;
	UnitBonuses UnitBonuses;
	int Armor;
	int BankCost;
	int Player;
	int UnitCount;
	bool EnemyFoundBase;
	int EnemyFoundBaseCell;
};

class Marine : public Unit
{
public:
	Marine( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
	
};

class Zergling : public Unit
{
public:
	Zergling( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Sniper : public Unit
{
public:
	Sniper( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Tank : public Unit
{
public:
	Tank( std::wstring basename, unsigned int nSurfaces, unsigned int nHoldFrames, D3DCOLOR key, int Player );
	void Draw( D3DGraphics& gfx, Map& map, int& index );
	void Move( Map& map, int& index );
	void Attack(Map& map, Unit **unit, int& index, D3DGraphics &gfx );
	void Transition(int& index, UnitStats::TankState DesiredState);
	float TransitionDelay;
	int SplashRadius;
	float SplashDamage;
	KeyedSurface surf1;
	KeyedSurface surf2;
};


class Marauder : public Unit 
{
public:
	Marauder( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Stalker : public Unit 
{
public:
	Stalker( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Roach : public Unit 
{
public:
	Roach( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Hydralisk : public Unit 
{
public:
	Hydralisk( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};

class Zealot : public Unit 
{
public:
	Zealot( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
};


class Base : public Unit
{
public:
	Base( std::wstring basename, unsigned int NMoveS, unsigned int NAttackS, unsigned int NIdleS, unsigned int nHoldFrames, D3DCOLOR key, int Player );
	virtual void TransitionState(Unit **Foe, Map &map, D3DGraphics &gfx);
	virtual void FindCellsInRange( Map &map, int& index, Unit **Foe, D3DGraphics &gfx );
};

