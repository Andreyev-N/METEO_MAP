// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define __CRTDBG_MAP_ALLOC
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include "MeteoMap.h"
using namespace std;

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


int main()
{
	ofstream out("out.txt"); 
	std::streambuf *coutbuf = std::cout.rdbuf(); 
	std::cout.rdbuf(out.rdbuf()); //перенаправим вывод в файл
	test1();
	test2(); 
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	_CrtDumpMemoryLeaks();
	return 0;
}

void printBeam(const beam* b) {
	cout << "beam :\n";
	for (int i = 0; i < (int) b->arrSize; i++) {
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
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
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
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
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
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
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
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
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
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
	unitCodogramm[7] = 0b1010101000110010; //5,5,50
	mm.add(unitCodogramm);
	unitCodogramm[3] = 0b1100101000111100; //6,5,60
	unitCodogramm[4] = 0b1110101001000110; //7,5,70
	unitCodogramm[5] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}

void test7() {
	cout << "test7: ";
	unsigned short unitCodogramm[8];
	MeteoMap mm(0, 1); // с такими коэффициентами весовой коэффициент не будет зависеть от времени
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0;
	unitCodogramm[5] = 0;
	unitCodogramm[6] = 0;
	unitCodogramm[7] = 0;
	mm.add(unitCodogramm);
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 2;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0b1010101000010100; //5,5,20
	unitCodogramm[5] = 0b1010101000011110; //5,5,30
	unitCodogramm[6] = 0b1010101000101000; //5,5,40
	unitCodogramm[7] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}

void test8() {
	cout << "test8: ";
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
	unitCodogramm[2] = 2;
	unitCodogramm[3] = 0b1010101000000101; //5,5,5
	unitCodogramm[4] = 0;
	mm.add(unitCodogramm);
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b1110111000000101; //7,7,5
	unitCodogramm[4] = 0b1110011000010100; //7,3,20
	unitCodogramm[5] = 0b1110101000011110; //7,5,30
	unitCodogramm[6] = 0;
	mm.add(unitCodogramm);
	printBeam(mm.getLastHandledBeam());
}

void test9() {
	cout << "test9: ";
	MeteoMap mm(0, 1);
	timespec t1 = { 0,0 }, t2 = { 0,0 }, t3 = { 0,0 };
	unsigned short unitCodogramm[8], unitCodogramm2[8];
	unitCodogramm[0] = 0;
	unitCodogramm[1] = 1;
	unitCodogramm[2] = 1;
	unitCodogramm[3] = 0b0010101000000101; //1,5,5 (мощность, размер, дальность)
	unitCodogramm[4] = 0b0100101000010100; //2,5,20
	unitCodogramm[5] = 0b0110101000011110; //3,5,30
	unitCodogramm[6] = 0b1000101000101000; //4,5,40
	unitCodogramm[7] = 0b1010101000110010; //5,5,50
	unitCodogramm2[0] = 0;
	unitCodogramm2[1] = 1;
	unitCodogramm2[2] = 1;
	unitCodogramm2[3] = 0b1100101000111100; //6,5,60
	unitCodogramm2[4] = 0b1110101001000110; //7,5,70
	unitCodogramm2[5] = 0; 
	long a = 0;
	timespec_get(&t1, TIME_UTC);
	for (int i = 0; i < 1000000; i++) {
		mm.add(unitCodogramm);
		mm.add(unitCodogramm2);
	}
	timespec_get(&t3, TIME_UTC);

	cout << "RESULT: " << (t3.tv_sec - t1.tv_sec) *1000 + (t3.tv_nsec - t1.tv_nsec)/1000000 << " milisec\n";
	
}
