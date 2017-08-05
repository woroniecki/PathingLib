#include "stdafx.h"
#include "Graph.h"
#include "Utility.h"
#include "Path.h"
#include <fstream>
#include <sstream>
#include <string> 

using namespace std;

namespace PathingLib
{
	Path::Path() {};

	Path::Path(int targetIndex, int* nodes, Graph& g, int distance, int maxNodesAmount) {
		int nextNode = targetIndex;
		int nodesAmount = 0;
		while (nextNode != -1 || nodesAmount >= maxNodesAmount) {
			this->nodes.push_back(NODE(g.getNode(nextNode)));
			nextNode = nodes[nextNode];
			nodesAmount++;
		}
		this->distance = distance;
		std::reverse(this->nodes.begin(), this->nodes.end());
	}

	double Path::getDistance(char unit) {
		if (unit == 'k')
			return (double)distance / 100000.0;
		else if (unit == 'm')
			return (double)distance / 100.0;
		else if (unit == 'c')
			return (double)distance;
		return (double)distance;
	}

	int Path::getNodesAmount() {
		return nodes.size();
	}

	void Path::saveToFileCSV(string filePath) {
		string tempLine = "";
		ofstream file;
		file.open(filePath);
		for (int i = 0; i < nodes.size() - 1; i++) {
			tempLine = Utility::getLineString(
				nodes[i].latitude,
				nodes[i].longtitude,
				nodes[i + 1].latitude,
				nodes[i + 1].longtitude
			);
			if (i < nodes.size() - 2)
				tempLine += "\n";
			file << tempLine;
		}
		file.close();
	}

	string Path::getJSON(int targetIndex, int* nodes, int* edges, Graph& g, int distance, int maxNodesAmount) {
		int nextNode = targetIndex;
		int nodesAmount = 0;
		std::vector<int> nodes_;
		std::vector<int> edges_;
		Graph *g_ = &g;

		while (nextNode != -1 || nodesAmount >= maxNodesAmount) {
			nodes_.push_back(nextNode);
			int edge = edges[nextNode];
			if(edge != -1)
				edges_.push_back(edge);
			nextNode = nodes[nextNode];
			nodesAmount++;
		}

		string edgesStr = "{ \"paths\": [{ \"instructions\": [";
		string coordsStr = "\"points\":\n { \"coordinates\": [\n";

		bool cycleStreet = g_->getEdge(edges_[nodesAmount - 2]).distance != g_->getEdge(edges_[nodesAmount - 2]).realDistance;
		int startInterval = 0;

		for (int i = nodesAmount - 1; i > -1; i--) {
			coordsStr += "[\n" + to_string(g_->getNode(nodes_[i]).latitude) + ",\n" + to_string(g_->getNode(nodes_[i]).longtitude) + "\n]\n";
			if (i > 0)
				coordsStr += ",";

			if (i > 0) {
				float xx = g_->getEdge(edges_[i - 1]).distance;
				float yy = g_->getEdge(edges_[i - 1]).realDistance;
				bool isThisEdgeCycleStreet = xx != yy;
				if (cycleStreet != isThisEdgeCycleStreet) {

					edgesStr += "{ \n";
					if (cycleStreet)
						edgesStr += "\"annotation_text\": \"cycleway\", \n";
					edgesStr += "\"interval\": [ \n " + to_string(startInterval) + ", \n " + to_string(nodesAmount - 1 - i) + "\n]";
					edgesStr += "}, \n";
					cycleStreet = isThisEdgeCycleStreet;
					startInterval = nodesAmount - 1 - i;
				}
			}
		}

		edgesStr += "{ \n";
		if (cycleStreet)
			edgesStr += "\"annotation_text\": \"cycleway\",";
		edgesStr += "\"interval\": [ \n " + to_string(startInterval) + ", \n " + to_string(nodesAmount - 1) + "\n]";
		edgesStr += "}";
		edgesStr += "\n";

		edgesStr += "],";
		coordsStr += "], \"type\": \"LineString\" } }] }";

		return edgesStr + coordsStr;
	}

	/* string Path::getJSON() {
		if (distance == -1)
			return " ";

		int amount = nodes.size();
		string result = "{ \'paths\':\n [{ \'instructions\': [\n";
		result += "{ \'distance\' : " + to_string(getDistance('m')) + ",\n \'interval\' : [0, " + to_string(amount - 1) + "] } ],\n";
		result += "\'points\':\n { \'coordinates\': [\n";
		for (int i = 0; i < amount; i++) {
			result += "[\n" + to_string(nodes[i].longtitude) + ",\n" + to_string(nodes[i].latitude) + "\n]\n";
			if (i + 1 <= amount)
				result += ",";
		}

		result += "], \'type\': \'LineString\'}";
		result += "\n";

		return result;
	} */
}