#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using error_code = boost::system::error_code;

auto main() -> int {
    error_code ec;
    asio::streambuf buffer;
    asio::io_context context;
    tcp::endpoint ep(asio::ip::address_v4::from_string("127.0.0.1"), 2001);
    tcp::socket sock(context);
    std::function<void(error_code&)> write_handle;
    std::function<void(error_code&)> read_handle;

    write_handle = [&] (error_code& ec) {
        if (!ec)
            asio::async_read(sock, buffer, boost::bind(read_handle, ec));
        else
            throw std::runtime_error{"Error on write\n"};
    };
    read_handle = [&] (error_code& ec) {
        if (!ec) {
            std::cout << "Recieved from server: " << asio::buffer_cast<const char*>(buffer.data()) << std::endl;
            buffer.consume(buffer.size());
            std::string msg;
            std::getline(std::cin, msg);
            if (msg == "exit") {
                sock.close();
                context.stop();
                return;
            }
            asio::async_write(sock, buffer, boost::bind(write_handle, ec));
        } else {
            throw std::runtime_error{"Error on read\n"};
        }

    };
    
    asio::async_write(sock, asio::buffer("asdasd"), boost::bind(write_handle, ec));


    context.run();
    return 0;
}