#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace io = boost::asio;
using tcp = boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client>
{
private:
    tcp::socket socket;
    char buff[1024];
    std::string msg;
    std::string name = "";
    int id;

    io::awaitable<void> connect();
    io::awaitable<void> read();
    io::awaitable<void> write();
    io::awaitable<void> run();

public:
    Client(io::io_context& context, int id);
    ~Client() = default;

};



#endif // !CLIENT_H
