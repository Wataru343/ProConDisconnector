cmake_minimum_required(VERSION 3.5)
project(ProConDisconnector VERSION 0.1.0)
project(ProConDisconnector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/wd4819)
    add_compile_options(/source-charset:utf-8)
    add_compile_options("$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-DDEBUG>")
endif()

add_subdirectory(src)
