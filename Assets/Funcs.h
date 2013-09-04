#pragma once

#include <math.h>

static float FindSlope( float& rise, float& run )
{
	if( rise < run )
	{
		return rise / run;
	}

	else if(rise > run)
	{
		return run / rise;
	}

	else
	{
		return 0;
	}
}

static float FindRise( int& Point1Y, int& Point2Y )
{
	return abs( Point1Y - Point2Y );
}

static float FindRun( int& Point1X, int& Point2X )
{
	return abs( Point1X - Point2X );
}

static float FindDistance( int& Point1X, int& Point1Y, int& Point2X, int& Point2Y )
{
	float distX = FindRun(Point1X, Point2X);
	float distY = FindRise(Point1Y, Point2Y);
	
	return sqrt( (distX*distX) + (distY*distY) );
}