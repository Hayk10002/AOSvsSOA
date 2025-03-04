cmake_minimum_required(VERSION 3.10)

project(AOSvsSOA)

set(CMAKE_CXX_STANDARD 23)
set(LLVM_CXX_STD c++23)
add_executable(main main.cpp)


# Add the main executable as a test
enable_testing()
add_test(NAME MainTest1 COMMAND main 1000000 1000)
add_test(NAME MainTest2 COMMAND main 5000000 200)
add_test(NAME MainTest3 COMMAND main 10000000 100)