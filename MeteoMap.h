#pragma once
#include <time.h>  //Для clock_gettime
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define AZIMUTH_SIZE 16384
#define ELEVATION_SIZE 4096
#define BEAM_SIZE 511
#define DELIMITER "|"
#define U_COEF 0.00001 //c такими коэфициентами за 10 секунд вероятность упадет до 50 процентов
#define K_COEF 1       // пока не думал над значениями
#define NEW_POINT_COEF 0.5  

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
	beam*** spaceMap; //хранится все
	beam* lastBeam;   //хранится последнее обработанное направление, для метода getLastHandledBeam
	beam* currentBeam;//хранится направление, которе сейчас обрабатывается.
	int currentPointIndex;  //хранится номер точки из currentBeam, которая будет обработана следующая
	beam* newBeam;    //новый массив, который заполняется в процессе обработки
	Status status;    //состояние
	unsigned failureCodogram[8]; 
	
	void recalcPower(const point oldPoint, const timespec oldTime, point* newPoint);
	unsigned getActuaPower(unsigned power, timespec time);
	void initNewBeam();
	void handleSegment(unsigned codogramLine);
	void finalizeFilling(unsigned azimuth, unsigned elevation);
	void initNewDirection();
};

