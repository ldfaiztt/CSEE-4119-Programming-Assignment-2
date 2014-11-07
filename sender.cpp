#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 6) {
		cout << "Usage: ./sender <filename> <remote_IP> <remote_port> <ack_port_num> <log_filename> <window_size>" << endl;
		exit(1);
	}
	ifstream file(argv[1]);
	if (!file.is_open()) {
		cout << "Could not open " << argv[1] << endl;
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
