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
			PATHINGLIB_API CH_NODE();

			NODE* node;
			int contract_lvl = -1;

			int* inHigherEdges;
			int inHEdgesAmount;
			int* outHigherEdges;
			int outHEdgesAmount;

			void addInHigherEdge(int edgeIndex);
			void addOutHigherEdge(int edgeIndex);
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

		// g - Graph on which contraction hierarchy will be created
		// alt - ALT of g graph, for finding the shortest distance between two points on g Graph
		PATHINGLIB_API CHGraph(Graph& g, ALT& alt);

		PATHINGLIB_API CHGraph(std::string nodesPath, std::string edgesPath);

		void PATHINGLIB_API saveGraphToFile(string nodesPath, string edgesPath);

		int PATHINGLIB_API getPath(int sourceIndex, int targetIndex);

		// int PATHINGLIB_API getPath(int sourceIndex, int targetIndex);

	private:
		Graph* g;
		ALT* alt;
		CH_NODE* nodes;
		int nodesAmount = 0;
		CH_EDGE* edges;
		int edgesAmount = 0;

		void extendGraph();
		void extendGraphNodes();
		void extendGraphEdges();
		void addShortcutEdge(int source, int target, int distance, int firstEDGEpartID, int secondEDGEpartID, bool = true);

		void CHGraph::preprocesing();
		void setHigherNeighbours();
		// void preprocesing();
	};
}
