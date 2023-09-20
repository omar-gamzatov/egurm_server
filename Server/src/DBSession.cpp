#include <DBSession.hpp>

DBSession::DBSession() {}

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
        w.exec(sqlQueries::create_table_if_not_exists());
        w.commit();
        DBConnection::free_connection(std::move(conn));
        return;
    } catch (std::exception& e) {
        std::cout << e.what();
    }
}

boost::asio::awaitable<int> DBSession::get_serial_number() {
    try {
        start();
        auto row = wrk->exec(sqlQueries::get_serial_number()); // *****
        wrk->commit();
        stop();
        co_return row[0]["id"].as<int>();
    } catch (std::exception& e) {
        std::cout << e.what();
    }
    co_return 0;
}
