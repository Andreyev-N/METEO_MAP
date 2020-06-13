// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <vld.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#include "pch.h"
#include <iostream>
#include <windows.h>
#include "MeteoMap.h"
using namespace std;

void printBinary(unsigned num);
void printBeam(const beam* b);

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();
void test11();
void test12();
void test13();
void test14();
void test15();


int main()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	_CrtDumpMemoryLeaks();
	return 0;
}


void printBinary(unsigned num) {
	for (int i = 15; i >= 0; --i) // short 16 бит (00000000 00000000)
		cout << ((num >> i) & 1);
	cout << "\n";
}

void printBeam(const beam* b) {
	cout << "beam :\n";
	for (int i = 0; i < b->arrSize; i++) {
		cout << "   point" << i << ": range = " << b->pointArr[i].range
			<< "; power = " << b->pointArr[i].power << "\n";
	}
} 

void test1() {
	cout << "test1: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0b1010101000110010; //5,5,50
	mm.add(unitCodogramm);
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 2;
	unitCodogramm[2] = 2;
	unitCodogramm[3] = 0b101010100000101; //5,5,5
	if (mm.add(unitCodogramm) == FAILURE)
		cout << "true\n";
	else
		cout << "false\n";
}
void test2() {
	cout << "test2: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0b1010101000110010; //5,5,50
	if (mm.add(unitCodogramm) == CONTINUE_DIRECTION)
		cout << "true\n";
	else
		cout << "false\n";
}
void test3() {
	cout << "test3: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0;
	if (mm.add(unitCodogramm) == END_OF_DIRECTION)
		cout << "true\n";
	else
		cout << "false\n";
}
void test4() {
	cout << "test4: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}

void test5() {
	cout << "test5: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0; 
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
	Sleep(2);
	unitCodogramm[3] = 0b1110111000000101; //7,7,5
	unitCodogramm[4] = 0b1110011000010100; //7,3,20
	unitCodogramm[5] = 0b1110101000011110; //7,5,30
	unitCodogramm[6] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}

void test6() {
	cout << "test6: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0b1010101000110010; //5,5,50
	mm.add(unitCodogramm);
	unitCodogramm[3] = 0b1010101000111100; //5,5,60
	unitCodogramm[4] = 0b1010101001000110; //5,5,70
	unitCodogramm[5] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}
void test7();
void test8();
void test9();
void test10();
void test11();
void test12();
void test13();
void test14();
void test15();

