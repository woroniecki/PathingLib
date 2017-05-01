#include "stdafx.h"
#include "Utility.h"
#include "Graph.h"
#include "CHGraph.h"
#include <string>
#include <functional> 
#include <algorithm>
#include <queue>

#include <iostream>

namespace PathingLib
{
	typedef pair<int, pair<int, bool>> P_p;

	string CHGraph::getPath(int sourceIndex, int targetIndex) {
		// <dystans, <node, kierunek>>
		int INF_ = Utility::getINF();
		std::priority_queue < std::pair<int, std::pair<int, bool>>,
			std::vector < std::pair<int, std::pair<int, bool>>>,
			std::greater < std::pair<int, std::pair<int, bool>>> > queue;
		int* distance_S = new int[nodesAmount];
		std::fill_n(distance_S, nodesAmount, 90000000);
		distance_S[sourceIndex] = 0;

		int* distance_T = new int[nodesAmount];
		std::fill_n(distance_T, nodesAmount, 90000000);
		distance_T[targetIndex] = 0;

		int* real_distance_S = new int[nodesAmount];
		std::fill_n(real_distance_S, nodesAmount, 90000000);
		real_distance_S[sourceIndex] = 0;

		int* real_distance_T = new int[nodesAmount];
		std::fill_n(real_distance_T, nodesAmount, 90000000);
		real_distance_T[targetIndex] = 0;

		int* from_S_EDGE = new int[nodesAmount];
		from_S_EDGE[sourceIndex] = -1;

		int* from_T_EDGE = new int[nodesAmount];
		from_T_EDGE[targetIndex] = -1;

		queue.push(make_pair(alt->heuristic(sourceIndex, targetIndex), make_pair(sourceIndex, true)));
		queue.push(make_pair(alt->heuristic(targetIndex, sourceIndex), make_pair(targetIndex, false)));

		int min_distance = INF_;
		int jointNodeID = -1;
		int visitedNodes = 0;

		while (!queue.empty()) {
			visitedNodes += 1;
			P_p top = queue.top();
			queue.pop();
			int distance;
			int realDistance;
			int node = top.second.first;
			bool forward = top.second.second;
			int* edgesOfNode;
			int edges_amount;

			if (forward) {
				distance = distance_S[node];
				edgesOfNode = nodes[node].outHigherEdges;
				edges_amount = nodes[node].outHEdgesAmount;
			}
			else {
				distance = distance_T[node];
				edgesOfNode = nodes[node].inHigherEdges;
				edges_amount = nodes[node].inHEdgesAmount;
			}

			for (int i = 0; i < edges_amount; i++) {
				int next_node;
				if (forward) {
					next_node = edges[edgesOfNode[i]].edge->target;
				}
				else {
					next_node = edges[edgesOfNode[i]].edge->source;
				}

				int dist_next_node = distance + edges[edgesOfNode[i]].edge->distance;
				int heuristic_;
				if (forward) {
					heuristic_ = alt->heuristic(next_node, targetIndex);
				}
				else {
					heuristic_ = alt->heuristic(next_node, sourceIndex);
				}

				int heuristic_dist = dist_next_node + heuristic_;
				if (forward) {
					if (dist_next_node < distance_S[next_node] &&
						heuristic_dist < min_distance) {
						distance_S[next_node] = dist_next_node;
						real_distance_S[next_node] = dist_next_node;
						from_S_EDGE[next_node] = edgesOfNode[i];

						real_distance_S[next_node] = real_distance_S[node] + edges[edgesOfNode[i]].edge->realDistance;
						if (distance_S[next_node] != INF_ &&
							distance_T[next_node] != INF_) {
							int temp_min_dist = distance_S[next_node] + distance_T[next_node];
							if (min_distance > temp_min_dist) {
								min_distance = temp_min_dist;
								jointNodeID = next_node;
							}
						}
						else {
							queue.push(make_pair(heuristic_dist, make_pair(next_node, forward)));
						}
					}
				}
				else {
					if (dist_next_node < distance_T[next_node] &&
						heuristic_dist < min_distance) {
						distance_T[next_node] = dist_next_node;
						from_T_EDGE[next_node] = edgesOfNode[i];

						real_distance_T[next_node] = real_distance_T[node] + edges[edgesOfNode[i]].edge->realDistance;
						if (distance_S[next_node] != INF_ &&
							distance_T[next_node] != INF_) {
							int temp_min_dist = distance_S[next_node] + distance_T[next_node];
							if (min_distance > temp_min_dist) {
								min_distance = temp_min_dist;
								jointNodeID = next_node;
							}
						}
						else {
							queue.push(make_pair(heuristic_dist, make_pair(next_node, forward)));
						}
					}
				}
			}
		}
		if (jointNodeID == -1)
			return " ";// failure trzeb coœ znaleŸæ

		string path = getPathJSON(jointNodeID, from_S_EDGE, from_T_EDGE, real_distance_S[jointNodeID] + real_distance_T[jointNodeID]);
	
		delete[] distance_S;
		delete[] distance_T;
		delete[] real_distance_S;
		delete[] real_distance_T;
		delete[] from_S_EDGE;
		delete[] from_T_EDGE;

		return path;
	}

