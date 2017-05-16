#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include "..\PathingLib\Graph.h"
#include "..\PathingLib\ALT.h"
#include "..\PathingLib\Utility.h"

using namespace std;
using namespace PathingLib;

const string isServerTakenFPath = "../server\\taken.txt";
const string isServerDoneWork = "../server\\workfinnished.txt";
const string serverOutputFPath = "../server\\output.txt";
const string coordsArgsFPath = "../server\\coords.txt";
const string goWorkFPath = "../server\\gowork.txt";


void freeFile_(string path) {
	std::ofstream ofs;
	ofs.open(path, fstream::out);
	ofs.close();
}

bool isFileTaken(string path) {
	ifstream f(path);
	streampos begin, end;
	begin = f.tellg();
	f.seekg(0, ios::end);
	end = f.tellg();
	bool result = false;

	if (end - begin > 0)
		result = true;

	f.close();
	return result;
}

void takeFile(string path) {
	freeFile_(path);
	ofstream f(path);
	f << "1";
	f.close();
}

void printInFile(string path, string text) {
	freeFile_(path);
	ofstream f(path);
	f << text;
	f.close();
}

void printFromFile(string path) {
	string line;
	ifstream f(path);
	while (getline(f, line))
	{
		cout << line << '\n';
	}
	f.close();
}

std::vector<std::string> separateLine(std::string line) {
	string buf;
	stringstream ss(line);

	vector<string> tokens;

	while (ss >> buf) {
		tokens.push_back(buf);
	}

	return tokens;
}

int main(int argc, char *argv[])
{
	cout << "Loading graph" << endl;
	PathingLib::Graph g = PathingLib::Graph::loadGraphFromFile("../server/podlaskieNODES.txt", "../server/podlaskieEDGES.txt");
	cout << "Prepare landmarks" << endl;
	PathingLib::ALT alt(g, 4, false);

	freeFile_(isServerTakenFPath);
	freeFile_(isServerDoneWork);
	freeFile_(serverOutputFPath);
	freeFile_(coordsArgsFPath);
	freeFile_(goWorkFPath);

	cout << "Server is running" << endl;
	while (true) {
		if (isFileTaken(goWorkFPath)) {
			float x1 = 5, y1 = 0, x2 = -5, y2 = 0;

			ifstream coordinatesFile(coordsArgsFPath);
			for (string line; getline(coordinatesFile, line); )
			{
				std::vector<std::string> nodeArgs;
				nodeArgs = separateLine(line);

				x1 = std::stod(nodeArgs[0].c_str());
				y1 = std::stod(nodeArgs[1].c_str());
				x2 = std::stod(nodeArgs[2].c_str());
				y2 = std::stod(nodeArgs[3].c_str());
			}

			cout << "Run pathfinding: " << x1 << " " << y1 << ", " << x2 << ", " << y2 << endl;
			 string pathJSON = alt.getPathJSON(x1, y1, x2, y2);
			 cout << "    To file" << endl;
			printInFile(serverOutputFPath, pathJSON);
			cout << "    Free files" << endl;
			freeFile_(goWorkFPath);
			takeFile(isServerDoneWork);
		}
	}

	return 0;
}
