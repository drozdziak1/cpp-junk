/**
 * A crude NTPv3 client implementation, inspired by David Lettier's ntpclient
 * https://github.com/lettier/ntpclient
 * http://www.lettier.com
 *
 * Author: Stanisław Drozd <drozdziak1 at gmail>
 */

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>

#include <cstring>
#include <cstdio>
#include <iostream>

#define LEAP_NOWARN 0b00000000 // Leap indicator set to off
#define NTP_V3      0b00011000 // NTP version 3
#define NTP_MODE_3  0b00000011 // NTP mode 3 (the client mode)

/**
 * NTP's got it's own epoch in 1900-01-01 instead of 1970-01-01. The 70-year
 * difference needs to be compensated manually
 */
#define NTP_TO_UNIX(ntp) (ntp) - 2208988800ULL

extern "C" {
	struct sockaddr_in;
	struct addrinfo;

	typedef struct {
		/**
		 * li_vn_mode - leap, version and mode byte starting from MSB
		 *
		 * li - 2 most significant bits
		 * vn - 3 bits in the middle
		 * mode - 3 least significant bits
		 */
		uint8_t li_vn_mode;

		uint8_t stratum;          // stratum level
		uint8_t poll;             // maximum delay between server polls
		uint8_t precision;        // clock precision

		uint32_t root_delay;      // root delta
		uint32_t root_dispersion; // root epsilon
		uint32_t ref_id;          // clock reference type

		uint32_t ref_sec;         // reference seconds
		uint32_t ref_nsec;        // reference nanoseconds

		uint32_t orig_sec;        // originate seconds
		uint32_t orig_nsec;       // originate nanoseconds

		uint32_t rx_sec;          // received seconds
		uint32_t rx_nsec;         // received nanosecons

		uint32_t tx_sec;          // transmit seconds
		uint32_t tx_nsec;         // transmit nanoseconds

	} ntp_packet;
};

int main(int argc, char *argv[])
{
	int sockfd, retval = 0;
	ssize_t sendto_retval, recv_retval;
	time_t client_time, server_time;

	std::string address;

	ntp_packet packet = {};

	packet.li_vn_mode = LEAP_NOWARN | NTP_V3 | NTP_MODE_3;

	struct addrinfo *result, *node;

	struct addrinfo hints = {};
	hints.ai_family = AF_UNSPEC; // IPv4 and v6 are allowed
	hints.ai_socktype = SOCK_DGRAM;

	if (argc != 2) {
		address = "pool.ntp.org";
		std::cerr << "No server specified, using the default server "
		          << '"' << address << '"' << std::endl;
	} else {
		address = argv[1];
	}

	if (int e = getaddrinfo(address.c_str(), "123", &hints, &result)) {
		std::cerr << "getaddrinfo() failed with " << e << '\n'
		          << '"' << gai_strerror(e) << '"' << std::endl;
		exit(1);
	}

	for (node = result; node != nullptr; node = node->ai_next) {
		sockfd = socket(
		             node->ai_family,
		             node->ai_socktype,
		             node->ai_protocol
		         );

		if (sockfd == -1)
			continue;

		if (connect(sockfd, node->ai_addr, node->ai_addrlen) == 0)
			break;

		close(sockfd);
	}

	if (node == nullptr) {
		std::cerr << "Could not connect() to any of the results"
		          << std::endl;
		exit(1);
	}

	struct sockaddr_in *addr = (sockaddr_in *)node->ai_addr;
	std::cout << "Connected to " << inet_ntoa(addr->sin_addr) << std::endl;

	putchar('\n');

	sendto_retval = send(sockfd, &packet, sizeof(packet), 0);
	if (sendto_retval != -1) {
		std::cout << "Successfuly sent " << sendto_retval << " bytes"
		          << std::endl;
	} else {
		std::cerr << "Failed to send the NTP packet\n"
		          << '"' << gai_strerror(errno) << '"' << std::endl;
		retval = 1;
		goto fini;
	}

	recv_retval = recv(sockfd, &packet, sizeof(packet), 0);
	if (recv_retval != -1) {
		std::cout << "Successfuly received " << recv_retval << " bytes"
		          << std::endl;
	} else {
		std::cerr << "Failed to receive the NTP packet\n"
		          << '"' << gai_strerror(errno) << '"' << std::endl;
		retval = 1;
		goto fini;
	}

	putchar('\n');

	client_time = time(NULL);
	server_time = NTP_TO_UNIX(ntohl(packet.tx_sec));

	std::cout << "Client time: " << ctime(&client_time)
	          << "Server time: " << ctime(&server_time) << std::endl;

	if (int lag = abs(server_time - client_time)) {
		std::cout << "Client is " << lag << "seconds off" << '\n'
		          << std::endl;
	}

	printf(
	    "Response packet details (timestamps in NTP epoch):\n"
	    "Leap Indicator:\t\t0x%02X\n"
	    "NTP Version:\t\t0x%02X\n"
	    "NTP Mode:\t\t0x%02X\n"
	    "\n"
	    "Stratum:\t\t%u\n"
	    "Polling Interval:\t%u\n"
	    "Clock precision:\t%u\n"
	    "Root Delay:\t\t%lu\n"
	    "Root Dispersion:\t%lu\n"
	    "\n"
	    "Reference ID:\t\t%lu\n"
	    "Reference seconds:\t%lu\n"
	    "Reference nanoseconds:\t%lu\n"
	    "\n"
	    "Origin seconds:\t\t%lu\n"
	    "Origin nanoseconds:\t%lu\n"
	    "\n"
	    "Receive seconds:\t%lu\n"
	    "Receive nanoseconds:\t%lu\n"
	    "\n"
	    "Transmit seconds:\t%lu\n"
	    "Transmit nanoseconds:\t%lu\n",
	    packet.li_vn_mode >> 6,
	    (packet.li_vn_mode >> 3) % 8,
	    packet.li_vn_mode % 8,
	    packet.stratum,
	    packet.poll,
	    packet.precision,
	    ntohl(packet.root_delay),
	    ntohl(packet.root_dispersion),
	    ntohl(packet.ref_id),
	    ntohl(packet.ref_sec),
	    ntohl(packet.ref_nsec),
	    ntohl(packet.orig_sec),
	    ntohl(packet.orig_nsec),
	    ntohl(packet.rx_sec),
	    ntohl(packet.rx_nsec),
	    ntohl(packet.tx_sec),
	    ntohl(packet.tx_nsec)
	);


fini:
	close(sockfd);
	return retval;
}
