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
	//newBeam = new beam;
	//newBeam->pointArr = new point[BEAM_SIZE];
	//currentBeam = new beam;
	//currentBeam->pointArr = new point[BEAM_SIZE];
	status = start;
	//currentBeam = NULL;
	//lastBeam = NULL;
}

MeteoMap::~MeteoMap(){
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
		for (int j = 0; j < ELEVATION_SIZE; j++) {
			delete spaceMap[i][j];
		}
	}
	for (int i = 0; i < AZIMUTH_SIZE; i++) {
		delete[] spaceMap[i];
	}
	delete [] spaceMap;
	//delete [] newBeam->pointArr;
	//delete newBeam;
	//delete[] currentBeam->pointArr;
	//delete currentBeam;
	//delete lastBeam;
}

const beam* MeteoMap::getLastHandledBeam() {
		return lastBeam;
}


int MeteoMap::add(unsigned short codogram[8]) {
	unsigned azimuth = codogram[1] & 0x3FFF;
	unsigned elevation = codogram[2];
	if (status == filling) {
		if (currentBeam != spaceMap[azimuth][elevation]) {
			finalizeFilling(azimuth, elevation); 
			return FAILURE;
		} 
	} else {
		currentBeam = spaceMap[azimuth][elevation];
		if (currentBeam == NULL) {
			initNewBeam();
		}
		initNewDirection();
	}
	for (int j = 3; j < 8; j++) {
		if (codogram[j] == NULL) {
			finalizeFilling(azimuth, elevation);
			return END_OF_DIRECTION;
		}
		handleSegment(codogram[j]);
	}
	delete[] spaceMap[azimuth][elevation];
	spaceMap[azimuth][elevation] = currentBeam;
	return CONTINUE_DIRECTION;
}

void MeteoMap::initNewDirection() {
	status = filling;
	currentPointIndex = 0;
	newBeam = new beam;                       //��� delete
	newBeam->pointArr = new point[BEAM_SIZE]; //��� delete
	newBeam->arrSize = 0;
}

void MeteoMap::initNewBeam() {
	currentBeam = new beam;
	currentBeam->pointArr = new point[BEAM_SIZE];
	currentBeam->arrSize = 0;
}

void MeteoMap::handleSegment(unsigned short segment) {
	unsigned power = (segment & 0xE000) >> 13;
	unsigned size = (segment & 0x1E00) >> 9;
	unsigned range = segment & 0x01FF;
	for (unsigned newPointRange = range; newPointRange < (range + size); newPointRange++) { //���������� ������� �� ������
		point newPoint = { newPointRange, power };
		if (newPointRange < currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize] = newPoint; // ������ ��������� � ����� 
			newBeam->arrSize++;
		} else if (newPointRange == currentBeam->pointArr[currentPointIndex].range){
			recalcPower(currentBeam->pointArr[currentPointIndex], currentBeam->time, &newPoint);
			newBeam->pointArr[newBeam->arrSize] = newPoint;
			newBeam->arrSize++;
			currentPointIndex++;
		} else if (newPointRange > currentBeam->pointArr[currentPointIndex].range) {
			newBeam->pointArr[newBeam->arrSize].power = getActuaPower(
				currentBeam->pointArr[currentPointIndex].power, currentBeam->time);
			newBeam->pointArr[newBeam->arrSize].range = currentBeam->pointArr[currentPointIndex].range;
			newBeam->arrSize++;
			currentPointIndex++;
			newPointRange--; // �� ���� � ��������� ����� �� ����� ��������� ��� �� �����,
			                // �� ���������� ����� �� ��������� �� currentBeam
		}
		if (newBeam->pointArr[newBeam->arrSize].power == 0) {
			newBeam->arrSize--; //�� ���� �� ������� ����� �� ������, ���� �� ��������� �������
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
		(currentTime.tv_sec - time.tv_sec) / 1000000; // ������� ������� � ������������
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
	//delete [] currentBeam->pointArr;
	//delete currentBeam;
	currentBeam == NULL;
	//delete spaceMap[azimuth][elevation];
	spaceMap[azimuth][elevation] = newBeam;
	lastBeam = newBeam;
	//delete newBeam;
	//delete[] currentBeam->pointArr;
	status = getting;
}
