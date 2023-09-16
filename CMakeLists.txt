cmake_minimum_required(VERSION 3.0.0)
cmake_policy(SET CMP0054 NEW)

project(client VERSION 0.1.0 LANGUAGES C CXX)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


set(BOOST_LIB_DIR "C:/boost/boost_1_82_0/stage/lib")
set(BOOST_DIR "C:/boost/boost_1_82_0")
set(SERVER_INCLUDE_DIR "./Server/inc")

set(PQXX_DIR "C:/Program Files (x86)/libpqxx/include")
set(PQXX_LIB_DIR "C:/Program Files (x86)/libpqxx/lib")

set(LIBPQ_DIR "C:/Program Files (x86)/PostgreSQL/10/include")
set(LIBPQ_LIB_DIR "C:/Program Files (x86)/PostgreSQL/10/lib")


include_directories(${BOOST_DIR} ${SERVER_INCLUDE_DIR} ${PQXX_DIR} ${LIBPQ_DIR})
link_directories(${BOOST_LIB_DIR}  )

add_executable(client 
    Client/client_entry_point.cpp 
    Client/Client.cpp   Client/Client.hpp)

add_executable(server
    Server/main.cpp         Server/inc/includes.hpp
    Server/src/Server.cpp   Server/inc/Server.hpp
    Server/src/Session.cpp  Server/inc/Session.hpp
    Server/src/DBConnection.cpp  Server/inc/DBConnection.hpp
    Server/src/DBSession.cpp  Server/inc/DBSession.hpp
    Server/inc/Logger.hpp
)

target_link_libraries(server pqxx libpq wsock32 ws2_32)
list(APPEND TARGETS client server)

foreach(target ${TARGETS})
    set (EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/${}/bin)
    target_compile_features(${target} PUBLIC cxx_std_20)
    set_target_properties(client PROPERTIES 
        CXX_STANDARD 20)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} 
            PRIVATE -g -Wall -Wextra -Werror -pedantic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} 
            PRIVATE /W4 /WX)
    endif()
    if (WIN32)
        target_compile_options(${target} 
            PRIVATE -D_WIN32_WINNT=0x0601)
    endif()

endforeach()
    
    