// Utility.h - Contains declaration of Utility class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include <string.h>
#include <vector>

namespace PathingLib
{

	// Represent utility functions
	class Utility
	{
	public:
		// return amount of lines in file
		static int getLinesAmountInFile(std::string path);

		// separate string by spaces char
		static std::vector<std::string> separateLine(std::string line);

		// return the same array extend by one unassigned field
		static int* addFieldToArray(int array[], int size);

		// copies int array
		static int* copyArray(int array[], int size);

		// return max integer
		static int getINF();

		// return line string
		static std::string getLineString(double x1, double y1, double x2, double y2);
	};
}