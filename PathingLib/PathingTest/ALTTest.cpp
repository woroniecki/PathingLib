#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include <vector>
#include "..\PathingLib\Path.h"
#include "..\PathingLib\Graph.h"
#include "..\PathingLib\ALT.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PathingLib;

namespace PathingTest
{
	TEST_CLASS(ALTTest)
	{
	public:

		TEST_METHOD(ALT_Constructor)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			PathingLib::ALT alt_random(g, 3);
			PathingLib::ALT alt_furthest(g, 3, false);

			bool exceptionThrownInvalidArgument = false;
			try {
				PathingLib::ALT alt_exception(g, 6, false);
			}
			catch(std::invalid_argument ex){
				exceptionThrownInvalidArgument = true;
			}
			Assert::AreEqual(true, exceptionThrownInvalidArgument, L"Invalid argument not thrown", LINE_INFO());

			exceptionThrownInvalidArgument = false;
			try {
				PathingLib::ALT alt_exception(g, 0, false);
			}
			catch (std::invalid_argument ex) {
				exceptionThrownInvalidArgument = true;
			}
			Assert::AreEqual(true, exceptionThrownInvalidArgument, L"Invalid argument not thrown", LINE_INFO());
		}

		TEST_METHOD(ALT_getPath)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			PathingLib::ALT alt(g, 2, false);

			PathingLib::Path p = alt.getPath(0, 5);
			Assert::AreEqual(3.1, p.getDistance('m'), L"Wrong distance of path", LINE_INFO());
			Assert::AreEqual(5, p.getNodesAmount(), L"Wrong nodes amount in path", LINE_INFO());
			
			std::vector<PathingLib::NODE> nodes = p.nodes;
			Assert::AreEqual(0, nodes[0].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(1, nodes[1].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(2, nodes[2].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(4, nodes[3].index, L"Wrong node index in created path", LINE_INFO());
			Assert::AreEqual(5, nodes[4].index, L"Wrong node index in created path", LINE_INFO());

			PathingLib::ALT alt_furthest(g, 2, true);

			PathingLib::Path p_ = alt_furthest.getPath(0, 5);
			Assert::AreEqual(3.1, p_.getDistance('m'), L"Wrong distance of path in alt with the furthest landmarks distance", LINE_INFO());
			Assert::AreEqual(5, p_.getNodesAmount(), L"Wrong nodes amount of path with the furthest landmarks distance", LINE_INFO());
		
			int distance = alt.getPathDist(0, 5);
			Assert::AreEqual(310, distance, L"Wrong distance of path", LINE_INFO());
		}

	};
}