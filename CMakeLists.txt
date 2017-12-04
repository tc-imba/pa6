cmake_minimum_required(VERSION 3.9)
project(pa6_RoblesLi)

set(CMAKE_CXX_STANDARD 11)

add_executable(pa6_RoblesLi
        cutest/CuTest.c
        cutest/CuTest.h
        alloc.c
        alloc.h
        runtest.c
        test.c)
