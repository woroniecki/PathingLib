// Graph.h - Contains declaration of Graph class
#pragma once  

#ifdef PATHINGLIB_EXPORTS  
#define PATHINGLIB_API __declspec(dllexport)   
#else  
#define PATHINGLIB_API __declspec(dllimport)   
#endif

#include <string>

using namespace std;

namespace PathingLib
{
	// Represent node in graph
	class NODE {
	public:
		int index = -1;
		double longtitude;
		double latitude;
		int* in_edges;
		int in_edges_amount = 0;
		int* out_edges;
		int out_edges_amount = 0;

		PATHINGLIB_API NODE();

		virtual PATHINGLIB_API ~NODE();

		PATHINGLIB_API NODE(const NODE& o);

		// add edge which is directed from any node to this one
		void PATHINGLIB_API addInEdge(int edgeIndex);

		// add edge which is directed from this node to any
		void PATHINGLIB_API addOutEdge(int edgeIndex);
	};

	// Represent edge in graph
	class EDGE {
	public:
		int index = -1;
		int source, target, distance;

		PATHINGLIB_API EDGE();
	};

	// Represent a directed graph object 
	class Graph
	{
	public:
		PATHINGLIB_API Graph();
		// int nodesMaxAmount - size of nodes array
		// int edgesMaxAmount - size of edges array
		PATHINGLIB_API Graph(int nodesMaxAmount, int edgesMaxAmount);

		// string nodesPath - path to file with nodes, which is represented in way
		// index (0, infinity), latitude, longtitude
		// Example:
		// 0 1.2341 2.232
		// 1 1.9923 2.333
		// string edgesPath - path to file with edges, which is represented in way
		// source node index(0, amount od nodes), target node index(0, amount od nodes), distance in km
		// Example:
		// 0 1 2.11
		// 1 0 2.11 
		static Graph PATHINGLIB_API loadGraphFromFile(std::string nodesPath, std::string edgesPath);

		// save graph to file
		// create two files with nodes and edges
		void PATHINGLIB_API saveToFile(string nodesPath, string edgesPath);

		// save all edges by using string line format in file
		void PATHINGLIB_API saveToFileCSV(string filePath);

		// add node to graph if array is not full
		bool PATHINGLIB_API addNode(double latitude, double longtitude);

		// add edge from source to target, if array is not full
		bool PATHINGLIB_API addDirectedEdge(int source, int target, int distance);

		// return node with index
		NODE PATHINGLIB_API getNode(int index);

		// return edge with index
		EDGE PATHINGLIB_API getEdge(int index);

		// return nodes amount
		int PATHINGLIB_API getNodesAmount();

		// return edges amount
		int PATHINGLIB_API getEdgesAmount();

		// return array of edges from any node to node with nodeIndex
		int* getInEdges(int nodeIndex);

		// return array of edges from node with nodeIndex to any node
		int* getOutEdges(int nodeIndex);

		// return amount of edges from any node to node with nodeIndex
		int PATHINGLIB_API getInEdgesAmount(int nodeIndex);

		// return amount of edges from node with nodeIndex to any node
		int PATHINGLIB_API getOutEdgesAmount(int nodeIndex);

	private:
		// array of nodes
		NODE* nodes;

		// array of edges
		EDGE* edges;

		// Current nodes amount
		int nodesAmount;

		// Current edges amount
		int edgesAmount;

		// size of array with nodes
		int nodesMaxAmount;

		// size of array with edges
		int edgesMaxAmount;
	};
}