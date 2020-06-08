#pragma once
#include <time.h>  //Для clock_gettime
#include <stdlib.h>
#include <iostream>

#define AZIMUTH_SIZE 16384
#define ELEVATION_SIZE 4096
#define BEAM_SIZE 511
#define DELIMITER "|"


struct point {
	unsigned range;
	unsigned power;
};

struct beam {
	point* pointArr;
	unsigned arrSize;
	timespec time;

};

enum Status {
	start,
	filling,
	getting,
	failure
};

enum retValue { // возвращаемое значение для метода add
	END_OF_DIRECTION = 0,  // коды можно поменять
	CONTINUE_DIRECTION = 1,
	FAILURE = 2
};

class MeteoMap
{
public:
	MeteoMap();
	~MeteoMap();
	int add(unsigned codogram[8]);
	//void printBeam(unsigned azimuth, unsigned elevation); 
	const beam* getLastHandledBeam();
	//beam getBeam(unsigned azimuth, unsigned elevation);
private:
	beam*** spaceMap;  
	beam* lastBeam;
	beam* currentBeam;
	int currentPointIndex;
	beam* newBeam;
	Status status;
	unsigned failureCodogram[8];
	
	void recalcPower(const point oldPoint, const timespec oldTime, point* newPoint);
	unsigned getActuaPower(unsigned power, timespec time);
	void justNewBeam();
	void handleSegment(unsigned codogramLine);
	void finalizeFilling(unsigned azimuth, unsigned elevation);
	void initNewDirection();
};

