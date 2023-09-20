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
    ~DBSession() = default;
    void start();
    void stop();

    boost::asio::awaitable<int> get_serial_number();
    static void create_table_if_not_exists();

};

#endif // !DBSESSION_H
