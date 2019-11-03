cmake_minimum_required(VERSION 3.5)
project(ProConDisconnector VERSION 0.1.0)
project(ProConDisconnector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4819)
    target_compile_options(${PROJECT_NAME} PUBLIC /source-charset:utf-8)
    target_compile_options(${PROJECT_NAME} PUBLIC "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-DDEBUG>")
endif()

add_subdirectory(src)
