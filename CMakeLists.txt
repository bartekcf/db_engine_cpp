cmake_minimum_required(VERSION 3.27)
project(baza_danych)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

add_executable(baza_danych main.cpp
        include/QueryParser.h
        source/QueryParser.cpp
        include/Table.h
        include/Database.h
        source/Database.cpp
        include/Table.h
        source/Table.cpp)
target_link_libraries(baza_danych fmt)
