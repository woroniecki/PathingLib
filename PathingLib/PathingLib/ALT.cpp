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

	ALT::ALT(Graph& g, int landmarksAmount, bool random) {
		if(landmarksAmount >= g.getNodesAmount())
			throw std::invalid_argument("amount of landmarks can't be higher than nodes amount in graph");
		if (landmarksAmount <= 0)
			throw std::invalid_argument("amount of landmarks have to be higher than 0");
		this->g = g;
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
				landmarksIndexes[i] = landmarkNodeIndex;
			}
		}
	}

	int ALT::getTheFurthestLandmark(int landmarkAmount) {
		int node = rand() % g.getNodesAmount();
		int max_dist = 0;
		for (int i = 0; i < g.getNodesAmount(); i++) {
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
		int* distanceArray = new int[g.getNodesAmount()];
		std::fill_n(distanceArray, g.getNodesAmount(), Utility::getINF());
		int* nodesBeforeArray = new int[g.getNodesAmount()];
		
		std::priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;

		distanceArray[sourceIndex] = 0;
		nodesBeforeArray[sourceIndex] = -1;
		q.push(make_pair(heuristic(sourceIndex, targetIndex), sourceIndex));

		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int node = top.second;
			int distance = distanceArray[node];
			int* out_edges = g.getOutEdges(node);
			for (int i = 0; i < g.getOutEdgesAmount(node); i++) {
				int next_node = g.getEdge(out_edges[i]).target;
				int dist_next_node = distance + g.getEdge(out_edges[i]).distance;
				int heuristic_ = heuristic(next_node, targetIndex);
				int heuristic_dist = dist_next_node + heuristic_;
				if (dist_next_node < distanceArray[next_node] &&
					heuristic_dist < distanceArray[targetIndex]) {
					distanceArray[next_node] = dist_next_node;
					nodesBeforeArray[next_node] = node;
					q.push(make_pair(heuristic_dist, next_node));
				}
			}
		}
		if (distanceArray[targetIndex] == Utility::getINF())
			return Path();
		Path path = Path(targetIndex, nodesBeforeArray, g, distanceArray[targetIndex]);
		delete[] distanceArray;
		delete[] nodesBeforeArray;
		return path;
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

	ALT::~ALT(){
		for (int i = 0; i < landmarksAmount; i++) {
			if (dijkstraLandmarks[i])
				delete[] dijkstraLandmarks[i];
		}
		if (dijkstraLandmarks)
			delete[] dijkstraLandmarks;
	}
}