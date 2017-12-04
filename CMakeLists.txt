cmake_minimum_required(VERSION 3.9)
project(pa6_RoblesLi)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-m32")

add_executable(pa6_RoblesLi
        cutest/CuTest.c
        cutest/CuTest.h
        alloc.c
        alloc.h
        runtest.c
        test.c)

add_executable(main main.c alloc.c alloc.h)
