// PathingLib.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
#include "stdafx.h"
#include "Dijkstra.h"
#include "Path.h"
#include "Utility.h"
#include "Graph.h"
#include <queue>
#include <utility> 
#include <functional> 
#include <algorithm>
#include <vector>

using namespace std;

namespace PathingLib
{
	typedef std::pair<int, int> P;

	Path Dijkstra::getPath(int sourceIndex, int targetIndex, Graph& g) {
		int* distanceArray = new int[g.getNodesAmount()];
		std::fill_n(distanceArray, g.getNodesAmount(), Utility::getINF());
		int* nodesBeforeArray = new int[g.getNodesAmount()];

		priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;
		distanceArray[sourceIndex] = 0;
		nodesBeforeArray[sourceIndex] = -1;
		q.push(P(0, sourceIndex));

		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int distance = top.first;
			int node = top.second;
			int* out_edges = g.getOutEdges(node);
			for (int i = 0; i < g.getOutEdgesAmount(node); i++) {
				int dist_next_node = distance + g.getEdge(out_edges[i]).distance;
				int next_node = g.getEdge(out_edges[i]).target;
				if (dist_next_node < distanceArray[next_node] &&
					dist_next_node < distanceArray[targetIndex]) {
					distanceArray[next_node] = dist_next_node;
					nodesBeforeArray[next_node] = node;
					q.push(P(dist_next_node, next_node));
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

	int* Dijkstra::getDijkstraArray(int startNodeIndex, Graph& g) {
		int* distanceArray = new int[g.getNodesAmount()];
		std::fill_n(distanceArray, g.getNodesAmount(), Utility::getINF());

		priority_queue< std::pair<int, int>,
			std::vector<std::pair<int, int>>,
			std::greater<std::pair<int, int>> > q;
		distanceArray[startNodeIndex] = 0;
		q.push(P(0, startNodeIndex));

		while (!q.empty()) {
			P top = q.top();
			q.pop();
			int distance = top.first;
			int node = top.second;
			int* in_edges = g.getInEdges(node);
			for (int i = 0; i < g.getInEdgesAmount(node); i++) {
				int dist_next_node = distance + g.getEdge(in_edges[i]).distance;
				int next_node = g.getEdge(in_edges[i]).source;
				if (dist_next_node < distanceArray[next_node]) {
					distanceArray[next_node] = dist_next_node;
					q.push(P(dist_next_node, next_node));
				}
			}
		}
		return distanceArray;
	}
}