cmake_minimum_required(VERSION 3.5)
project(ProConDisconnector VERSION 0.1.0)
project(ProConDisconnector LANGUAGES CXX)


add_subdirectory(src)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4819)
    target_compile_options(${PROJECT_NAME} PUBLIC /source-charset:utf-8)
    target_compile_options(${PROJECT_NAME} PUBLIC "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-DDEBUG>")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_compile_options(${PROJECT_NAME} PUBLIC "-std=c++17")
