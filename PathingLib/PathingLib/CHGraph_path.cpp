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

	int CHGraph::getPath(int sourceIndex, int targetIndex) {
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

		int* from_S = new int[nodesAmount];
		from_S[sourceIndex] = -1;

		int* from_T = new int[nodesAmount];
		from_T[targetIndex] = -1;

		queue.push(make_pair(alt->heuristic(sourceIndex, targetIndex), make_pair(sourceIndex, true)));
		queue.push(make_pair(alt->heuristic(targetIndex, sourceIndex), make_pair(targetIndex, false)));

		int min_distance = INF_;
		int visitedNodes = 0;

		while (!queue.empty()) {
			visitedNodes += 1;
			P_p top = queue.top();
			queue.pop();
			int distance;
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
						if (distance_S[next_node] != INF_ &&
							distance_T[next_node] != INF_) {
							int temp_min_dist = distance_S[next_node] + distance_T[next_node];
							if (min_distance > temp_min_dist) {
								min_distance = temp_min_dist;
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
						if (distance_S[next_node] != INF_ &&
							distance_T[next_node] != INF_) {
							int temp_min_dist = distance_S[next_node] + distance_T[next_node];
							if (min_distance > temp_min_dist) {
								min_distance = temp_min_dist;
							}
						}
						else {
							queue.push(make_pair(heuristic_dist, make_pair(next_node, forward)));
						}
					}
				}
			}
		}
		//cout << "    Visited Nodes: " << visitedNodes << endl;
		//cout << "    Distance: " << min_distance << endl;
		return min_distance;
	}
}