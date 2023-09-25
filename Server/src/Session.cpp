#include <Session.hpp>

Session::Session(tcp::socket&& _soc, std::shared_ptr<io::io_context> _context)
	: socket(std::move(_soc)), context(_context) {
	io::co_spawn(*context, run_session(), io::detached);
}

// send message to client
io::awaitable<void> Session::send(std::string msg) {
	co_await socket.async_write_some(io::buffer(msg), io::use_awaitable);
	co_return;
}

// read message from client
io::awaitable<std::size_t> Session::read() {
	std::size_t n = co_await socket.async_read_some(io::buffer(buff), io::use_awaitable);
	co_return n;
}

//
io::awaitable<void> Session::clear_read_buffer() {
	memset(&buff[0], 0, sizeof(buff));
	co_return;
}

//
io::awaitable<void> Session::get_client_info() {
	std::size_t msg_size = co_await read();
	client_info = boost::json::parse(buff);
	std::cout << "User connected: " << socket.remote_endpoint() << " "
		<< client_info.at("name") 
		<< "(" << msg_size << "bytes): "
		<< std::endl; // trace log
}

//
io::awaitable<void> Session::get_serial_number() {
	auto serial_number = co_await db.get_serial_number(client_info); // trace log
	client_info.at("id") = 10;
	client_info.at("serial_num") = serial_number;
	write_msg = std::format("%s.%s.%s", 
		std::string(client_info.at("depart_num").as_string()),
		std::string(client_info.at("stand_num").as_string()),
		serial_number);
	co_return;
}

//
io::awaitable<void> Session::insert_test_info() {
	co_await clear_read_buffer();
	co_await read();
	client_info.at("test_info") = boost::json::parse(buff);
	co_await db.insert_test_info(client_info);
	co_return;
}

// eset response consider on request
io::awaitable<void> Session::get_resp(const char* _buff) {
	std::string msg(_buff);
	if (msg == "get serial number") {
		co_await get_serial_number();
	}
	else if (msg == "insert test info") {
		write_msg = "success";
	}
	else if (msg == "get test info") {
		auto res = co_await db.get_test_info(7); // trace log
		write_msg = res;
	}
	else if (msg == "exit") {
		write_msg = "exit";	// trace log
	}
	else {
		write_msg = "unknown message"; // trace log
	}
	co_return;
}

//
io::awaitable<void> Session::run_session() {
	try
	{
		co_await get_client_info();
		co_await clear_read_buffer();
		co_await send("connected successfully");
		while (true) {
			std::size_t msg_size = co_await read();
			std::cout << client_info.at("name") <<"(" << msg_size << "bytes): " << buff << std::endl; // trace log
			co_await get_resp(buff);
			if (write_msg == "exit") {
				break;
			}
			else {
				co_await send(write_msg);
				co_await clear_read_buffer();
			}
		}
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << '\n';
	}
}

