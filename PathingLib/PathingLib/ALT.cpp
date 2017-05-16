#include "stdafx.h"
#include "Graph.h"
#include "ALT.h"
#include "Dijkstra.h"
#include "Utility.h"
#include "Path.h"
#include <time.h> 
#include <queue>
#include <utility> 
#include <functional> 
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;


namespace PathingLib
{
	ALT::ALT() {}

	ALT::ALT(Graph& g, int landmarksAmount, bool random) {
		if (landmarksAmount >= g.getNodesAmount())
			throw std::invalid_argument("amount of landmarks can't be higher than nodes amount in graph");
		if (landmarksAmount <= 0)
			throw std::invalid_argument("amount of landmarks have to be higher than 0");
		this->g = &g;
		this->landmarksAmount = landmarksAmount;
		dijkstraLandmarks = new int*[landmarksAmount];
		int* landmarksIndexes = new int[landmarksAmount];
		int node = 0;
		srand(time(NULL));

		int landmarkNodeIndex;
		if (random) {
			bool is_not_repeated;
			for (int i = 0; i < landmarksAmount; i++) {
				is_not_repeated = false;
				while (!is_not_repeated) {
					is_not_repeated = true;
					landmarkNodeIndex = rand() % g.getNodesAmount();
					for (int j = 0; j < i; j++) {
						if (landmarkNodeIndex == landmarksIndexes[j])
							is_not_repeated = false;
					}
				}
				dijkstraLandmarks[i] = Dijkstra::getDijkstraArray(landmarkNodeIndex, g);
				landmarksIndexes[i] = landmarkNodeIndex;
			}
		}
		else {
			for (int i = 0; i < landmarksAmount; i++) {
				landmarkNodeIndex = getTheFurthestLandmark(i);
				dijkstraLandmarks[i] = Dijkstra::getDijkstraArray(landmarkNodeIndex, g);
				int amount_of_notconnected_nodes = 0;
				for (int j = 0; j < this->g->getNodesAmount(); j++) {
					if (dijkstraLandmarks[i][j] == Utility::getINF()) {
						amount_of_notconnected_nodes++;
					}
				}
				landmarksIndexes[i] = landmarkNodeIndex;
				cout << "    NOT CONNECTED NODES AMOUNT: " << amount_of_notconnected_nodes << endl;
			}
		}
	}

	int ALT::getTheFurthestLandmark(int landmarkAmount) {
		int node = rand() % g->getNodesAmount();
		int max_dist = 0;
		for (int i = 0; i < g->getNodesAmount(); i++) {
			int dist = Utility::getINF();
			for (int j = 0; j < landmarkAmount; j++) {
				if (dijkstraLandmarks[j][i] < dist) {
					dist = dijkstraLandmarks[j][i];
				}
			}
			if (dist > max_dist && dist != Utility::getINF()) {
				max_dist = dist;
				node = i;
			}
		}
		return node;
	}

	typedef std::pair<int, int> P;

