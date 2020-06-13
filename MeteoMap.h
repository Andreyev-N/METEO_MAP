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

enum retValue { // ������������ �������� ��� ������ add
	END_OF_DIRECTION = 0,  // ���� ����� ��������
	CONTINUE_DIRECTION = 1,
	FAILURE = 2
};

class MeteoMap
{
public:
	MeteoMap(long koef_u, long koef_k);
	~MeteoMap();
	int add(unsigned short codogram[8]);
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
	long koef_u;
	long koef_k;

	void recalcPower(const point oldPoint, const timespec oldTime, point* newPoint);
	unsigned getActuaPower(unsigned power, timespec time);
	void initNewBeam();
	void handleSegment(unsigned short codogramLine);
	void finalizeFilling(unsigned azimuth, unsigned elevation);
	void initNewDirection();
};

