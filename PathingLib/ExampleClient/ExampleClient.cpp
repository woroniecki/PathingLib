// ExampleClient.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include <iostream>  
#include <string> 
#include "..\PathingLib\Graph.h"
#include "..\PathingLib\ALT.h"
#include "..\PathingLib\Path.h"

#include "..\PathingLib\CHGraph.h"

using namespace std;
using namespace PathingLib;

int main()
{
	/* cout << "Loading graph" << endl;
	PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../ExampleClient/andorraNodes.txt", "../ExampleClient/andorraEdges.txt");
	cout << "Prepare landmarks" << endl;
	PathingLib::ALT alt(g, 4, false);

	cout << "Run path finding algorithm" << endl;
	PathingLib::Path p = alt.getPath(0, 1000);

	cout << "Path distance: " << p.getDistance('k') << " kilometers"<< endl;
	p.saveToFileCSV("../ExampleClient/examplePath.txt");

	PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt", 100);
	PathingLib::ALT alt(g, 3, false);
	PathingLib::CHGraph chgraph(g, alt);
	chgraph.saveGraphToFile("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt"); */

	PathingLib::CHGraph chgraph("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt");
	cout << chgraph.getPath(0, 5) << endl;

    return 0;
}

