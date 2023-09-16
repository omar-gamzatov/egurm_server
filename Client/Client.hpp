#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace io = boost::asio;
using tcp = boost::asio::ip::tcp;

class Client : boost::enable_shared_from_this<Client>
{
private:
    io::io_context context;
    tcp::socket socket;

public:
    Client(/* args */);
    ~Client();
};

Client::Client(/* args */)
    : socket(context)
{
}

Client::~Client()
{
}



#endif // !CLIENT_H