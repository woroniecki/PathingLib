// CHGraph.h - Contains declaration of CHGraph class
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
	class CH_NODE {
		public:
			NODE* node;
			int contract_lvl;

			int* in_higher_edges;
			int in_higher_e_amount;
			int* out_higher_edges;
			int out_higher_e_amount;
	};

	class CH_EDGE  {
		public:
			EDGE* edge;
			bool shortcut = false;
			int firstEdgePart = -1, secondEdgePart = -1;
	};

	class CHGraph
	{
	public:
		PATHINGLIB_API CHGraph();
		PATHINGLIB_API CHGraph(Graph& g, ALT& alt);
		// int query(int source_, int target_);
	private:
		Graph g;
		ALT alt;
		CH_NODE* nodes;
		int nodes_amount;
		CH_EDGE* edges;
		int edges_amount;

		void extendGraph();
		void extendGraphNodes();
		void extendGraphEdges();
		void addShortcutEdge(int source, int target, int distance, int firstEDGEpartID, int secondEDGEpartID);

		void CHGraph::preprocesing();
		// void preprocesing();
		// void setHigherNeighbours();
	};
}
