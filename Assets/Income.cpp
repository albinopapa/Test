#include "Income.h"

Income::Income()
	:
		x( 0 ),
		x2( 0 ),
		time( 10 ),
		Bank( 100 ),
		IncomeAmount( 1 ),
		BankComp( 100 ),
		IncomeAmountComp( 1 ),
		TimeRemaining( 10 )
	
	{}

void Income::GetIncome()
{
	if( x == 0 )
	{
		timer.StartWatch();
		x = 1;
	}
	if( timer.GetTimeMilli() >= time*1000 )
	{
		timer.StopWatch();
		Bank += IncomeAmount;
		BankComp += IncomeAmountComp;
		IncomeAmount += 25;
		IncomeAmountComp += 25;
		x = 0;
		TimeRemaining = 10;
	}
}

int Income::GetRemainingTime()
{
	if( x2 == 0 )
	{
		timeremaining.StartWatch();
		x2 = 1;
	}


	if( timeremaining.GetTimeMilli() >= 1000 )
	{
		if( TimeRemaining > 0 )
		{
			TimeRemaining--;
			timeremaining.StopWatch();
		}
		x2 = 0;
	}

	return TimeRemaining;
}
