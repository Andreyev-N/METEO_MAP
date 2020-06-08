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
	lastBeam = NULL;
	status = start;
}

MeteoMap::~MeteoMap(){
}

const beam* MeteoMap::getLastHandledBeam() {
		return lastBeam;
}


int MeteoMap::add(unsigned codogram[8]) {
	if (status == failure) {
		status = start;
		add(failureCodogram);
	}
	unsigned azimuth = codogram[1] & 0x3FFF;
	unsigned elevation = codogram[2];
	currentBeam = spaceMap[azimuth][elevation];
	if (status == getting || status == start) {
		initNewDirection();
		if (currentBeam == NULL) {
			justNewBeam();
		} 
	} else {
		if (currentBeam != spaceMap[azimuth][elevation]) {
			for (int i = 0; i < 8; i++) {
				failureCodogram[i] = codogram[i];
			}
			status = failure;
			return FAILURE;
		} 
	}
	for (int j = 3; j < 8; j++) {
		if (codogram[j] == NULL) {
			finalizeFilling(azimuth, elevation);
			return END_OF_DIRECTION;
		}
		handleSegment(codogram[j]);
	}
	return CONTINUE_DIRECTION;
}

void MeteoMap::initNewDirection() {
	status = filling;
	currentPointIndex = 0;
	newBeam = new beam;
	newBeam->arrSize = 0;
}

void MeteoMap::handleSegment(unsigned segment) {
	unsigned power = (segment & 0xE000) >> 13;
	unsigned size = (segment & 0x1E00) >> 9;
	unsigned range = segment & 0x01FF;
	for (unsigned newPointRange = range; newPointRange < (range + size); newPointRange++) { //перебираем отрезок по точкам
		point newPoint = { range, power };
		if (newPointRange < currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize] = newPoint; // просто добавл€ем в конец 
			newBeam->arrSize++;
		} else if (newPointRange == currentBeam->pointArr[currentPointIndex].range){
			recalcPower(currentBeam->pointArr[currentPointIndex], currentBeam->time, &newPoint);
			newBeam->pointArr[newBeam->arrSize] = newPoint;
			newBeam->arrSize++;
			currentPointIndex++;
		} else if (newPointRange > currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize].power = getActuaPower(
				currentBeam->pointArr[currentPointIndex].power, currentBeam->time);
			newBeam->arrSize++;
			currentPointIndex++;
			newPointRange--; // то есть в следующем цикле мы будем провер€ть эту же точку,
			                // но сравнивать будем со следующей из currentBeam
		}
	}
}

void MeteoMap::justNewBeam(){
			currentBeam = new beam;
			currentBeam->pointArr = new point[BEAM_SIZE];
			currentBeam->arrSize = 0;
		}

void MeteoMap::recalcPower(const point oldPoint, const timespec oldTime, point* newPoint){
	//
}

unsigned MeteoMap::getActuaPower(unsigned power, timespec time) {
	return power;
}

void MeteoMap::finalizeFilling(unsigned azimuth, unsigned elevation) {
	timespec_get(&newBeam->time, TIME_UTC);
	spaceMap[azimuth][elevation] = newBeam;
	lastBeam = newBeam;
	status = getting;
}


	
	/*for (int i = 3; i < 8; i++) { // дл€ каждой строчки
		if (codogram[i] == 0)
			return true;
		else {
			beam* Beam = spaceMap[azimuth][elevation];
			if (Beam == NULL) {  // не существует направлени€
				Beam = new beam;
				Beam->pointArr = new point[BEAM_SIZE];
				//Beam->pointArr = &Point;
				Beam->arrSize = 0;
				Beam->time = {0,0};
				timespec_get(&Beam->time, TIME_UTC);
			}
			point p;
			p.power = (codogram[i] & 0xE000) >> 13;
			unsigned size = (codogram[i] & 0x1E00) >> 9;
			unsigned range = codogram[i] & 0x01FF;
			for (int j = range; j < (int)(range + size); j++) {
				p.range = j;
				newPoint(azimuth, elevation, p);
			}
		}
	}
	return false;
}

void MeteoMap::newPoint(unsigned azimuth, unsigned elevation, point Point) {
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
	return p.power; //здесь будет мощность умноженна€ на весовой коэффициент
}

void MeteoMap::recalcPower(point newPoint, point* oldPoint) {
	//алгоритм пересчета мощности
}*/

