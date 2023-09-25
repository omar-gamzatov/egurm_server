#ifndef SESSION_H_
#define SESSION_H_

#include <includes.hpp>
#include <DBSession.hpp>

using callback = std::function<void(const error_code)>;

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket&& socket, std::shared_ptr<io::io_context> _context);
	~Session() = default;
	
private:
	std::shared_ptr<io::io_context> context;
	tcp::socket socket;
	boost::json::value client_info;
	error_code err_code;
	DBSession db;

	char buff[1024] = {};
	std::string write_msg;

	boost::asio::awaitable<void> send(std::string);
	boost::asio::awaitable<std::size_t> read();
	boost::asio::awaitable<void> clear_read_buffer();
	boost::asio::awaitable<void> get_client_info();
	boost::asio::awaitable<void> get_serial_number();
	boost::asio::awaitable<void> insert_test_info();
	boost::asio::awaitable<void> run_session();
	boost::asio::awaitable<void> get_resp(const char* buff);

	//boost::asio::awaitable<void> log(std::string &msg);
};

#endif // !SESSION_H_
