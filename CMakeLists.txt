cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app)

target_sources(app PRIVATE src/main.c)

set(COMPONENTS 
    utils
    twi
    mpu9250
    lis2dh12
)

foreach(COMPONENT ${COMPONENTS})
  add_subdirectory(components/${COMPONENT})
endforeach()