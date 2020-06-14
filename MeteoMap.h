#pragma once
#include <time.h>  //Для clock_gettime
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vld.h> //for tests 

#define AZIMUTH_SIZE 16384
#define ELEVATION_SIZE 4096
#define BEAM_SIZE 511

#define DELIMITER "|"
#define U_COEF 0.00001 //c такими коэфициентами за 10 секунд вероятность упадет до 50 процентов
#define K_COEF 1       // пока не думал над значениями
#define NEW_POINT_COEF 1  

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
};

enum retValue { // возвращаемое значение для метода add
	END_OF_DIRECTION = 0,  // коды можно поменять
	CONTINUE_DIRECTION = 1,
	FAILURE = 2
};

class MeteoMap
{
public:
	MeteoMap(long koef_u, long koef_k);
	~MeteoMap();
	int add(unsigned short codogram[8]);
	const beam* getLastHandledBeam();

private:
	beam*** spaceMap; //хранится все
	beam* lastBeam;   //хранится последнее обработанное направление, для метода getLastHandledBeam
	beam* currentBeam;//хранится направление, которе сейчас обрабатывается.
	int currentPointIndex;  //хранится номер точки из currentBeam, которая будет обработана следующая
	beam* newBeam;    //новый массив, который заполняется в процессе обработки
	Status status;    //состояние
	long koef_u;
	long koef_k;
	unsigned lastAzimuth;
	unsigned lastElevation;

	void recalcPower(const point oldPoint, const timespec oldTime, point* newPoint);
	unsigned getActuaPower(unsigned power, timespec time);
	void handleSegment(unsigned short codogramLine);
	void finalizeFilling(unsigned azimuth, unsigned elevation);
	void initNewDirection();
};

