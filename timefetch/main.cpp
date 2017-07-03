#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

extern "C" struct sockaddr_in;

using namespace std;

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in my_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (errno) {
		cerr << "Failed to call socket()" << endl;
		exit(1);
	}

	my_addr.sin_family = AF_INET; // AF_INET means we want to send stuff via IP
	my_addr.sin_port = 0; // Choose the port automatically
	my_addr.sin_addr.s_addr = INADDR_ANY; // Choose this machine's IP address
	memset(&(my_addr.sin_zero), 0x00, 8);

	bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
	return 0;
}
