cmake_minimum_required(VERSION "3.16")
project(ServeurWeb VERSION "1.0.0" LANGUAGES CXX)
add_executable(Server
    "src/main.cpp"
    "src/Server/Server.cpp"
    "include/Server/Server.hpp"
)
target_include_directories(Server PRIVATE "include")
