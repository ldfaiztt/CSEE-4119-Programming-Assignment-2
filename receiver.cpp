#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"
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
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	int ackfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in receiver;
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(atoi(argv[2]));
	receiver.sin_addr.s_addr = htonl(INADDR_ANY);
	struct sockaddr_in sender;
	sender.sin_family = AF_INET;
	sender.sin_port = htons(atoi(argv[4]));
	if (!inet_aton(argv[3], &sender.sin_addr)) {
		cout << argv[3] << " is not a valid IP address" << endl;
		exit(1);
	}
	if (bind(sockfd, (struct sockaddr *) &receiver, sizeof(receiver)) == -1) {
		perror("error");
		exit(1);
	}
	struct tcp_packet packet;
	int n;
	while ((n = recvfrom(sockfd, &packet, MSS, 0, NULL, NULL)) > 0) {
		log.write(packet.payload, n - sizeof(packet.header));
	}
	file.close();
	if (logFile.is_open())
		logFile.close();
	return 0;
}
