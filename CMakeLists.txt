cmake_minimum_required(VERSION 3.5)

project(ServerVint)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

# ${PROJECT_NAME}

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

file(GLOB
        CPP_SOURCES "src/Source/*.cpp")

# Указываем путь к заголовочным файлам
include_directories(src/Header/)

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
