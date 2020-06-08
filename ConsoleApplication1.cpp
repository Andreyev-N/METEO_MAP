// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "MeteoMap.h"
using namespace std;

void printBinary(unsigned num);

int main()
{
	unsigned codogram = 0xffff;
	bool a = true;
 
	
	unsigned res1 = codogram & 0xE000 >> 13;
	unsigned res2 = (codogram & 0x1E00) >> 9;  
	
	printBinary(codogram);
	printBinary(res1);
	printBinary(res2);
	printBinary(res2);
	cout << res2 << "\n";
	unsigned testCodogram[8];
	for (int i = 0; i < 8; i++) {
		testCodogram[i] = 1;
	}
	MeteoMap meteoMap;
	meteoMap.add(testCodogram);

}


void printBinary(unsigned num) {
	for (int i = 15; i >= 0; --i) // short 16 бит (00000000 00000000)
		cout << ((num >> i) & 1);
	cout << "\n";
}



