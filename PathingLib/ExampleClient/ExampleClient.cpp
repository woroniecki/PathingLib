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
	cout << "Loading graph" << endl;
	PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
	cout << "Prepare landmarks" << endl;
	PathingLib::ALT alt(g, 4, false);

	cout << "Run path finding algorithm" << endl;
	string path = alt.getPathJSON (0.0f, -1.0f, 1.0f, 3.0f);
	cout << path << endl;

	// p.saveToFileCSV("../ExampleClient/examplePath.txt");

	/* cout << "Loading graph" << endl;
	PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../ExampleClient/podlaskieNODES.txt", "../ExampleClient/podlaskieEDGES.txt", 6000000, 0.65f);
	cout << "Prepare landmarks" << endl;
	PathingLib::ALT alt(g, 4, false);
	cout << "Preprocessing CH" << endl;
	PathingLib::CHGraph chgraph(g, alt);
	cout << "Save" << endl;
	chgraph.saveGraphToFile("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt"); 

	PathingLib::CHGraph chgraph("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt");
	cout << chgraph.getPath(0, 5) << endl; */
    return 0;
}

