// ALT.h - Contains declaration of ALT class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include "Graph.h"
#include "Path.h"
#include <string> 

using namespace std;

namespace PathingLib
{

	class ALT
	{
	public:
		PATHINGLIB_API ALT();
		
		// Graph used to finding path, it have to be connective
		// landmarksAmount - amount of landmarks used to pathing
		// random - if true landmarks selected random
		// if not landmark is selected to be the furthest from the other landmarks
		PATHINGLIB_API ALT(Graph& g, int landmarksAmount, bool = true);

		// PATHINGLIB_API ALT(const ALT& o);

		// PATHINGLIB_API ~ALT();

		// return path by using alt algorithm <br>
		Path PATHINGLIB_API getPath(int sourceIndex, int targetIndex);

		// return distance of path from source to target
		int PATHINGLIB_API getPathDist(int sourceIndex, int targetIndex);

		string PATHINGLIB_API getPathJSON(float lng1, float lat1, float lng2, float lat2);

		int PATHINGLIB_API heuristic(int node, int target);
		
		int** dijkstraLandmarks;
	private:
		Graph* g;
		
		int landmarksAmount;
		int PATHINGLIB_API getTheFurthestLandmark(int landmarkAmount);
	};
}