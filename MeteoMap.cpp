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
			initNewBeam();
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

void MeteoMap::initNewBeam() {
	currentBeam = new beam;
	currentBeam->pointArr = new point[BEAM_SIZE];
	currentBeam->arrSize = 0;
}

void MeteoMap::handleSegment(unsigned segment) {
	unsigned power = (segment & 0xE000) >> 13;
	unsigned size = (segment & 0x1E00) >> 9;
	unsigned range = segment & 0x01FF;
	for (unsigned newPointRange = range; newPointRange < (range + size); newPointRange++) { //перебираем отрезок по точкам
		point newPoint = { range, power };
		if (newPointRange < currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize] = newPoint; // просто добавляем в конец 
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
			newPointRange--; // то есть в следующем цикле мы будем проверять эту же точку,
			                // но сравнивать будем со следующей из currentBeam
		}
		if (newBeam->pointArr[newBeam->arrSize].power == 0) {
			newBeam->arrSize--; //то есть мы удаляем точку из памяти, если ее мощьность нулевая
		}
	}
}

void MeteoMap::recalcPower(const point oldPoint, const timespec oldTime, point* newPoint){
	timespec currentTime = { 0, 0 };
	unsigned oldPower = getActuaPower(oldPoint.power, oldTime);
	newPoint->power = NEW_POINT_COEF * newPoint->power + oldPower;
}

unsigned MeteoMap::getActuaPower(unsigned power, timespec time) {
	timespec currentTime = { 0, 0 };
	timespec_get(&currentTime, TIME_UTC);
	long dt_mlsec = (currentTime.tv_sec - time.tv_sec) * 1000 +
		(currentTime.tv_sec - time.tv_sec) / 1000000; // разница времени в милисекундах
	long weightCoef = 1 / (1 + U_COEF * pow(dt_mlsec, K_COEF));
	return power * weightCoef;
}

void MeteoMap::finalizeFilling(unsigned azimuth, unsigned elevation) {
	for (int i = currentPointIndex; i < currentBeam->arrSize; i++) {
		newBeam->pointArr[newBeam->arrSize].power = getActuaPower(
			currentBeam->pointArr[currentPointIndex].power, currentBeam->time);
		newBeam->arrSize++;
	}
	timespec_get(&newBeam->time, TIME_UTC);
	spaceMap[azimuth][elevation] = newBeam;
	lastBeam = newBeam;
	status = getting;
}
