#include <Session.hpp>

Session::Session(tcp::socket&& _soc, boost::shared_ptr<io::io_context> _context)
	: socket(std::move(_soc)), 
		context(_context), 
		username(std::to_string(socket.remote_endpoint().port())) 
		{
	io::co_spawn(*context, run_session(), io::detached);
}

void Session::set_name(std::string name) {
	username = name;
}

std::string Session::get_resp(const char* _buff) {
	std::string msg(_buff);
	if (msg == "get name")
		return "server";
	else if (msg == "exit") {
		// exit();
		return "exit";
	}
	else
		return "unknown message";
}
boost::asio::awaitable<void> Session::log(std::string &msg) {
	BOOST_LOG_TRIVIAL(trace) << "user " << username << ": " << msg << std::endl;
	co_return; 
}

boost::asio::awaitable<void> Session::run_session() {
	try
	{
		while (true) {
			std::size_t n = co_await socket.async_read_some(io::buffer(buff), io::use_awaitable);
				std::cout << username <<"(" << n << "bytes): " << buff << std::endl;
			std::string msg(get_resp(buff));
			co_await log(msg);
			if (msg == "exit") {
				break;
			}
			else {
				co_await socket.async_write_some(io::buffer(msg), io::use_awaitable);
				memset(&buff[0], 0, sizeof(buff));
			}
		}
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << '\n';
	}
}

