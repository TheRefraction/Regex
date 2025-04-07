cmake_minimum_required(VERSION 3.30)
project(Assignment1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Assignment1 main.c
        node.c
        node.h
        automaton.c
        automaton.h
        match.c
        match.h)
