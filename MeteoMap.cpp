#include "pch.h"
#include "MeteoMap.h"

using namespace std;

MeteoMap::MeteoMap() : spaceMap(){
	spaceMap = new beam**[AZIMUTH_SIZE];
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
	
		spaceMap[i] = new beam*[ELEVATION_SIZE];
		for (int j = 0; j < ELEVATION_SIZE; j++) {
			spaceMap[i][j] = NULL;
		}
		
	}
}

MeteoMap::~MeteoMap(){
}

beam MeteoMap::getLastHandledBeam() {
	beam* b = NULL;
	return *b; //функция-заглушка
}

beam getBeam(unsigned azimuth, unsigned elevation) {
	beam* b = NULL;
	return *b;
}

bool MeteoMap::add(unsigned codogram[8]) {
	if (codogram[0] == NULL) {
		return false;
	}
	//unsigned type = codogram[0] & 0x00FF; 
	unsigned azimuth = codogram[1] & 0x3FFF;
	unsigned elevation = codogram[2];
	for (int i = 3; i < 8; i++) {
		if (codogram[i] == 0)
			return true;
		else {
			point p;
			p.power = codogram[i] & 0xE000;
			unsigned size = codogram[i] & 0x1E00;
			unsigned range = codogram[i] & 0x01FF;
			for (int j = range; j < (int)(range + size); j++) {
				p.range = j;
				newPoint(azimuth, elevation, p);
			}
		}
	}
	return true;
}

void MeteoMap::newPoint(unsigned azimuth, unsigned elevation, point Point) {
	beam* Beam = spaceMap[azimuth][elevation];
	if (Beam == NULL) {
		Beam = new beam; 
		Beam->pointArr = new point[BEAM_SIZE];
		Beam->pointArr = &Point;
		Beam->arrSize = 1;
		timespec time;
		timespec_get(&time, TIME_UTC);
		Beam->time = time;
	}
	else {
			handleBeam(Beam, Point);
	}
}

void MeteoMap::handleBeam(beam* Beam, point Point) {
	bool pointIsExist = false;
	for (int i = 0; i < (int)Beam->arrSize; i++) {	
		if (pointIsExist == false && Beam->pointArr[i].range == Point.range) {
			recalcPower(Point, &(Beam->pointArr[i]));
			pointIsExist = true;
		}
		Beam->pointArr[i].power = getActuaPower(Point, Beam->time);
	}
	if (!pointIsExist) {
		Beam->pointArr[Beam->arrSize] = Point;
		Beam->arrSize++;
	}
}

int MeteoMap::getActuaPower(point p, timespec time) {
	return p.power; //здесь будет мощность умноженная на весовой коэффициент
}

void MeteoMap::recalcPower(point newPoint, point* oldPoint) {
	//алгоритм пересчета мощности
}

