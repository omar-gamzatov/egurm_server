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
	if (argc < 4) {
		close_incorrect_parameters();
		return 0;
	}
	DBConnection::create_pool(argv[2], std::stoi(argv[3]));


	Server serv(uint16_t(std::stoi(argv[1])));

	using namespace std::chrono_literals;
	while (true) {
		std::this_thread::sleep_for(1s);
		//std::cout << "1 second left\n";
	}

	std::cout << "Press any key to close" << std::endl;
	std::cin.get();
	return 0;
}
