cmake-minimum-required(VERSION 3.10)
project(StorageEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(storage
    src/main.cpp
    src/storage.cpp
)