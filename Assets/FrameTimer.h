#pragma once

#include "Timer.h"
#include "float.h"

class FrameTimer
{
public:
	FrameTimer()
		:
	timeMin( FLT_MAX ),
	timeMax( 0.0f ),
	timeSum( 0.0f ),
	lastMin( 0.0f ),
	lastMax( 0.0f ),
	lastAvg( 0.0f ),
	frameCount( 0 ),
	lastfpsmin(0.0f),
	lastfpsmax(0.0f),
	lastfpsavg(0.0f)
	{}
	void StartFrame()
	{		
		timer.StartWatch();
	}
	void StopFrame()
	{
		timer.StopWatch();
		const float frameTime = timer.GetTimeMilli();
		timeSum += frameTime;
		timeMin = min( timeMin,frameTime );
		timeMax = max( timeMax,frameTime );
		
		frameCount++;
		NewAvg = 1000 / timeSum * frameCount;
		if( frameCount >= nFramesAvg )
		{
			lastAvg = timeSum / (float)nFramesAvg;
			lastMin = timeMin;
			lastMax = timeMax;
			lastfpsmin = 1000 / lastMax;
			lastfpsmax = 1000 / lastMin;
			lastfpsavg = 1000 / lastAvg;
			timeSum = 0.0f;
			timeMin = FLT_MAX;
			timeMax = 0.0f;
			frameCount = 0;
		}
	}
	float GetAvg() const
	{
		return lastAvg;
	}
	float GetMin() const
	{
		return lastMin;
	}
	float GetMax() const
	{
		return lastMax;
	}
	float GetfpsAvg() const
	{
		return NewAvg;
	}
	float GetfpsMin() const
	{
		return lastfpsmin;
	}
	float GetfpsMax() const
	{
		return lastfpsmax;
	}
	

private:
	static const int nFramesAvg = 60;
	Timer timer;
	float timeSum;
	float timeMin;
	float timeMax;
	int frameCount;
	float lastMin;
	float lastMax;
	float lastAvg;
	float lastfpsmin;
	float lastfpsmax;
	float lastfpsavg;
	int NewAvg;
};