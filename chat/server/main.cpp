#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

class Printer
{
public:
	Printer(boost::asio::io_service &io, int count)
		: strand_(io),
		  timer1_(io, boost::posix_time::seconds(1)),
		  timer2_(io, boost::posix_time::seconds(1)),
		  count_(count)
	{
		timer1_.async_wait(boost::bind(&Printer::print1, this));
		timer2_.async_wait(boost::bind(&Printer::print2, this));
	}

	~Printer()
	{
		std::cout << "Printer done." << std::endl;
	}

	void print1()
	{
		if (count_ > 0) {
			std::cout << "Timer 1: " << --count_ << " executions left" << std::endl;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer1_.async_wait(strand_.wrap(boost::bind(&Printer::print1, this)));
		}
	}

	void print2()
	{
		if (count_ > 0) {
			std::cout << "Timer 2: " << --count_ << " executions left" << std::endl;

			timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(strand_.wrap(boost::bind(&Printer::print2, this)));
		}
	}

private:
	boost::asio::io_service::strand strand_;
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;
	int count_;
};

int main(int argc, char *argv[])
{
	boost::asio::io_service io;

	Printer p(io, 10);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io));

	std::cout << "Before io.run()" << std::endl;
	io.run();
	std::cout << "After io.run()" << std::endl;
	t.join();


	return 0;
}
