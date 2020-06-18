
#include "pch.h"
#include "MeteoMap.h"

using namespace std;

MeteoMap::MeteoMap(long koeff_u, long koeff_k){
	koef_u = koeff_u;
	koef_k = koeff_k;
	spaceMap = new beam**[AZIMUTH_SIZE];
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
		spaceMap[i] = new beam*[ELEVATION_SIZE];
		for (int j = 0; j < ELEVATION_SIZE; j++) {
			spaceMap[i][j] = NULL;
		}
	}
	newBeam = new beam;
	newBeam->pointArr = new point[BEAM_SIZE];
	status = start;
}

MeteoMap::~MeteoMap(){
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
		for (int j = 0; j < ELEVATION_SIZE; j++) {
			if(spaceMap[i][j] != NULL)
				delete [] spaceMap[i][j]->pointArr;
			delete spaceMap[i][j];
		}
	}
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
		delete[] spaceMap[i];
	}
	delete [] spaceMap;
	delete [] newBeam->pointArr;
	delete newBeam;
	if (status == CONTINUE_DIRECTION) {
		delete[] currentBeam->pointArr;
		delete currentBeam;
	}
}

const beam* MeteoMap::getLastHandledBeam() {
		return lastBeam;
}


int MeteoMap::add(unsigned short codogram[8]) {
	unsigned azimuth = codogram[1] & 0x3FFF;
	unsigned elevation = codogram[2];
	if (status == filling) {
		if (azimuth != lastAzimuth || elevation != lastElevation) {
			finalizeFilling(lastAzimuth, lastElevation);
			return FAILURE;
		} 
	} else {
		lastAzimuth = azimuth;
		lastElevation = elevation;
		currentBeam = spaceMap[azimuth][elevation];
		initNewDirection();
	}
	for (int j = 3; j < 8; j++) {
		if (codogram[j] == NULL) {
			finalizeFilling(lastAzimuth, lastElevation);
			return END_OF_DIRECTION;
		}
		handleSegment(codogram[j]);
	}
	return CONTINUE_DIRECTION;
}

void MeteoMap::initNewDirection() {
	if (currentBeam == NULL) {
		currentBeam = new beam;
		currentBeam->pointArr = new point[BEAM_SIZE];
		currentBeam->arrSize = 0;
	} 
	status = filling;
	currentPointIndex = 0;
	newBeam->arrSize = 0;
}


void MeteoMap::handleSegment(unsigned short segment) {
	unsigned power = (segment & 0xE000) >> 13;
	unsigned size = (segment & 0x1E00) >> 9;
	unsigned range = segment & 0x01FF;
	for (unsigned newPointRange = range; newPointRange < (range + size); newPointRange++) { //перебираем отрезок по точкам
		point newPoint = { newPointRange, power };
		if (newPointRange < currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize] = newPoint; // просто добавляем в конец 
			newBeam->arrSize++;
		} else if (newPointRange == currentBeam->pointArr[currentPointIndex].range){
			recalcPower(currentBeam->pointArr[currentPointIndex], currentBeam->time, &newPoint);
			newBeam->pointArr[newBeam->arrSize] = newPoint;
			newBeam->arrSize++;
			currentPointIndex++;
		} else /*if (newPointRange > currentBeam->pointArr[currentPointIndex].range)*/ {
			newBeam->pointArr[newBeam->arrSize].power = getActuaPower(
				currentBeam->pointArr[currentPointIndex].power, currentBeam->time);
			newBeam->pointArr[newBeam->arrSize].range = currentBeam->pointArr[currentPointIndex].range;
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
	newPoint->power = ( newPoint->power + oldPower) /2;
}

unsigned MeteoMap::getActuaPower(unsigned power, timespec time) {
	timespec currentTime = { 0, 0 };
	timespec_get(&currentTime, TIME_UTC);
	long dt_mlsec = (currentTime.tv_sec - time.tv_sec) * 1000 +
		(currentTime.tv_sec - time.tv_sec) / 1000000; // разница времени в милисекундах
	long weightCoef = 1 / (1 + koef_u * pow(dt_mlsec, koef_k));
	return power * weightCoef;
}

void MeteoMap::finalizeFilling(unsigned azimuth, unsigned elevation) {
	for (int i = currentPointIndex; i < currentBeam->arrSize; i++) {
		newBeam->pointArr[newBeam->arrSize].power = getActuaPower(
			currentBeam->pointArr[i].power, currentBeam->time);
		newBeam->pointArr[newBeam->arrSize].range = currentBeam->pointArr[i].range;
		newBeam->arrSize++;
	}
	timespec_get(&newBeam->time, TIME_UTC);
	if (currentBeam != lastBeam) {
		delete[] currentBeam->pointArr;
		delete currentBeam;
	}
	if (spaceMap[azimuth][elevation] == lastBeam && spaceMap[azimuth][elevation] != NULL) {
		delete[] spaceMap[azimuth][elevation]->pointArr;
		delete spaceMap[azimuth][elevation];
	}
	spaceMap[azimuth][elevation] = newBeam;
	lastBeam = newBeam;
	newBeam = new beam;
	newBeam->pointArr = new point[BEAM_SIZE];
	status = getting;
}
