cmake_minimum_required(VERSION 3.5)

project(args)

add_library(args INTERFACE)

target_include_directories(args INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                          $<INSTALL_INTERFACE:include/args>)

install(TARGETS args EXPORT argsConfig
        LIBRARY DESTINATION lib/args)

install(EXPORT argsConfig DESTINATION lib/args/cmake)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/args.h" DESTINATION include/args)

add_executable(argsTester "main.cpp")
