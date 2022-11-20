cmake_minimum_required(VERSION 3.10)
project(calcWeight LANGUAGES CXX)

find_package(pybind11 REQUIRED)  # or add_subdirectory(pybind11)

pybind11_add_module(calcWeight calculatorWeight.cpp)