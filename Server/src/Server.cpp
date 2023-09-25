#include <Server.hpp>

Server::Server(uint16_t _port = 3001)
	: port(_port),
	  context(std::make_shared<io::io_context>()),
	  acceptor(tcp::acceptor(*context, tcp::endpoint(tcp::v4(), port))) {
	boost::thread server_thread(run);
	server_thread.join();
}

void Server::accept() {
	error_code ec;
	socket = tcp::socket(*context);
	std::function<void(error_code&)> acc_handler = [&] (error_code& ec) {
		if (!ec) {
			auto client = std::make_shared<Session>(
				std::move(socket.value()), 
				context
			);
			clients.push_back(client);
			accept();
		} else {
			std::cerr << ec.what() << std::endl; // log
		}
	};
	acceptor.value().async_accept(*socket, boost::bind(acc_handler, ec));
}