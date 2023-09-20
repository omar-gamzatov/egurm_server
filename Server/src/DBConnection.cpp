#include <DBConnection.hpp>

DBConnection::DBConnection() {}

std::string DBConnection::db_access;
unsigned int DBConnection::pool_size;
std::queue<std::shared_ptr<pqxx::connection>> DBConnection::pool;
boost::fibers::mutex DBConnection::mtx;
boost::fibers::condition_variable DBConnection::pool_is_empty;

void DBConnection::create_pool(std::string _db_access, unsigned int _pool_size = 10) {
    std::scoped_lock<boost::fibers::mutex> lock(mtx);
    db_access = _db_access;
    pool_size = _pool_size;
    unsigned int tmp_cnt = 0;
    for (unsigned int i = 0; i < pool_size; ++i) {
        try {
            std::shared_ptr<pqxx::connection> db_ptr = std::make_shared<pqxx::connection>(db_access);
            if (db_ptr->is_open()) {
                pool.push(db_ptr);
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

std::shared_ptr<pqxx::connection> DBConnection::get_connection() {
    std::unique_lock<boost::fibers::mutex> lock(mtx);
    if (pool.empty()) {
        while (pool.empty()) {
            pool_is_empty.wait(lock);
        }
    }
    std::shared_ptr<pqxx::connection> db_ptr(pool.front());
    pool.pop();
    return db_ptr;
}

bool DBConnection::free_connection(std::shared_ptr<pqxx::connection>&& ptr) {
    if (ptr->is_open()) {
        std::unique_lock<boost::fibers::mutex> lock(mtx);
        pool.push(std::move(ptr));
        lock.unlock();
        pool_is_empty.notify_one();
        return true;
    }
    else {
        return add_connection();
    }
}

bool DBConnection::add_connection() {
    std::shared_ptr<pqxx::connection> db_ptr = std::make_shared<pqxx::connection>(db_access);
    if (db_ptr->is_open()) {
            std::unique_lock<boost::fibers::mutex> lock(mtx);
            pool.push(db_ptr);
            lock.unlock();
            // logger << "new database connection added";
            pool_is_empty.notify_one();
            return true;
            // logger << "new database connection failed: internal memory pool is exhausted"
    } else {
        // logger << "new database connection failed: connection closed";
        return false;
    }
}

void DBConnection::close_connections() {
    while (!pool.empty()) {
        pool.pop();
    }
}
