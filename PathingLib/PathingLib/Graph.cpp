// PathingLib.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
#include "stdafx.h"
#include "Utility.h"
#include "Graph.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <iostream>

using namespace std;

namespace PathingLib
{
	NODE::NODE() {}

	NODE::~NODE() {
		/*if (this->in_edges != NULL)
		delete[] this->in_edges;
		if (this->out_edges != NULL)
		delete[] this->out_edges;*/
	}

	NODE::NODE(const NODE& o) : index(o.index), longtitude(o.longtitude), latitude(o.latitude) {
		PathingLib::Utility u;
		in_edges = Utility::copyArray(o.in_edges, o.in_edges_amount);
		in_edges_amount = o.in_edges_amount;
		out_edges = Utility::copyArray(o.out_edges, o.out_edges_amount);
		out_edges_amount = o.out_edges_amount;
	}

	NODE::NODE(int index, double longtitude, double latitude) : index(index), longtitude(longtitude), latitude(latitude) {}

	void NODE::addInEdge(int edgeIndex) {
		in_edges = Utility::addFieldToArray(in_edges, in_edges_amount);
		in_edges[in_edges_amount] = edgeIndex;
		in_edges_amount += 1;
	};

	void NODE::addOutEdge(int edgeIndex) {
		out_edges = Utility::addFieldToArray(out_edges, out_edges_amount);
		out_edges[out_edges_amount] = edgeIndex;
		out_edges_amount += 1;
	};

	EDGE::EDGE() {};

	Graph::Graph() {};

	Graph::Graph(int nodesMaxAmount, int edgesMaxAmount) {
		nodes = new NODE[nodesMaxAmount]();
		this->nodesAmount = 0;
		this->nodesMaxAmount = nodesMaxAmount;
		edges = new EDGE[edgesMaxAmount]();
		this->edgesAmount = 0;
		this->edgesMaxAmount = edgesMaxAmount;
	}

	Graph Graph::loadGraphFromFile(std::string nodesPath, std::string edgesPath, int maxEdgesAmount_) {
		int nodesAmount = Utility::getLinesAmountInFile(nodesPath);
		int edgesAmount = Utility::getLinesAmountInFile(edgesPath);
		if (maxEdgesAmount_ != -1)
			edgesAmount = maxEdgesAmount_;
		if (nodesAmount == -1 || edgesAmount == -1)
			throw std::runtime_error("Invalid file");
		Graph graph(nodesAmount, edgesAmount);

		ifstream inputNode(nodesPath);
		for (string line; getline(inputNode, line); )
		{
			vector<string> nodeArgs = Utility::separateLine(line);
			
			double lat = std::stod(nodeArgs[1].c_str());
			double lng = std::stod(nodeArgs[2].c_str());

			graph.addNode(lat, lng);
			nodeArgs.clear();
		}
		inputNode.close();

		ifstream inputEdge(edgesPath);
		for (string line; getline(inputEdge, line); )
		{
			std::vector<std::string> edgeArgs = Utility::separateLine(line);

			int source = std::stod(edgeArgs[0].c_str());
			int target = std::stod(edgeArgs[1].c_str());
			int distance = int(std::stod(edgeArgs[2].c_str()) * 100); // to centimiters

			graph.addDirectedEdge(source, target, distance);
			edgeArgs.clear();
		}
		inputEdge.close();

		return graph;
	}

	void Graph::saveToFile(string nodesPath, string edgesPath) {
		string tempLine = "";
		ofstream nodesFile;
		nodesFile.open(nodesPath);
		for (int i = 0; i<nodesAmount; i++) {
			tempLine = "";
			tempLine += to_string(i) + " ";
			tempLine += to_string(nodes[i].latitude) + " ";
			tempLine += to_string(nodes[i].longtitude) + " ";
			if (i < nodesAmount - 1)
				tempLine += "\n";
			nodesFile << tempLine;
		}
		nodesFile.close();

		ofstream edgesFile;
		edgesFile.open(edgesPath);
		for (int i = 0; i<edgesAmount; i++) {
			tempLine = "";
			tempLine += to_string(edges[i].source) + " ";
			tempLine += to_string(edges[i].target) + " ";
			tempLine += to_string(((float)edges[i].distance / 100)) + " ";
			if (i < edgesAmount - 1)
				tempLine += "\n";
			edgesFile << tempLine;
		}
		edgesFile.close();
	}

	void Graph::saveToFileCSV(string filePath) {
		string tempLine = "";
		ofstream file;
		file.open(filePath);
		for (int i = 0; i < getEdgesAmount(); i++) {
			tempLine = Utility::getLineString(
				nodes[edges[i].source].latitude,
				nodes[edges[i].source].longtitude,
				nodes[edges[i].target].latitude,
				nodes[edges[i].target].longtitude
			);
			if (i < getEdgesAmount() - 1)
				tempLine += "\n";
			file << tempLine;
		}
		file.close();
	}

	NODE Graph::addNode(double latitude, double longtitude) {
		if (nodesAmount >= nodesMaxAmount) {
			throw std::out_of_range("Can't add more nodes, because array of nodes is full");
		}
		nodes[nodesAmount].index = nodesAmount;
		nodes[nodesAmount].latitude = latitude;
		nodes[nodesAmount].longtitude = longtitude;
		nodesAmount++;
		return nodes[nodesAmount - 1];
	}

	EDGE Graph::addDirectedEdge(int source, int target, int distance) {
		if (edgesAmount >= edgesMaxAmount)
			throw std::out_of_range("Can't add more edges, because array of egdes is full");
		if(source >= nodesMaxAmount)
			throw std::out_of_range("Can't add edge, because source node is out of nodes range");
		if(target >= nodesMaxAmount)
			throw std::out_of_range("Can't add edge, because target node is out of nodes range");
		edges[edgesAmount].index = edgesAmount;
		edges[edgesAmount].source = source;
		edges[edgesAmount].target = target;
		edges[edgesAmount].distance = distance;

		nodes[source].addOutEdge(edgesAmount); 
	    nodes[target].addInEdge(edgesAmount);
		edgesAmount++;
		return edges[edgesAmount - 1];
	}

	NODE Graph::getNode(int index) {
		if (index >= nodesAmount) {
			throw std::out_of_range("Can't get node, because index is out of nodes range");
		}
		return nodes[index];
	}

	EDGE Graph::getEdge(int index) {
		if (index >= edgesAmount) {
			throw std::out_of_range("Can't get edge, because index is out of edges range");
		}
		return edges[index];
	}

	int Graph::getNodesAmount(){
		return nodesAmount;
	}

	int Graph::getEdgesAmount(){
		return edgesAmount;
	}

	int* Graph::getInEdges(int nodeIndex){
		if (nodeIndex >= nodesAmount) {
			throw std::out_of_range("Can't get node, because index is out of nodes range");
		}
		return nodes[nodeIndex].in_edges;
	}

	int* Graph::getOutEdges(int nodeIndex){
		if (nodeIndex >= nodesAmount) {
			throw std::out_of_range("Can't get node, because index is out of nodes range");
		}
		return nodes[nodeIndex].out_edges;
	}

	int Graph::getInEdgesAmount(int nodeIndex){
		if (nodeIndex >= nodesAmount) {
			throw std::out_of_range("Can't get node, because index is out of nodes range");
		}
		return nodes[nodeIndex].in_edges_amount;
	}

	int Graph::getOutEdgesAmount(int nodeIndex){
		if (nodeIndex >= nodesAmount) {
			throw std::out_of_range("Can't get node, because index is out of nodes range");
		}
		return nodes[nodeIndex].out_edges_amount;
	}
}