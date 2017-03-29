#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include "..\PathingLib\Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PathingLib;

namespace PathingTest
{		
	TEST_CLASS(GraphTest)
	{
	public:

		TEST_METHOD(NODE_addInEdge)
		{
			PathingLib::NODE n;
			n.addInEdge(11);
			n.addInEdge(22);
			Assert::AreEqual(2, n.in_edges_amount, L"Incorrect in edges amount", LINE_INFO());
			Assert::AreEqual(11, n.in_edges[0], L"Incorrect in edge value", LINE_INFO());
		}

		TEST_METHOD(NODE_addOutEdge)
		{
			PathingLib::NODE n;
			n.addOutEdge(11);
			n.addOutEdge(22);
			Assert::AreEqual(2, n.out_edges_amount, L"Incorrect out edges amount", LINE_INFO());
			Assert::AreEqual(11, n.out_edges[0], L"Incorrect out edge value", LINE_INFO());
		}

		TEST_METHOD(Graph_getNode)
		{
			PathingLib::Graph g(2, 2);
			bool exceptionThrownOutOfRange = false;
			try
			{
				g.getNode(0);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"exception not thrown", LINE_INFO());
		}

		TEST_METHOD(Graph_getEdge)
		{
			PathingLib::Graph g(2, 2);
			bool exceptionThrownOutOfRange = false;
			try
			{
				g.getEdge(0);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"exception not thrown", LINE_INFO());
		}

		TEST_METHOD(Graph_addNode)
		{
			PathingLib::Graph g(2, 0);
			g.addNode(1, 1);
			g.addNode(2, 2);
			PathingLib::NODE node = g.getNode(1);
			Assert::AreEqual(2.0, node.latitude, L"Incorrect node latitude", LINE_INFO());
			Assert::AreEqual(2.0, node.longtitude, L"Incorrect node longtitude", LINE_INFO());

			bool exceptionThrownOutOfRange = false;
			try
			{
				g.addNode(3, 3);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"exception not thrown", LINE_INFO());
		}

		TEST_METHOD(Graph_addDirectedEdge)
		{
			PathingLib::Graph g(2, 2);
			g.addNode(1, 1);
			g.addNode(2, 2);

			bool exceptionThrownOutOfRange = false;
			try
			{
				g.addDirectedEdge(3, 0, 1);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"Exception not thrown - source node range", LINE_INFO());

			exceptionThrownOutOfRange = false;
			try
			{
				g.addDirectedEdge(0, 3, 1);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"Exception not thrown - target node range", LINE_INFO());
			
			g.addDirectedEdge(0, 1, 1);
			g.addDirectedEdge(1, 0, 11);

			PathingLib::EDGE e = g.getEdge(1);

			Assert::AreEqual(1, e.source, L"Incorrect source of edge", LINE_INFO());
			Assert::AreEqual(0, e.target, L"Incorrect target of edge", LINE_INFO());
			Assert::AreEqual(11, e.distance, L"Incorrect distance of edge", LINE_INFO());

			Assert::AreEqual(1, g.getInEdgesAmount(0), L"Incorrect amount of in edges", LINE_INFO());
			Assert::AreEqual(1, g.getOutEdgesAmount(0), L"Incorrect source of out edges", LINE_INFO());

			exceptionThrownOutOfRange = false;
			try
			{
				g.addNode(3, 3);
			}
			catch (std::out_of_range ex)
			{
				exceptionThrownOutOfRange = true;
			}
			Assert::IsTrue(exceptionThrownOutOfRange, L"Exception not thrown - edges amount range", LINE_INFO());
		}

		TEST_METHOD(Graph_loadGraphFromFile)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			Assert::AreEqual(6, g.getNodesAmount(), L"Incorrect nodes amount", LINE_INFO());
			Assert::AreEqual(16, g.getEdgesAmount(), L"Incorrect edges amount", LINE_INFO());

			PathingLib::NODE n = g.getNode(2);
			Assert::AreEqual(0.0, n.latitude, L"Incorrect node latitude", LINE_INFO());
			Assert::AreEqual(2.0, n.longtitude, L"Incorrect node longtitude", LINE_INFO());

			PathingLib::EDGE e = g.getEdge(12);
			Assert::AreEqual(4, e.source, L"Incorrect edge source", LINE_INFO());
			Assert::AreEqual(2, e.target, L"Incorrect edge target", LINE_INFO());
			Assert::AreEqual(100, e.distance, L"Incorrect edge distance", LINE_INFO());

			bool exceptionRuntimeError = false;
			try
			{
				PathingLib::Graph g_invalidFiles = PathingLib::Graph::loadGraphFromFile("notexistfile", "notexistfile");
			}
			catch (std::runtime_error ex)
			{
				exceptionRuntimeError = true;
			}
			Assert::IsTrue(exceptionRuntimeError, L"Exception not thrown - invalid files", LINE_INFO());
		}

		TEST_METHOD(Graph_saveToFile)
		{
			PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../PathingTest/nodes.txt", "../PathingTest/edges.txt");
			g.saveToFile("../PathingTest/nodesSaveTest.txt", "../PathingTest/edgesSaveTest.txt");
		}

	};
}