	Path ALT::getPath(int sourceIndex, int targetIndex) {
		int* distanceArray = new int[g->getNodesAmount()];
		std::fill_n(distanceArray, g->getNodesAmount(), Utility::getINF());
		int* realDistanceArray = new int[g->getNodesAmount()];
		int* edgeBeforeArray = new int[g->getNodesAmount()];
		int* nodesBeforeArray = new int[g->getNodesAmount()];

		std::priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;

		distanceArray[sourceIndex] = 0;
		realDistanceArray[sourceIndex] = 0;
		nodesBeforeArray[sourceIndex] = -1;
		q.push(make_pair(heuristic(sourceIndex, targetIndex), sourceIndex));

		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int node = top.second;
			int distance = distanceArray[node];
			int realDistance = realDistanceArray[node];
			int* out_edges = g->getOutEdges(node);
			for (int i = 0; i < g->getOutEdgesAmount(node); i++) {
				int next_node = g->getEdge(out_edges[i]).target;
				int dist_next_node = distance + g->getEdge(out_edges[i]).distance;
				int heuristic_ = heuristic(next_node, targetIndex);
				int heuristic_dist = dist_next_node + heuristic_;
				if (dist_next_node < distanceArray[next_node] &&
					heuristic_dist < distanceArray[targetIndex]) {
					distanceArray[next_node] = dist_next_node;
					realDistanceArray[next_node] = realDistance + g->getEdge(out_edges[i]).realDistance;
					nodesBeforeArray[next_node] = node;
					q.push(make_pair(heuristic_dist, next_node));
				}
			}
		}
		if (distanceArray[targetIndex] == Utility::getINF())
			return Path();
		Path path(targetIndex, nodesBeforeArray, *g, realDistanceArray[targetIndex]);
		delete[] distanceArray;
		delete[] realDistanceArray;
		delete[] nodesBeforeArray;
		delete[] edgeBeforeArray;
		return path;
	}

	string ALT::getPathJSON(float lng1, float lat1, float lng2, float lat2) {
		int sourceIndex = g->getTheClosestNode(lng1, lat1);
		int targetIndex = g->getTheClosestNode(lng2, lat2);
		if (sourceIndex == targetIndex)
			return "no path";
		int* distanceArray = new int[g->getNodesAmount()];
		std::fill_n(distanceArray, g->getNodesAmount(), Utility::getINF());
		int* realDistanceArray = new int[g->getNodesAmount()];
		int* edgeBeforeArray = new int[g->getNodesAmount()];
		int* nodesBeforeArray = new int[g->getNodesAmount()];

		std::priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;

		distanceArray[sourceIndex] = 0;
		realDistanceArray[sourceIndex] = 0;
		nodesBeforeArray[sourceIndex] = -1;
		edgeBeforeArray[sourceIndex] = -1;
		q.push(make_pair(heuristic(sourceIndex, targetIndex), sourceIndex));
		
		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int node = top.second;
			int distance = distanceArray[node];
			int realDistance = realDistanceArray[node];
			int* out_edges = g->getOutEdges(node);
			for (int i = 0; i < g->getOutEdgesAmount(node); i++) {
				int next_node = g->getEdge(out_edges[i]).target;
				int dist_next_node = distance + g->getEdge(out_edges[i]).distance;
				int heuristic_ = heuristic(next_node, targetIndex);
				int heuristic_dist = dist_next_node + heuristic_;
				if (dist_next_node < distanceArray[next_node] &&
					heuristic_dist < distanceArray[targetIndex]) {
					distanceArray[next_node] = dist_next_node;
					realDistanceArray[next_node] = realDistance + g->getEdge(out_edges[i]).realDistance;
					nodesBeforeArray[next_node] = node;
					edgeBeforeArray[next_node] = out_edges[i];
					q.push(make_pair(heuristic_dist, next_node));
				}
			}
		}
		
		if (distanceArray[targetIndex] == Utility::getINF())
			return "";

		string result = Path::getJSON(targetIndex, nodesBeforeArray, edgeBeforeArray, *g, realDistanceArray[targetIndex], 50000);
		delete[] distanceArray;
		delete[] realDistanceArray;
		delete[] nodesBeforeArray;
		delete[] edgeBeforeArray;
		return result;
	}

	int ALT::getPathDist(int sourceIndex, int targetIndex) {
		int* distanceArray = new int[g->getNodesAmount()];
		std::fill_n(distanceArray, g->getNodesAmount(), Utility::getINF());

		std::priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;

		distanceArray[sourceIndex] = 0;
		q.push(make_pair(heuristic(sourceIndex, targetIndex), sourceIndex));

		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int node = top.second;
			int distance = distanceArray[node];
			int* out_edges = g->getOutEdges(node);
			for (int i = 0; i < g->getOutEdgesAmount(node); i++) {
				int next_node = g->getEdge(out_edges[i]).target;
				int dist_next_node = distance + g->getEdge(out_edges[i]).distance;
				int heuristic_ = heuristic(next_node, targetIndex);
				int heuristic_dist = dist_next_node + heuristic_;
				if (dist_next_node < distanceArray[next_node] &&
					heuristic_dist < distanceArray[targetIndex]) {
					distanceArray[next_node] = dist_next_node;
					q.push(make_pair(heuristic_dist, next_node));
				}
			}
		}
		if (distanceArray[targetIndex] == Utility::getINF())
			return -1;
		int dist = distanceArray[targetIndex];
		delete[] distanceArray;
		return dist;
	}

	int ALT::heuristic(int node, int target) {
		int heuristic = 0;
		for (int i = 0; i < landmarksAmount; i++) {
			int node_dist = dijkstraLandmarks[i][target] - dijkstraLandmarks[i][node];
			int target_dist = dijkstraLandmarks[i][node] - dijkstraLandmarks[i][target];
			int dist = max(node_dist, target_dist);
			if (dist > heuristic)
				heuristic = dist;
		}
		return heuristic;
	}

	/*ALT::ALT(const ALT& o) {
		g = o.g;
		landmarksAmount = o.landmarksAmount;
		for (int i = 0; i < landmarksAmount; i++) {
			dijkstraLandmarks[i] = new int[g.getNodesAmount()]();
			for (int j = 0; j < g.getNodesAmount(); j++) {
				dijkstraLandmarks[i][j] = o.dijkstraLandmarks[i][j];
			}
		}
	}

	ALT::~ALT() {
		for (int i = 0; i < landmarksAmount; i++) {
			if (dijkstraLandmarks[i])
				delete[] dijkstraLandmarks[i];
		}
		if (dijkstraLandmarks)
			delete[] dijkstraLandmarks;
	}*/
}