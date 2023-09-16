#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <includes.hpp>

class DBConnection
{
private:
    static std::string db_access;
    static unsigned int pool_size;
    static boost::lockfree::queue<boost::shared_ptr<pqxx::connection>> pool;
    static boost::fibers::mutex mtx;
    static boost::fibers::condition_variable empty_pool;

    static bool add_connection();

public:
    DBConnection();
    ~DBConnection() = default;
    static void create_pool(std::string db_access, unsigned int pool_size);
    static boost::shared_ptr<pqxx::connection>&& get_connection();
    static bool free_connection(boost::shared_ptr<pqxx::connection> ptr);
    static void close_connections();
};

#endif // !DBCONNECTION_H