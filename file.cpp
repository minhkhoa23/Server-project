#include "file.h"

vector <file> docdanhsachfile(string namefile) {
	ifstream f;
	vector <file> a;
	f.open(namefile, ios::in);
	if (!f.is_open()) {
		cout << "Mo tap tin that bai ";
		return a;
	}
	else {
		string line;
		int i = 0;
		while (getline(f, line)) {
			stringstream ss(line);
			file temp;
			ss >> temp.name;
			ss >> temp.size;
			a.push_back(temp);
			i++;
		}
		return a;
		f.close();
	}
}

//ham gui danh sach file cho client lua chon
void sendFileList(CSocket& clientSocket, const vector<file> a) {
	string filelist;
	for (const auto& fileinfo : a) {
		filelist += fileinfo.name + " " + fileinfo.size + "\n";
	}
	clientSocket.Send(filelist.c_str(), filelist.size());
}

//ham gui file ma client yeu cau
void sendFile(CSocket& clientSocket, const char* fileName) {
	ifstream f;
	f.open(fileName, ios::binary);
	if (!f.is_open()) {
		cout << "Mo tap tin that bai";
		return;
	}
	else {
		char buffer[BUFFER_SIZE];
		while (!f.eof()) {
			f.read(buffer, BUFFER_SIZE);
			int bytesRead = f.gcount();
			clientSocket.Send(buffer, bytesRead);
		}
		f.close();
	}
}

//ham xu ly phim tat ctrl + c
void signalHandler(int signum) {
	cout << "Caught signal " << signum << endl;
	exit(signum);
}