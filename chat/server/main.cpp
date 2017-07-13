#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std;
	time_t now = time(0);
	std::cout << "Got a request!" << std::endl;
	return ctime(&now);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: chatserv <port>" << std::endl;
		return 1;
	}
	try {
		boost::asio::io_service io;

		std::cout << "Using port " << argv[1] << std::endl;
		// Use port 13
		tcp::acceptor acceptor(
		    io,
		    tcp::endpoint(tcp::v4(), std::stoi(argv[1]))
		);

		for (;;) {
			tcp::socket socket(io);
			acceptor.accept(socket);

			std::string message = make_daytime_string();

			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
