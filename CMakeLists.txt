cmake_minimum_required(VERSION 3.10)

project(ProcessCheckerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ProcessCheckerApp
    main.cpp
    ProcessChecker/ProcessChecker.cpp
    ProcessExecutor/ProcessExecutor.cpp
    communication/ISubject.cpp
)
