#ifndef SQL_QUERIES_H
#define SQL_QUERIES_H

#include <includes.hpp>

namespace sqlQueries {

    inline std::string check_if_table_exists() {
        return std::string("SELECT EXISTS (\
            SELECT FROM information_schema.tables \
            WHERE  table_schema = 'public'\
            AND    table_name   = 'egurm'\
            );"
        );
    }

    inline std::string create_table_if_not_exists() {
        return std::string("CREATE TABLE IF NOT EXISTS egurm1 \
            (id             SERIAL  PRIMARY KEY,\
             name   	    text,\
             depart_num     integer,\
             stand_num      integer,\
             serial_year    integer,\
             serial_mon     integer,\
             serial_num     integer,\
			 date		    timestamp,\
			 test_info	    json\
            );"
        );
    }

    inline std::string create_functions_if_not_exists() {
            return std::string("CREATE FUNCTION get_last_serial_num() RETURNS integer AS $$\
                SELECT serial_num FROM egurm1 \
                    WHERE serial_year = date_part('year', CURRENT_DATE) \
                        AND serial_mon = date_part('month', CURRENT_DATE)\
                        AND serial_num != 0\
                    ORDER BY id DESC LIMIT 1;\
            $$ LANGUAGE sql;\n\
            \n\
            CREATE FUNCTION get_new_serial_num() RETURNS integer AS $$\
                DECLARE\
                    serial_number integer := get_last_serial_num();\
                BEGIN\
                    IF serial_number = 0\
                    THEN\
                        RETURN 1;\
                    ELSE\
                        RETURN serial_number + 1;\
                    END IF;\
                END\
            $$ LANGUAGE plpgsql;\n\
            \n\
            CREATE FUNCTION insert_new_serial_num(name text, depart integer, stand integer) RETURNS text AS $$\
            DECLARE\
                serial_number integer := get_new_serial_num();\
            BEGIN\
                INSERT INTO egurm1 (name, depart_num, stand_num, serial_year, serial_mon, serial_num, date, test_info)\
                    VALUES (\
                        insert_new_serial_num.name,\
                        insert_new_serial_num.depart,\
                        insert_new_serial_num.stand,\
                        date_part('year', CURRENT_DATE),\
                        date_part('month', CURRENT_DATE),\
                        serial_number,\
                        CURRENT_TIMESTAMP,\
                        '{}'\
                    );\
                RETURN format('%s.%s.%s',\
                            to_char(date_part('year', CURRENT_DATE), 'FM0999'),\
                            to_char(date_part('month', CURRENT_DATE), 'FM09'),\
                            to_char(serial_number, 'FM0999')\
                            );\
            END\
            $$ LANGUAGE plpgsql;");
    }

    inline std::string get_serial_number(boost::json::value client_info) {
         return std::string(std::format("SELECT insert_new_serial_num(\'%s\', %s, %s);", 
                std::string(client_info.at("name").as_string()),
                std::string(client_info.at("depart_num").as_string()),
                std::string(client_info.at("stand_num").as_string())
            ));
    }

    inline std::string get_test_info(boost::json::value client_info) {
        uint32_t serial_number = std::stoi(
            std::string(client_info.at("serial_num").as_string().end() - 4,
                        client_info.at("serial_num").as_string().end()
            ));
        return std::string(std::format("SELECT test_info FROM egurm1 WHERE serial_num = %d", serial_number));
    }

    inline std::string insert_test_info(boost::json::value client_info) {
        uint32_t serial_number = std::stoi(
            std::string(client_info.at("serial_num").as_string().end() - 4,
                        client_info.at("serial_num").as_string().end()
            ));
        return std::string(std::format("UPDATE egurm1 SET test_info = '%s' WHERE serial_num = %d", 
            std::string(client_info.at("test_info").as_string()), 
            serial_number
        ));
    }
}

#endif // !SQL_QUERIES_H
