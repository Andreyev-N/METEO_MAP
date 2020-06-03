#pragma once
#include <time.h>  //Äëÿ clock_gettime
#include <stdlib.h>
#include <iostream>

#define AZIMUTH_SIZE 16384
#define ELEVATION_SIZE 4096
#define BEAM_SIZE 511
#define DELIMITER "|"


struct point {
	unsigned range;
	int power;
};

struct beam {
	point* pointArr;
	unsigned arrSize;
	timespec time;

};

class MeteoMap
{
public:
	MeteoMap();
	~MeteoMap();
	bool add(unsigned codogram[8]);
	//void printBeam(unsigned azimuth, unsigned elevation); 
	beam getLastHandledBeam();
	beam getBeam(unsigned azimuth, unsigned elevation);
private:
	beam*** spaceMap;  

	void newPoint(unsigned azimuth, unsigned elevation, point point);
	void handleBeam(beam* Beam, point Point);
	void recalcPower(point newPoint, point* oldPoint);
	int getActuaPower(point p, timespec time);
};

