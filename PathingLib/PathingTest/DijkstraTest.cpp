#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include <vector>
#include "..\PathingLib\Path.h"
#include "..\PathingLib\Graph.h"
#include "..\PathingLib\Dijkstra.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PathingLib;

namespace PathingTest
{
	TEST_CLASS(DijkstraTest)
	{
	public:

		TEST_METHOD(Dijkstra_getPath)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			PathingLib::Path p = PathingLib::Dijkstra::getPath(0, 5, g);
			Assert::AreEqual(4.0, p.getDistance('m'), L"Wrong distance of path", LINE_INFO());
			Assert::AreEqual(5, p.getNodesAmount(), L"Wrong nodes amount in path", LINE_INFO());

			std::vector<PathingLib::NODE> nodes = p.nodes;
			Assert::AreEqual(0, nodes[0].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(1, nodes[1].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(2, nodes[2].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(4, nodes[3].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(5, nodes[4].index, L"Wrong node index in created path", LINE_INFO());
		}

	};
}