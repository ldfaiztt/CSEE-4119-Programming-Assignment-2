#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 6) {
		cout << "Usage: ./receiver <filename> <listening_port> <sender_IP> <sender_port> <log_filename>" << endl;
		exit(1);
	}
	ofstream file(argv[1]);
	if (!file.is_open()) {
		cout << "Could not create " << argv[1] << endl;
		exit(1);
	}
	ofstream logFile;
	streambuf *logBuffer;
	if (string(argv[5]) != "stdout") {
		logFile.open(argv[5]);
		if (!logFile.is_open()) {
			cout << "Could not create " << argv[5] << endl;
			exit(1);
		}
		logBuffer = logFile.rdbuf();
	} else {
		logBuffer = cout.rdbuf();
	}
	ostream log(logBuffer);
	log << "Hello world" << endl;
	file.close();
	if (logFile.is_open())
		logFile.close();
	return 0;
}
