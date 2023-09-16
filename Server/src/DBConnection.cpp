#include <DBConnection.hpp>

DBConnection::DBConnection() {}

void DBConnection::create_pool(std::string _db_access, unsigned int _pool_size = 10) {
    db_access = _db_access;
    pool_size = _pool_size;
    unsigned int tmp_cnt = 0;
    for (unsigned int i = 0; i < pool_size; ++i) {
        try {
            boost::shared_ptr<pqxx::connection> db_ptr = boost::make_shared<pqxx::connection>(db_access);
            if (db_ptr->is_open()) {
                pool.bounded_push(db_ptr);
                ++tmp_cnt;
            } else {
                --i;
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    if (tmp_cnt == pool_size) {
        // logger << "database connection pool created";
    } else {
        // loger << database connection pool not created correctly";
    }
}

boost::shared_ptr<pqxx::connection>&& DBConnection::get_connection() {
    if (pool.empty()) {
        std::unique_lock<boost::fibers::mutex> lock(mtx);
        while (pool.empty()) {
            empty_pool.wait(lock);
        }
    }
    boost::shared_ptr<pqxx::connection> db_ptr;
    pool.pop(db_ptr);
    return std::move(db_ptr);
}

bool DBConnection::free_connection(boost::shared_ptr<pqxx::connection> ptr) {
    if (ptr->is_open() && pool.bounded_push(ptr)) {
        empty_pool.notify_one();
        return true;
    }
    else {
        return add_connection();
    }
}


bool DBConnection::add_connection() {
    boost::shared_ptr<pqxx::connection> db_ptr = boost::make_shared<pqxx::connection>(db_access);
    if (db_ptr->is_open()) {
        if (pool.bounded_push(db_ptr)) {
            // logger << "new database connection added";
            empty_pool.notify_one();
            return true;
        } else {
            // logger << "new database connection failed: internal memory pool is exhausted"
            return false;
        }
    } else {
        // logger << "new database connection failed: connection closed";
        return false;
    }
}

void DBConnection::close_connections() {
    pool.consume_all([&] (boost::shared_ptr<pqxx::connection> ptr) {
        ptr->close();
    });
}