	string CHGraph::getPathJSON(int jointNode, int* fromSEDGES, int* fromTEDGES, int distance) {
		int amount = getNodesAmount(jointNode, fromSEDGES, true) + getNodesAmount(jointNode, fromTEDGES, false);
		int* nodes = new int[++amount];
		string returnVal = "{ \'paths\':\n [{ \'instructions\': [\n";
		returnVal += "{ \'distance\' : " + to_string(distance) + ",\n \'interval\' : [0, " + to_string(amount - 1) +"] } ],\n";

		fillArrayWithPath(nodes, jointNode, fromSEDGES, fromTEDGES);

		returnVal += "\'points\':\n { \'coordinates\': [\n";

		for (int i = 0; i < amount; i++) {
			returnVal += "[\n" + this->nodes[nodes[i]].node->longtitudeSTRING + ",\n" + this->nodes[nodes[i]].node->latitudeSTRING + "\n]\n";
			if (i + 1 <= amount)
				returnVal += ",";
		}
		returnVal += "], \'type\': \'LineString\'}";
		returnVal += "\n";

		delete[] nodes;
		return returnVal;
	}

	void CHGraph::fillArrayWithPath(int* fillArray, int jointNode, int* fromSEDGES, int* fromTEDGES) {

		int index = runFillingArray(jointNode, fillArray, fromSEDGES, true, 0);
		cout << "NEXTfillingTOtarget" << endl;
		runFillingArray(jointNode, fillArray, fromTEDGES, false, index);

	}

	int CHGraph::runFillingArray(int currentNode, int* fillArray, int* arrayEdges, bool directionToSource, int index) {
		int currentEdge = -1;

		cout << "    runFill:" << currentNode << endl;

		currentEdge = arrayEdges[currentNode];

		if (currentEdge == -1)
			return index;

		int recNode;

		if (directionToSource) {
			recNode = edges[arrayEdges[currentNode]].edge->source;
			index = runFillingArray(recNode, fillArray, arrayEdges, directionToSource, index);
			return fillColumnsOfArrayWithNodes(arrayEdges[currentNode], fillArray, arrayEdges, index);
		}
		else {
			recNode = edges[arrayEdges[currentNode]].edge->target;
			index = fillColumnsOfArrayWithNodes(arrayEdges[currentNode], fillArray, arrayEdges, index);
			return runFillingArray(recNode, fillArray, arrayEdges, directionToSource, index);
		}
	}

	int CHGraph::fillColumnsOfArrayWithNodes(int ID, int* fillArray, int* arrayEdges, int firstIndex) {

		cout << "        FILLING: " << ID << " " << edges[ID].firstEdgePart << " " << edges[ID].secondEdgePart << endl;
		if (!edges[ID].shortcut) {
			cout << "            FILL: " << firstIndex << endl;
			fillArray[firstIndex] = edges[ID].edge->source;
			cout << "            FILL: " << firstIndex + 1 << endl;
			fillArray[++firstIndex] = edges[ID].edge->target;
			return firstIndex;
		}

		cout << "        NEXT FILLING: " << edges[ID].firstEdgePart << endl;
		firstIndex = fillColumnsOfArrayWithNodes(edges[ID].firstEdgePart, fillArray, arrayEdges, firstIndex);
		cout << "        NEXT FILLING: " << edges[ID].secondEdgePart << endl;
		firstIndex = fillColumnsOfArrayWithNodes(edges[ID].secondEdgePart, fillArray, arrayEdges, firstIndex);

		return firstIndex;
	}

	int CHGraph::getNodesAmount(int currentNode, int* arrayEdges, bool directionToSource) {
		int amount = 0;
		int currentEdge = -1;

		currentEdge = arrayEdges[currentNode];

		while (currentEdge != -1) {
			amount += edges[currentEdge].edgesAmount;
			if (directionToSource) {
				currentNode = edges[arrayEdges[currentNode]].edge->source;
			}
			else {
				currentNode = edges[arrayEdges[currentNode]].edge->target;
			}
			currentEdge = arrayEdges[currentNode];
		}

		return amount;
	}
}
