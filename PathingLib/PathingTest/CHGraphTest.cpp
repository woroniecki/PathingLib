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
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt", 100);
			PathingLib::ALT alt(g, 3, false);
			PathingLib::CHGraph chgraph(g, alt);
			chgraph.saveGraphToFile("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt");
			Assert::AreEqual(400, chgraph.getPath(0, 5));
			Assert::AreEqual(200, chgraph.getPath(0, 2));
			Assert::AreEqual(100, chgraph.getPath(0, 3));
			Assert::AreEqual(300, chgraph.getPath(0, 4));
		}

		TEST_METHOD(CHGraph_Constructor_from_files)
		{
			PathingLib::CHGraph chgraph("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt");
			Assert::AreEqual(400, chgraph.getPath(0, 5));
			Assert::AreEqual(200, chgraph.getPath(0, 2));
			Assert::AreEqual(100, chgraph.getPath(0, 3));
			Assert::AreEqual(300, chgraph.getPath(0, 4));
		}

	};
}