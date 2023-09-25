#include <iostream>
#include <sstream>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <Client.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using error_code = boost::system::error_code;

auto main() -> int {
    std::vector<std::shared_ptr<Client>> clients;

    asio::io_context context;

    auto cl = std::make_shared<Client>(context, 0);
    clients.push_back(cl);
    context.run();
    
    return 0;
}