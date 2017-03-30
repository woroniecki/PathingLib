// PathingLib.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
#include "stdafx.h"
#include "Utility.h"
#include "Graph.h"
#include "CHGraph.h"
#include "ALT.h"
#include <string>
#include <algorithm>
#include <vector>

#include <iostream>

using namespace std;

namespace PathingLib
{
	CHGraph::CHGraph() {};

	CHGraph::CHGraph(Graph& g, ALT& alt) {
		this->g = g;
		this->alt = alt;
		extendGraph();
		preprocesing();
	}

	void CHGraph::preprocesing() {
		int* node_indexes = new int[g.getNodesAmount()];
		for (int i = 0; i < g.getNodesAmount(); i++)
			node_indexes[i] = i;
		random_shuffle(&node_indexes[0], &node_indexes[g.getNodesAmount() - 1]);

		int sourceID = -1;
		int targetID = -1;
		int distSource = -1;
		int distTarget = -1;
		int distance = -1;
		int addedEdges = 0;

		for (int i = 0; i < g.getNodesAmount(); i++) {
			int node_index = node_indexes[i];
			nodes[node_index].contract_lvl = i;

			int* in_edgesNode = nodes[node_index].node->in_edges;
			int in_edges_amount_N = nodes[node_index].node->in_edges_amount;
			int* out_edgesNode = nodes[node_index].node->in_edges;
			int out_edges_amount_N = nodes[node_index].node->out_edges_amount;

			for (int inEdgeID = 0; inEdgeID < in_edges_amount_N; inEdgeID++) {
				sourceID = edges[in_edgesNode[inEdgeID]].edge->source;
				if (nodes[sourceID].contract_lvl != -1)
					continue;
				distSource = edges[in_edgesNode[inEdgeID]].edge->distance;

				for (int outEdgeID = 0; outEdgeID < out_edges_amount_N; outEdgeID++) {
					targetID = edges[out_edgesNode[outEdgeID]].edge->target;
					if (nodes[targetID].contract_lvl != -1)
						continue;
					if (targetID == sourceID)
						continue;
					distTarget = edges[out_edgesNode[outEdgeID]].edge->distance;
					distance = distSource + distTarget;
					
					if (distance <= alt.getPathDist(sourceID, targetID)) {
						// TRZEBA ŒCIE¯KE ROWEROW¥ JESZCZE PARAMETR DODAÆ!!!!
						addShortcutEdge(sourceID, targetID, distance, in_edgesNode[inEdgeID], out_edgesNode[outEdgeID]);
						addedEdges += 1;
					}
				}
			}
		}
	}

	void CHGraph::addShortcutEdge(int source, int target, int distance, int firstEDGEpartID, int secondEDGEpartID) {
		EDGE* newEdge = new EDGE();
		newEdge->index = edges_amount;
		newEdge->source = source;
		newEdge->target = target;
		newEdge->distance = distance;

		edges[edges_amount].edge = newEdge;
		edges[edges_amount].shortcut = true;
		edges[edges_amount].firstEdgePart = firstEDGEpartID;
		edges[edges_amount].secondEdgePart = secondEDGEpartID;

		nodes[source].node->addOutEdge(edges_amount);
		nodes[target].node->addInEdge(edges_amount);

		edges_amount++;
	}

	void CHGraph::extendGraph() {
		extendGraphNodes();
		extendGraphEdges();
		nodes_amount = g.getNodesAmount();
		edges_amount = g.getEdgesAmount();
	}

	void CHGraph::extendGraphNodes() {
		nodes = new CH_NODE[g.getNodesAmount()]();
		for (int i = 0; i < g.getNodesAmount(); i++) {
			nodes[i].node = &g.getNode(i);
		}
	}

	void CHGraph::extendGraphEdges() {
		edges = new CH_EDGE[g.getEdgesAmount() * 4]();
		for (int i = 0; i < g.getEdgesAmount(); i++) {
			edges[i].edge = &g.getEdge(i);
		}
	}
}