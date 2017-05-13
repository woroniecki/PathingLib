// PathingLib.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
#include "stdafx.h"
#include "Utility.h"
#include "Graph.h"
#include "CHGraph.h"
#include "ALT.h"
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

#include <iostream>

using namespace std;

namespace PathingLib
{
	CHGraph::CHGraph() {};

	CHGraph::CHGraph(Graph& g, ALT& alt) {
		this->g = &g;
		this->alt = &alt;
		extendGraph();
		preprocesing();
		setHigherNeighbours();
	}

	CHGraph::CHGraph(std::string nodesPath, std::string edgesPath) {
		int nodesAmount = Utility::getLinesAmountInFile(nodesPath);
		int edgesAmount = Utility::getLinesAmountInFile(edgesPath);
		if (nodesAmount == -1 || edgesAmount == -1)
			throw std::runtime_error("Invalid file");
		g = new Graph(nodesAmount, edgesAmount);
		
		nodes = new CH_NODE[nodesAmount]();
		ifstream inputNode(nodesPath);
		int index = 0;
		for (string line; getline(inputNode, line); )
		{
			vector<string> nodeArgs = Utility::separateLine(line);

			int index = int(std::stod(nodeArgs[0].c_str()));
			double lat = std::stod(nodeArgs[1].c_str());
			double lng = std::stod(nodeArgs[2].c_str());
			int ch_lvl = int(std::stod(nodeArgs[3].c_str()));

			g->addNode(lat, lng);
			nodes[index].node = &g->nodes[index];
			nodes[index].contract_lvl = ch_lvl;
			
			nodeArgs.clear();
			index++;
		}
		inputNode.close();

		index = 0;
		edges = new CH_EDGE[edgesAmount]();
		ifstream inputEdge(edgesPath);
		for (string line; getline(inputEdge, line); )
		{
			std::vector<std::string> edgeArgs = Utility::separateLine(line);

			int source = std::stod(edgeArgs[0].c_str());
			int target = std::stod(edgeArgs[1].c_str());
			int distance = int(std::stod(edgeArgs[2].c_str()) * 100); // to centimiters
			int realdistance = int(std::stod(edgeArgs[3].c_str()) * 100); // to centimiters
			bool shortcut = std::stod(edgeArgs[4].c_str());
			int first_edge = std::stod(edgeArgs[5].c_str());
			int second_edge = std::stod(edgeArgs[6].c_str());
			int amountOfEdges = std::stod(edgeArgs[7].c_str());

			addShortcutEdge(source, target, distance, realdistance, first_edge, second_edge, amountOfEdges, shortcut);

			edgeArgs.clear();
			index++;
		}
		inputEdge.close();

		this->nodesAmount = nodesAmount;
		this->edgesAmount = edgesAmount;

		this->alt = new ALT(*g, 4, false);
		setHigherNeighbours();
	}

