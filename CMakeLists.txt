project(bcm_control)

cmake_minimum_required(VERSION 3.5)
find_library(wiringPi_LIB wiringPi)
set(source src/main.cpp)
add_executable(bcm_control ${source})
target_link_libraries(bcm_control ${wiringPi_LIB})
