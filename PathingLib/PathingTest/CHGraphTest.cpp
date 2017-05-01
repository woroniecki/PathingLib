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
			string result = " ";
			/* Assert::AreEqual(result, chgraph.getPath(0, 5));
			Assert::AreEqual(result, chgraph.getPath(0, 2));
			Assert::AreEqual(result, chgraph.getPath(0, 3));
			Assert::AreEqual(result, chgraph.getPath(0, 4)); */
		}

		TEST_METHOD(CHGraph_Constructor_from_files)
		{
			PathingLib::CHGraph chgraph("../PathingTest/CHnodes.txt", "../PathingTest/CHedges.txt");
			string result = chgraph.getPath(0, 5);
			unsigned first = result.find("nce' : ");
			unsigned last = result.find(" 'interval");
			int distResult = int(std::stod(result.substr(first + 7, last - first - 9)));
			Assert::AreEqual(400, distResult);

			result = chgraph.getPath(0, 2);
			first = result.find("nce' : ");
			last = result.find(" 'interval");
			distResult = int(std::stod(result.substr(first + 7, last - first - 9)));
			Assert::AreEqual(200, distResult);

			result = chgraph.getPath(3, 5);
			first = result.find("nce' : ");
			last = result.find(" 'interval");
			distResult = int(std::stod(result.substr(first + 7, last - first - 9)));
			Assert::AreEqual(400, distResult);

			result = chgraph.getPath(1, 3);
			first = result.find("nce' : ");
			last = result.find(" 'interval");
			distResult = int(std::stod(result.substr(first + 7, last - first - 9)));
			Assert::AreEqual(200, distResult);
		}

	};
}