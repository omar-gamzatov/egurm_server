#ifndef LOGGER_H
#define LOGGER_H

#include <includes.hpp>

namespace logger {
    void init() {
        logging::add_file_log (
            keywords::open_mode = std::ios::out | std::ios::app,
            keywords::file_name = "egurm_server_log/%d-%m-%Y__%H-%M-%S.%f.log",
            keywords::rotation_size = 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%] <%Severity%> :\t %Message%"
        );

        logging::core::get()->set_filter (   
            logging::trivial::severity >= logging::trivial::trace
        );
        
    	logging::add_common_attributes();
    }
}



#endif // !LOGGER_H