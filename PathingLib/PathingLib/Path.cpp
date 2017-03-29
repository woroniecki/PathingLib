#include "stdafx.h"
#include "Graph.h"
#include "Utility.h"
#include "Path.h"
#include <fstream>
#include <sstream>

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
}