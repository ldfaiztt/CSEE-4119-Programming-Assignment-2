Thomas Huzij
Programming Assignment #2

To compile:
make

To run sender:
./sender <filename> <remote_IP> <remote_port> <ack_port_num> <log_filename>
Example: ./sender sender.cpp 128.59.15.41 2109 2110 senderLog.txt
To run receiver:
./receiver <filename> <listening_port> <sender_IP> <sender_port> <log_filename>
Example: ./receiver file.txt 2109 128.59.15.30 2110 receiverLog.txt

Sample sender output:
Delivery completed successfully
Total bytes sent = 4995
Segments sent = 9
Segments retransmitted = 0

Sample receiver output:
Delivery completed successfully

Sample sender log:
1415420932,	0.0.0.0,	128.59.15.41,	0,	0,	ACK: 1	FIN: 0,	2
1415420932,	255.127.0.0,	255.127.0.0,	0,	576,	ACK: 1	FIN: 0,	2
1415420932,	0.0.0.0,	128.59.15.41,	576,	20,	ACK: 1	FIN: 0,	1.75013
1415420932,	128.59.15.41,	128.59.15.41,	20,	1152,	ACK: 1	FIN: 0,	1.75013
1415420932,	0.0.0.0,	128.59.15.41,	1152,	40,	ACK: 1	FIN: 0,	1.53165
1415420932,	128.59.15.41,	128.59.15.41,	40,	1728,	ACK: 1	FIN: 0,	1.53165
1415420932,	0.0.0.0,	128.59.15.41,	1728,	60,	ACK: 1	FIN: 0,	1.34033
1415420932,	128.59.15.41,	128.59.15.41,	60,	2304,	ACK: 1	FIN: 0,	1.34033
1415420932,	0.0.0.0,	128.59.15.41,	2304,	80,	ACK: 1	FIN: 0,	1.17314
1415420932,	128.59.15.41,	128.59.15.41,	80,	2880,	ACK: 1	FIN: 0,	1.17314
1415420932,	0.0.0.0,	128.59.15.41,	2880,	100,	ACK: 1	FIN: 0,	1.02664
1415420932,	128.59.15.41,	128.59.15.41,	100,	3456,	ACK: 1	FIN: 0,	1.02664
1415420932,	0.0.0.0,	128.59.15.41,	3456,	120,	ACK: 1	FIN: 0,	0.898405
1415420932,	128.59.15.41,	128.59.15.41,	120,	4032,	ACK: 1	FIN: 0,	0.898405
1415420932,	0.0.0.0,	128.59.15.41,	4032,	140,	ACK: 1	FIN: 0,	0.786474
1415420932,	128.59.15.41,	128.59.15.41,	140,	4608,	ACK: 1	FIN: 0,	0.786474
1415420932,	0.0.0.0,	128.59.15.41,	4608,	160,	ACK: 1	FIN: 1,	0.688535
1415420932,	128.59.15.41,	128.59.15.41,	160,	4995,	ACK: 1	FIN: 0,	0.688535

Sample receiver log:
1415420932,	128.59.15.30,	128.59.15.30,	0,	0,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	0,	576,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	576,	20,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	20,	1152,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	1152,	40,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	40,	1728,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	1728,	60,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	60,	2304,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	2304,	80,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	80,	2880,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	2880,	100,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	100,	3456,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	3456,	120,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	120,	4032,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	4032,	140,	ACK: 16	FIN: 0
1415420932,	0.0.0.0,	0.0.0.0,	140,	4608,	ACK: 16	FIN: 0
1415420932,	128.59.15.30,	128.59.15.30,	4608,	160,	ACK: 16	FIN: 1
1415420932,	0.0.0.0,	0.0.0.0,	160,	4995,	ACK: 16	FIN: 0

(a) The TCP header is a struct tcp_header created in tcp.h. It stores all the necessary values and exclusively uses shorts and ints to do so.
When sending the packets, another struct tcp_packet is used, which contains the header and the payload.
(b) The sender enters the state where it wants to send a packet. If it receives an ack before a timeout, it sends the next packet. Otherwise, keep sending the same segment.
The receiver waits for a packet to arrive. If the checksum is invalid or the packet has an invalid segment number, reject it and repeat the waiting process. Otherwise,
accept it and wait for the next valid segment.
(c) The loss recovery mechanism is simple. If a packet is lost, obviously the sender will timeout because it will never receive an ack, and simply resends the segment.
The receiver will eventually receive the segment in this manner.

Nothing unusual to report.
