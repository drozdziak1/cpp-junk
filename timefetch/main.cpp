#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

extern "C" {
	struct sockaddr_in;
	struct addrinfo;

	typedef struct {

		unsigned li   : 2;       // Only two bits. Leap indicator.
		unsigned vn   : 3;       // Only three bits. Version number of the protocol.
		unsigned mode : 3;       // Only three bits. Mode. Client will pick mode 3 for client.

		uint8_t stratum;         // Eight bits. Stratum level of the local clock.
		uint8_t poll;            // Eight bits. Maximum interval between successive messages.
		uint8_t precision;       // Eight bits. Precision of the local clock.

		uint32_t rootDelay;      // 32 bits. Total round trip delay time.
		uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
		uint32_t refId;          // 32 bits. Reference clock identifier.

		uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
		uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

		uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
		uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

		uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
		uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

		uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
		uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

	} ntp_packet;              // Total: 384 bits or 48 bytes.
};

using namespace std;

int main(int argc, char *argv[])
{
	int sockfd, retval = 0;
	ssize_t sendto_retval, recv_retval;

	string address;

	ntp_packet packet = {};

	*( ( char * ) &packet + 0 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.


	struct addrinfo *result, *resp_node;

	struct addrinfo hints = {};
	hints.ai_family = AF_UNSPEC; // IPv4 and v6 are allowed
	hints.ai_socktype = SOCK_DGRAM;

	if (argc != 2) {
		address = "pool.ntp.org";
		cerr << "Wrong number of arguments, using the default server "
		     << '"' << address << '"' << endl;
	} else {
		address = argv[1];
	}

	if (int e = getaddrinfo(address.c_str(), "123", &hints, &result)) {
		cerr << "getaddrinfo() failed with" << e << endl;
		exit(1);
	}

	for (resp_node = result; resp_node != nullptr; resp_node = resp_node->ai_next) {
		sockfd = socket(resp_node->ai_family, resp_node->ai_socktype, resp_node->ai_protocol);

		if (sockfd == -1)
			continue;

		if (connect(sockfd, resp_node->ai_addr, resp_node->ai_addrlen) == 0)
			break;

		close(sockfd);
	}

	if (resp_node == nullptr) {
		cerr << "Could not connect() to any of the results" << endl;
		exit(1);
	}

	struct sockaddr_in *addr = (sockaddr_in *)resp_node->ai_addr;
	cout << "Connected to " << inet_ntoa(addr->sin_addr) << endl;

	sendto_retval = send(sockfd, &packet, sizeof(packet), 0);
	if (sendto_retval != -1) {
		cout << "Successfuly sent " << sendto_retval << " bytes"
		     << endl;
	} else {
		cerr << "Failed to send the NTP packet" << endl;
		cerr << "Errno: " << gai_strerror(errno) << endl;
		retval = 1;
		goto fini;
	}

	recv_retval = recv(sockfd, &packet, sizeof(packet), 0);
	if (recv_retval != -1) {
		cout << "Successfuly received " << recv_retval << " bytes"
		     << endl;
	} else {
		cerr << "Failed to receive the NTP packet" << endl;
		retval = 1;
		goto fini;
	}

fini:
	close(sockfd);

	return retval;
}
