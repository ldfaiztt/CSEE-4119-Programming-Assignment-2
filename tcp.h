#define MSS 576
#define HEADER_SIZE 20

struct tcp_header {
	unsigned short source_port;
	unsigned short destination_port;
	unsigned int sequence_number;
	unsigned int acknowledgment_number;
	unsigned short offset_and_flags;
	unsigned short window_size;
	unsigned short checksum;
	unsigned short urgent_pointer;
};

struct tcp_packet {
	struct tcp_header header;
	char payload[MSS - sizeof(struct tcp_header)];
};
