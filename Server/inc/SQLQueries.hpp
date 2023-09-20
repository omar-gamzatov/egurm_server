#ifndef SQL_QUERIES_H
#define SQL_QUERIES_H

#include <includes.hpp>

namespace sqlQueries {
    inline std::string get_serial_number() {
        return std::string("SELECT * FROM test_table ORDER BY id DESC LIMIT 1;");
    }

    inline std::string create_table_if_not_exists() {
        return std::string("CREATE TABLE IF NOT EXISTS public.json_test_table \n\
            (id             integer     NOT NULL     PRIMARY KEY,\n\
             worker_info    json        NOT NULL,\n\
             test_info      json\n\
            );");
    }
}



#endif // !SQL_QUERIES_H
