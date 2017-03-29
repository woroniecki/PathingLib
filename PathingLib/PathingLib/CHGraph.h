// Graph.h - Contains declaration of Graph class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include "Graph.h"
#include "ALT.h"
#include <string>

using namespace std;

namespace PathingLib
{
	class HIGHER_EDGE: public EDGE {

	};

	// Represent a directed graph object 
	class CHGraph
	{
	public:
		CHGraph();
		CHGraph(const CHGraph& orig);
		virtual ~CHGraph();
		CHGraph(Graph& g, ALT& alt);
		int query(int source_, int target_);
	private:
		Graph g;
		EDGE* higher_edges;
		int** in_higher_edges;
		int** out_higher_edges;
		void preprocesing();
		void setHigherNeighbours();
	};
}