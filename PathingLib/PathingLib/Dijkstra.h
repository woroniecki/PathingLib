// Dijkstra.h - Contains declaration of Dijkstra class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include "Graph.h"
#include "Path.h"

using namespace std;

namespace PathingLib
{

	class Dijkstra
	{
	public:

		// return path from source to target in graph
		static Path PATHINGLIB_API getPath(int sourceIndex, int targetIndex, Graph& g);

		// return pointer on array filled with distances of the shorthest path to all nodes from startNode
		static int* getDijkstraArray(int startNode, Graph& g);
	};
}