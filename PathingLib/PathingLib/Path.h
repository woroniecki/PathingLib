// Path.h - Contains declaration of Path class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include "Graph.h"
#include <vector>

using namespace std;

namespace PathingLib
{

	class Path
	{
	public:
		// hold indexes of nodes, which represent path
		std::vector<NODE> nodes;

		PATHINGLIB_API Path();

		// targetIndex - index in nodes array which is last <br>
		// nodes - array with dijkstra nodes before <br>
		// g - graph on which path is based <br>
		// distance - total distance of path <br>
		// max nodes amount - by default 10 000, it's to avoid infinity loop <br>
		PATHINGLIB_API Path(int targetIndex, int* nodes, Graph& g, int distance, int = 10000);

		// return distance <br>
		// if unit is 'c' in centimeters <br>
		// if unit is 'm' in meters <br>
		// if unit is 'k' in kilometers <br>
		double PATHINGLIB_API getDistance(char unit);

		// return amount of nodes in path
		int PATHINGLIB_API getNodesAmount();

		// save nodes in file <br>
		// lines save in stringline format
		void PATHINGLIB_API saveToFileCSV(string filePath);

		string static PATHINGLIB_API getJSON(int targetIndex, int* nodes, int* edges, Graph& g, int distance, int maxNodesAmount);

	private:
		int distance = -1;
	};
}