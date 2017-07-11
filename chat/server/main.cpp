#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code &e, boost::asio::deadline_timer& t, int& count)
{
	std::cout << "A timer thing No. " << count++ << std::endl;
}

int main(int argc, char *argv[])
{
	boost::asio::io_service io;

	boost::asio::deadline_timer t1(io, boost::posix_time::seconds(1));
	boost::asio::deadline_timer t2(io, boost::posix_time::seconds(2));
	t1.async_wait(&print);
	std::cout << "Between" << std::endl;
	t2.async_wait(&print);

	io.run();

	std::cout << "Meantime stuff" << std::endl;

	return 0;
}
