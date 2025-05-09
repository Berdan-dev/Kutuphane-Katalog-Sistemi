cmake_minimum_required(VERSION 3.10)

project(KutuphaneKatalog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(katalog
    src/main.cpp
    src/Catalog.cpp
)

target_include_directories(katalog PRIVATE include)
