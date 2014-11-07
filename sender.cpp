#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "tcp.h"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 6) {
		cout << "Usage: ./sender <filename> <remote_IP> <remote_port> <ack_port_num> <log_filename>" << endl;
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
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	int ackfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in receiver;
	struct sockaddr_in sender;
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(atoi(argv[3]));
	if (!inet_aton(argv[2], &receiver.sin_addr)) {
		cout << argv[2] << " is not a valid IP address" << endl;
		exit(1);
	}
	sender.sin_family = AF_INET;
	sender.sin_port = htons(atoi(argv[4]));
	sender.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(ackfd, (struct sockaddr *) &sender, sizeof(sender)) == -1) {
		perror("error");
		exit(1);
	}
	int sequence_number = 0;
	struct tcp_packet packet;
	int n;
	while (!file.eof()) {
		bzero(&packet, sizeof(packet));
		packet.header.source_port = sender.sin_port;
		packet.header.destination_port = receiver.sin_port;
		packet.header.sequence_number = sequence_number;
		packet.header.offset_and_flags = 5 << 4;
		file.read(packet.payload, MSS - 20);
		n = file.gcount();
		sequence_number += n + sizeof(packet.header);
		if (n < MSS - 20) {
			packet.header.offset_and_flags |= 1;
		}
		if (sendto(sockfd, &packet, MSS, 0, (struct sockaddr *) &receiver, sizeof(receiver)) == -1) {
			perror("error");
			exit(1);
		}
	}
	file.close();
	if (logFile.is_open())
		logFile.close();
	return 0;
}
