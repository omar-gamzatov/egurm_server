#ifndef SESSION_H_
#define SESSION_H_

#include <includes.hpp>
#include <DBSession.hpp>

using callback = std::function<void(const error_code)>;

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket&& socket, boost::shared_ptr<io::io_context> _context);
	~Session() = default;
	void set_name(std::string name);
	
private:
	boost::shared_ptr<io::io_context> context;
	tcp::socket socket;
	std::string username;
	error_code err_code;
	DBSession db;

	char buff[1024] = {};
	std::string write_msg;

	boost::asio::awaitable<void> run_session();
	boost::asio::awaitable<void> get_resp(const char* buff);
	//boost::asio::awaitable<void> log(std::string &msg);
};

#endif // !SESSION_H_
