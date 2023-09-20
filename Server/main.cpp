#include <Server.hpp>
#include <Logger.hpp>
#include <DBConnection.hpp>

using namespace pqxx;

void close_incorrect_parameters() {
	BOOST_LOG_TRIVIAL(fatal) << "Параметры запуска не введены или введены неправильно\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Press any key to close" << std::endl;
	std::cin.get();
}

int main(int argc, char* argv[]) {
	std::setlocale(LC_ALL, "rus");
	logger::init();

	// check if compiler parameters is ok
	if (argc < 4) {
		close_incorrect_parameters();
		return 0;
	}
	
	// create db pool
	DBConnection::create_pool(argv[2], std::stoi(argv[3]));
	std::cout << "pool created\n";

	// check if table not exists
	DBSession::create_table_if_not_exists();
	std::cout << "table created\n";
	
	// start server work
	Server serv(uint16_t(std::stoi(argv[1])));
	
	using namespace std::chrono_literals;
	while (true) {
		std::this_thread::sleep_for(1s);
		//std::cout << "1 second left\n";
	}

	std::cout << "Press any key to close" << std::endl;
	DBConnection::close_connections();
	std::cin.get();
	return 0;
}
