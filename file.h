#pragma once
#include <iostream>
#include <afxsock.h>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <signal.h>

#define BUFFER_SIZE 1024
using namespace std;

struct file {
	string name;
	string size;
};

vector <file> docdanhsachfile(string namefile);
void sendFileList(CSocket& clientSocket, const vector<file> a);
void sendFile(CSocket& clientSocket, const char* fileName);
void signalHandler(int signum);