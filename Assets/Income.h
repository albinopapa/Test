#pragma once
#include "Timer.h"

class Income
{
public:
	Income();
	void GetIncome();
	int GetRemainingTime();
	int Bank;
	int IncomeAmount;
	int BankComp;
	int IncomeAmountComp;
private:
	Timer timer;
	Timer timeremaining;
	int time;
	int x;
	int x2;
	int TimeRemaining;
};