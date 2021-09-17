cmake_minimum_required(VERSION 3.0.0)
project(BigNumber VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)
aux_source_directory(src SRC)

add_executable(BigNumber main.cpp ${SRC})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
