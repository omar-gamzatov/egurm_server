#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
// std
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <queue>
#include <tuple>
#include <utility>
#include <thread>
#include <chrono>
#include <optional>
#include <initializer_list>
#include <ctime>
//#include <condition_variable>

// boost
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/json.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/fiber/condition_variable.hpp>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatter.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

//pqxxlib
#include <pqxx/pqxx>

// boost::asio
namespace io = boost::asio;
using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;
using namespace boost::placeholders;


// boost::log
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;