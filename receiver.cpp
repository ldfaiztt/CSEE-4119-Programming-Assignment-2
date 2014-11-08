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
	int sequence_number = 0;
	int acknowledgment_number = 0;
	int n;
	struct sockaddr_in ephemeral;
	socklen_t ephemeralSize = sizeof(ephemeral);
	bzero(&ephemeral, ephemeralSize);
	bzero(&packet, sizeof(packet));
	while ((n = recvfrom(sockfd, &packet, MSS, 0, (struct sockaddr *) &ephemeral, &ephemeralSize)) > 0) {
		log << (int) time(NULL) << ",\t" << inet_ntoa(ephemeral.sin_addr) << ",\t" << inet_ntoa(receiver.sin_addr) << ",\t" << packet.header.sequence_number << ",\t" << packet.header.acknowledgment_number << ",\tACK: " << (packet.header.offset_and_flags & (1 << 4)) << "\tFIN: " << (packet.header.offset_and_flags & 1) << endl;
		/* Using C implementation of Internet checksum from https://tools.ietf.org/html/rfc1071 */
		unsigned short checksum = packet.header.checksum;
		packet.header.checksum = 0;
		int packetSize = n;
		unsigned short *ptr = (unsigned short *) &packet;
		int sum = 0;
		while (packetSize > 1) {
			sum += *ptr++;
			packetSize -= 2;
		}
		if (packetSize > 0)
			sum += * (unsigned char *) ptr;
		while (sum >> 16)
			sum = (sum & 0xFFFF) + (sum >> 16);
		unsigned short newCheck = ~sum;
		if (newCheck != checksum)
			continue;
		if (packet.header.sequence_number != acknowledgment_number)
			continue;
		file.write(packet.payload, n - sizeof(packet.header));
		packet.header.sequence_number = sequence_number;
		sequence_number += HEADER_SIZE;
		packet.header.offset_and_flags |= 1 << 4;
		acknowledgment_number += n;
		packet.header.acknowledgment_number = acknowledgment_number;
		int end = packet.header.offset_and_flags & 1;
		packet.header.offset_and_flags = (0xFF - 1);
		if (sendto(ackfd, &packet, sizeof(packet.header), 0, (struct sockaddr *) &sender, sizeof(sender)) == -1) {
			perror("error");
			exit(1);
		}
		log << (int) time(NULL) << ",\t" << inet_ntoa(receiver.sin_addr) << ",\t" << inet_ntoa(sender.sin_addr) << ",\t" << packet.header.sequence_number << ",\t" << packet.header.acknowledgment_number << ",\tACK: " << (packet.header.offset_and_flags & (1 << 4)) << "\tFIN: " << (packet.header.offset_and_flags & 1) << endl;
		if (end)
			break;
		bzero(&packet, sizeof(packet));
	}
	file.close();
	if (logFile.is_open())
		logFile.close();
	close(sockfd);
	close(ackfd);
	cout << "Delivery completed successfully" << endl;
	return 0;
}
