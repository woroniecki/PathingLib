#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

const string isServerTakenFPath = "../server\\taken.txt";
const string isServerDoneWork = "../server\\workfinnished.txt";
const string serverOutputFPath = "../server\\output.txt";
const string coordsArgsFPath = "../server\\coords.txt";
const string goWorkFPath = "../server\\gowork.txt";

float crashTime = 15;

void freeFile_(string path) {
	std::ofstream ofs;
	ofs.open(path, std::ofstream::out | std::ofstream::trunc);
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

string getUrlArg(string url, string arg) {
	int foundFirst = url.find(arg + "=");
	if (foundFirst != std::string::npos) {
		int founfLast = url.find("&" , foundFirst);
		if (founfLast == std::string::npos) {
			founfLast = url.length();
		}
		return url.substr(foundFirst + 3, founfLast - foundFirst - 3); 
	}
	return "";
}

int main()
{
	cout << "Content-type:text/html\r\n\r\n";

	char* url_chars = getenv("REQUEST_URI");
	string url(url_chars);

	//string url = "localhost/geo/path/path.cgi?x1=53.0947059&y1=23.168587&x2=53.1947059&y2=23.068587";

	float x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	string x1_s, y1_s, x2_s, y2_s;
	try {
		x1_s = getUrlArg(url, "x1");
		if (x1_s == "")
			return -1;
		x1 = std::stod(x1_s);

		y1_s = getUrlArg(url, "y1");
		if (y1_s == "")
			return -1;
		y1 = std::stod(y1_s);

		x2_s = getUrlArg(url, "x2");
		if (x2_s == "")
			return -1;
		x2 = std::stod(x2_s);

		y2_s = getUrlArg(url, "y2");
		if (y2_s == "")
			return -1;
		y2 = std::stod(y2_s);
	}
	catch(exception ex){
		return -2;
	}

	const clock_t begin_time = clock();

	bool done = false;

	while (true) {
		if (float(clock() - begin_time) / CLOCKS_PER_SEC > crashTime)
			done = true;

		if (!isFileTaken(isServerTakenFPath)) {
			takeFile(isServerTakenFPath);
			string coords = x1_s + " " + y1_s + " " + x2_s + " " + y2_s;
			printInFile(coordsArgsFPath, coords);
			freeFile_(isServerDoneWork);
			takeFile(goWorkFPath);

			while (true) {
				if (float(clock() - begin_time) / CLOCKS_PER_SEC > crashTime)
					done = true;

				if (isFileTaken(isServerDoneWork)) {
					printFromFile(serverOutputFPath);
					done = true;
				}

				if (done)
					break;
			}

			freeFile_(isServerTakenFPath);
			freeFile_(isServerDoneWork);
			freeFile_(serverOutputFPath);
			freeFile_(coordsArgsFPath);
			freeFile_(goWorkFPath);
		}

		if (done)
			break;
	}

	cout << " ";
	return 0;
}
