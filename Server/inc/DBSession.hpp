#ifndef DBSESSION_H
#define DBSESSION_H

#include <DBConnection.hpp>

class DBSession
{
private:
    boost::shared_ptr<pqxx::connection> connection;
public:
    DBSession();
    ~DBSession();
};

DBSession::DBSession()
{
    connection = std::move(DBConnection::get_connection());
}

DBSession::~DBSession()
{
    DBConnection::free_connection(std::move(connection));
}


#endif // !DBSESSION_H