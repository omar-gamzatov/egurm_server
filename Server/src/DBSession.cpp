#include <DBSession.hpp>

DBSession::DBSession() {}

DBSession::~DBSession() {
    if (connection != nullptr) {
        DBConnection::free_connection(std::move(connection)); 
    }
}

void DBSession::start() {
    connection = std::move(DBConnection::get_connection()); //
    wrk = std::make_shared<pqxx::work>(*connection);
}

void DBSession::stop() {
    DBConnection::free_connection(std::move(connection));   //
    connection = nullptr;
}

void DBSession::create_table_if_not_exists() {
    try {
        auto conn = std::move(DBConnection::get_connection());
        auto w = pqxx::work(*conn);
        auto table_exists = w.exec(sqlQueries::check_if_table_exists());
        if (!table_exists[0]["exists"].as<bool>()) {
            w.exec(sqlQueries::create_table_if_not_exists());
            w.exec(sqlQueries::create_functions_if_not_exists());
        }
        w.commit();
        DBConnection::free_connection(std::move(conn));
    	std::cout << "table created\n"; // info log
        return;
    } catch (std::exception& e) {
        std::cout << e.what(); // error log
    }
}

boost::asio::awaitable<std::string> DBSession::get_serial_number(boost::json::value client_info) {
    try {
        start();
        auto result = wrk->exec(sqlQueries::get_serial_number(client_info)); // *****
        wrk->commit();
        stop();
        co_return result[0]["insert_new_serial_num"].as<std::string>();
    } catch (std::exception& e) {
        std::cout << e.what();
        co_return e.what(); // error log
    }
}

boost::asio::awaitable<std::string> DBSession::get_test_info(boost::json::value client_info) {
    try {
        start();
        auto result = wrk->exec(sqlQueries::get_test_info(client_info)); // *****
        wrk->commit();
        stop();
        if (result[0]["test_info"].is_null())
            co_return "{}";
        else
            co_return result[0]["test_info"].as<std::string>();
    } catch (std::exception& e) {
        std::cout << e.what();
        co_return e.what(); // error log
    }
}

boost::asio::awaitable<void> DBSession::insert_test_info(boost::json::value client_info) {
    try {
        start();
        auto result = wrk->exec(sqlQueries::insert_test_info(client_info)); // *****
        wrk->commit();
        stop();
        co_return;
    } catch (std::exception& e) {
        std::cout << e.what(); // error log
        co_return;
    }
}