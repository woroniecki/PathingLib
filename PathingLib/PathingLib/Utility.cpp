// PathingLib.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
#include "stdafx.h"
#include "Utility.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

namespace PathingLib
{
	int Utility::getLinesAmountInFile(std::string path) {
		int amount = 0;
		ifstream input(path);
		if (input){
			for (string line; getline(input, line); )
			{
				amount++;
			}
			input.close();
		}
		else {
			return -1;
		}
		return amount;
	}

	std::vector<std::string> Utility::separateLine(std::string line) {
		string buf;
		stringstream ss(line);

		vector<string> tokens;

		while (ss >> buf) {
			tokens.push_back(buf);
		}

		return tokens;
	}

	int* Utility::addFieldToArray(int array[], int size) {
		int newSize = size + 1;
		int* newArray = new int[newSize];
		for (int i = 0; i < size; i++) {
			newArray[i] = array[i];
		}
		if (size > 0)
			delete[] array;
		return newArray;
	}


	int* Utility::copyArray(int array[], int size) {
		if (size == 0)
			return NULL;
		int* newArray = new int[size];
		for (int i = 0; i < size; i++) {
			newArray[i] = array[i];
		}
		return newArray;
	}

	int Utility::getINF() {
		return 90000000;
	}

	std::string Utility::getLineString(double x1, double y1, double x2, double y2) {
		string s = "LINESTRING (";
		s += to_string(x1) + " ";
		s += to_string(y1) + ", ";
		s += to_string(x2) + " ";
		s += to_string(y2) + ") ";
		return s;
	}
}