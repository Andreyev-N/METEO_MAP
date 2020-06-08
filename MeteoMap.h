#pragma once
#include <time.h>  //��� clock_gettime
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define AZIMUTH_SIZE 16384
#define ELEVATION_SIZE 4096
#define BEAM_SIZE 511
#define DELIMITER "|"
#define U_COEF 0.00001 //c ������ ������������� �� 10 ������ ����������� ������ �� 50 ���������
#define K_COEF 1       // ���� �� ����� ��� ����������
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

enum retValue { // ������������ �������� ��� ������ add
	END_OF_DIRECTION = 0,  // ���� ����� ��������
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
	beam*** spaceMap; //�������� ���
	beam* lastBeam;   //�������� ��������� ������������ �����������, ��� ������ getLastHandledBeam
	beam* currentBeam;//�������� �����������, ������ ������ ��������������.
	int currentPointIndex;  //�������� ����� ����� �� currentBeam, ������� ����� ���������� ���������
	beam* newBeam;    //����� ������, ������� ����������� � �������� ���������
	Status status;    //���������
	unsigned failureCodogram[8]; 
	
	void recalcPower(const point oldPoint, const timespec oldTime, point* newPoint);
	unsigned getActuaPower(unsigned power, timespec time);
	void initNewBeam();
	void handleSegment(unsigned codogramLine);
	void finalizeFilling(unsigned azimuth, unsigned elevation);
	void initNewDirection();
};

