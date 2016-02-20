#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <Box2D.h>
#include "Spline/spline.h"

void SmoothMotionInfo::init()
{
	times.push_back(0.0f);
	times.push_back(0.5f);
	times.push_back(1.0f);
	
	points.push_back(b2Vec2(0.0f,0.0f));
	points.push_back(b2Vec2(0.0f,0.0f));
	points.push_back(b2Vec2(0.0f,0.0f));
	
	pointCount+=3;

}

void SmoothMotionInfo::clear()
{
	times.clear();
	points.clear();
	pointCount=0;
}

void SmoothMotionInfo::SetPoint(int index, b2Vec2 point)
{
	if(index<pointCount)
	{
		float time = (float)index/(float)pointCount;
		points[index]= point;
		times[index]=time;
	}

}
void SmoothMotionInfo::initWithSize(int cnt)
{
	pointCount=0;
	for(int i=0;i<cnt;i++)
	{
		times.push_back(0.0f);
		points.push_back(b2Vec2(0.0f,0.0f));
		timesCopy.push_back(0.0f);
		pointsCopy.push_back(b2Vec2(0.0f,0.0f));

		pointCount++;
	}
}

b2Vec2 SmoothMotionInfo::interpolate()
{	
	/* Create the spline interpolating the position over time */
	Spline<float, b2Vec2> sp(times, points);
	b2Vec2 result = sp[0.5f];
	return result;
}


//interpolates all the points from minTime to maxTime
void SmoothMotionInfo::interPolateAll()
{	
	//create a copy of all the points

	
	for(int i=0;i<pointCount;i++)
	{
		timesCopy[i]=times[i];
		pointsCopy[i]=points[i];	
	}

	Spline<float, b2Vec2> sp(timesCopy, pointsCopy);

	for(int i=0;i<pointCount;i++)
	{
		float time = (float)i/(float)pointCount;
		points[i]= sp[time];
	
	}
}


