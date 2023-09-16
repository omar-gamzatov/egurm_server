#ifndef SERVER_H
#define SERVER_H

#include <includes.hpp>
#include <Session.hpp>

class Server
{
public:
	Server(uint16_t port);
	~Server() = default;

private:
	uint16_t port;
	boost::shared_ptr<io::io_context> context;
	std::optional<tcp::acceptor> acceptor;
	std::optional<tcp::socket> socket;
	std::vector<std::shared_ptr<Session>> clients;

	void accept();
	std::function<void()> run = [&] () {
		accept();
		context->run();
	};

};

#endif // !SERVER_H
