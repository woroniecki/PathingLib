#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include <vector>
#include "..\PathingLib\Graph.h"
#include "..\PathingLib\CHGraph.h"
#include "..\PathingLib\ALT.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PathingLib;

namespace PathingTest
{
	TEST_CLASS(CHGraphTest)
	{
	public:

		TEST_METHOD(CHGraph_Constructor)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			PathingLib::ALT alt(g, 3, false);
			PathingLib::CHGraph chgraph(g, alt);
		}

	};
}