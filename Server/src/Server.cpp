#include <Server.hpp>

Server::Server(uint16_t _port = 3001)
	: context(boost::make_shared<io::io_context>()), port(_port), acceptor(tcp::acceptor(*context, tcp::endpoint(tcp::v4(), port))) {
	boost::thread server_thread(run);
	server_thread.join();
}

void Server::accept() {
	error_code ec;
	socket = tcp::socket(*context);
	std::function<void(error_code&)> acc_handler = [&] (error_code& ec) {
		if (!ec) {
			std::cout << "Connetion handled: " << socket.value().remote_endpoint() << std::endl;
			auto client = std::make_shared<Session>(
				std::move(socket.value()), 
				context
			);
			clients.push_back(client);
			accept();
		} else {
			context->stop();
		}
	};
	acceptor.value().async_accept(*socket, boost::bind(acc_handler, ec));
}