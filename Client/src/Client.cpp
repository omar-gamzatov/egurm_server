#include <Client.hpp>

Client::Client(io::io_context& context, int id)
    : socket(context), name(std::string("user") + std::to_string(id)), id(id)
{
    std::thread th([&] () {
        io::co_spawn(context, run(), io::detached);
    });
    th.join();
}

io::awaitable<void> Client::connect() {
    tcp::endpoint ep(io::ip::address_v4::from_string("127.0.0.1"), 3001);
    co_await socket.async_connect(ep, io::use_awaitable);
	co_return;
}

io::awaitable<void> Client::read() {
	co_await socket.async_read_some(io::buffer(buff), io::use_awaitable);
    std::cout << "Message recieved: " << buff << std::endl;
	memset(&buff[0], 0, sizeof(buff));
	co_return;
}

io::awaitable<void> Client::write() {
	co_await socket.async_write_some(io::buffer(msg), io::use_awaitable);
	co_return;
}

io::awaitable<void> Client::run() {
    co_await connect();
    msg = std::string("{\"name\":\"" + name + "\", \"depart_num\":\"10\", \"stand_num\":\"115\"}");
    co_await write();
    while (true) {
        int cnt;
        std::cin >> msg >> cnt;
        if (msg == "exit")
            break;
        for (int i = 0; i < cnt; ++i) {
            co_await write();
            co_await read();
            std::cout << i << std::endl;
        }
    }
    co_return;
}