	void CHGraph::preprocesing() {
		int* node_indexes = new int[g->getNodesAmount()];
		for (int i = 0; i < g->getNodesAmount(); i++)
			node_indexes[i] = i;
		random_shuffle(&node_indexes[0], &node_indexes[g->getNodesAmount() - 1]);

		int sourceID = -1;
		int targetID = -1;
		int distSource = -1;
		int distTarget = -1;
		int distance = -1;
		int realDistSource = -1;
		int realDistTarget = -1;
		int realDistance = -1;
		int addedEdges = 0;
		int lastAddedEdges = 0;

		const clock_t begin_time = clock();

		for (int i = 0; i < g->getNodesAmount(); i++) {
			int node_index = node_indexes[i];
			nodes[node_index].contract_lvl = i;

			int* in_edgesNode = nodes[node_index].node->in_edges;
			int in_edges_amount_N = nodes[node_index].node->in_edges_amount;
			int* out_edgesNode = nodes[node_index].node->out_edges;
			int out_edges_amount_N = nodes[node_index].node->out_edges_amount;
			for (int inEdgeID = 0; inEdgeID < in_edges_amount_N; inEdgeID++) {
				sourceID = edges[in_edgesNode[inEdgeID]].edge->source;
				if (nodes[sourceID].contract_lvl != -1)
					continue;
				distSource = edges[in_edgesNode[inEdgeID]].edge->distance;
				realDistSource = edges[in_edgesNode[inEdgeID]].edge->realDistance;

				for (int outEdgeID = 0; outEdgeID < out_edges_amount_N; outEdgeID++) {
					targetID = edges[out_edgesNode[outEdgeID]].edge->target;
					if (nodes[targetID].contract_lvl != -1)
						continue; 
					if (targetID == sourceID)
						continue;
					distTarget = edges[out_edgesNode[outEdgeID]].edge->distance;
					realDistTarget = edges[out_edgesNode[outEdgeID]].edge->realDistance;
					distance = distSource + distTarget;
					realDistance = realDistSource + realDistTarget;

					if (distance <= alt->getPathDist(sourceID, targetID)) {
						addShortcutEdge(
							sourceID,
							targetID,
							distance,
							realDistance,
							in_edgesNode[inEdgeID],
							out_edgesNode[outEdgeID],
							edges[in_edgesNode[inEdgeID]].edgesAmount + edges[out_edgesNode[outEdgeID]].edgesAmount,
							true);
						addedEdges += 1;
					}
				}
			}
			if (i > 400000) {
				if (i % 5 == 0) {
					cout << i << " AMOUNT_TOTAL: " << addedEdges << " IN_ITER: " << addedEdges - lastAddedEdges << endl;
					lastAddedEdges = addedEdges;
				}
			}
			else if (i > 250000) {
				if (i % 20 == 0) {
					cout << i << " AMOUNT_TOTAL: " << addedEdges << " IN_ITER: " << addedEdges - lastAddedEdges << endl;
					lastAddedEdges = addedEdges;
				}
			}
			else {
				if (i % 100 == 0) {
					cout << i << " AMOUNT_TOTAL: " << addedEdges << " IN_ITER: " << addedEdges - lastAddedEdges << endl;
					lastAddedEdges = addedEdges;
				}
			}
			if (i % 1000 == 0)
				cout << "TIME: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		}

		cout << "Total time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << " AMOUNT: " << addedEdges << endl;
	}

	void CHGraph::addShortcutEdge(int source, int target, int distance, int realDistance, int firstEDGEpartID, int secondEDGEpartID, int amountOfEdges, bool isShortCut) {
		g->addDirectedEdge(source, target, realDistance, distance);

		edges[edgesAmount].edge = &g->edges[edgesAmount];
		edges[edgesAmount].shortcut = isShortCut;
		edges[edgesAmount].firstEdgePart = firstEDGEpartID;
		edges[edgesAmount].secondEdgePart = secondEDGEpartID;
		edges[edgesAmount].edgesAmount = amountOfEdges;

		nodes[source].node->addOutEdge(edgesAmount);
		nodes[target].node->addInEdge(edgesAmount);

		edgesAmount++;
	}

	void CHGraph::extendGraph() {
		extendGraphNodes();
		extendGraphEdges();
		nodesAmount = g->getNodesAmount();
		edgesAmount = g->getEdgesAmount();
	}

	void CHGraph::extendGraphNodes() {
		nodes = new CH_NODE[g->getNodesAmount()]();
		for (int i = 0; i < g->getNodesAmount(); i++) {
			nodes[i].node = &g->nodes[i];
		}
	}

	void CHGraph::extendGraphEdges() {
		edges = new CH_EDGE[g->getEdgesAmount() * 4]();
		for (int i = 0; i < g->getEdgesAmount(); i++) {
			edges[i].edge = &g->edges[i];
		}
	}

	void CHGraph::setHigherNeighbours() {
		for (int i = 0; i < edgesAmount; i++) {
			int c_lvl_source = nodes[edges[i].edge->source].contract_lvl;
			int c_lvl_target = nodes[edges[i].edge->target].contract_lvl;
			if (c_lvl_source > c_lvl_target) {
				nodes[edges[i].edge->target].addInHigherEdge(i);
			}
			else if (c_lvl_source < c_lvl_target) {
				nodes[edges[i].edge->source].addOutHigherEdge(i);
			}
		}
	}

	void CHGraph::saveGraphToFile(string nodesPath, string edgesPath) {
		string tempLine = "";
		ofstream nodesFile;
		nodesFile.open(nodesPath);
		for (int i = 0; i< nodesAmount; i++) {
			tempLine = "";
			tempLine += to_string(i) + " ";
			tempLine += to_string(nodes[i].node->latitude) + " ";
			tempLine += to_string(nodes[i].node->longtitude) + " ";
			tempLine += to_string(nodes[i].contract_lvl);
			if (i < nodesAmount - 1)
				tempLine += "\n";
			nodesFile << tempLine;
		}
		nodesFile.close();

		ofstream edgesFile;
		edgesFile.open(edgesPath);
		for (int i = 0; i < edgesAmount; i++) {
			tempLine = "";
			tempLine += to_string(edges[i].edge->source) + " ";
			tempLine += to_string(edges[i].edge->target) + " ";
			tempLine += to_string(((float)edges[i].edge->distance / 100)) + " ";
			tempLine += to_string(((float)edges[i].edge->realDistance / 100)) + " ";
			tempLine += to_string((int)edges[i].shortcut) + " ";
			tempLine += to_string((int)edges[i].firstEdgePart) + " ";
			tempLine += to_string((int)edges[i].secondEdgePart) + " ";
			tempLine += to_string((int)edges[i].edgesAmount) + " ";
			if (i < edgesAmount - 1)
				tempLine += "\n";
			edgesFile << tempLine;
		}
		edgesFile.close();
	}

	CH_NODE::CH_NODE(){
		this->node = NULL;
		this->contract_lvl = -1;
		this->inHigherEdges = NULL;
		this->inHEdgesAmount = 0;
		this->outHigherEdges = NULL;
		this->outHEdgesAmount = 0;
	}

	void CH_NODE::addInHigherEdge(int edgeIndex) {
		inHigherEdges = Utility::addFieldToArray(inHigherEdges, inHEdgesAmount);
		inHigherEdges[inHEdgesAmount] = edgeIndex;
		inHEdgesAmount += 1;
	};

	void CH_NODE::addOutHigherEdge(int edgeIndex) {
		outHigherEdges = Utility::addFieldToArray(outHigherEdges, outHEdgesAmount);
		outHigherEdges[outHEdgesAmount] = edgeIndex;
		outHEdgesAmount += 1;
	};
}