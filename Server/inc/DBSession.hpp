#ifndef DBSESSION_H
#define DBSESSION_H

#include <includes.hpp>
#include <DBConnection.hpp>
#include <SQLQueries.hpp>

class DBSession {
private:
    std::shared_ptr<pqxx::connection> connection;
    std::shared_ptr<pqxx::work> wrk;

public:
    DBSession();
    ~DBSession();

    void start();
    void stop();

    static void create_table_if_not_exists();

    boost::asio::awaitable<std::string> get_serial_number(boost::json::value client_info);
    boost::asio::awaitable<std::string> get_test_info(boost::json::value client_info);
    boost::asio::awaitable<void> insert_test_info(boost::json::value client_info);

};

#endif // !DBSESSION_H
