#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code &e, boost::asio::deadline_timer *t, int *count)
{
	if (*count < 5) {
		std::cout << "A timer thing No. " << (*count)++ << std::endl;
		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
		t->async_wait(boost::bind(
		                  print,
		                  boost::asio::placeholders::error,
		                  t,
		                  count
		              ));
	}
}

int main(int argc, char *argv[])
{
	boost::asio::io_service io;

	int count = 0;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
	t.async_wait(boost::bind(
	                 print,
	                 boost::asio::placeholders::error,
	                 &t,
	                 &count
	             ));

	std::cout << "Meantime stuff" << std::endl;
	io.run();


	return 0;
}
