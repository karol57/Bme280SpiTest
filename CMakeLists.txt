cmake_minimum_required(VERSION 3.10)

project(Bme280SpiTest)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(sysapi)

add_library(BME280_driver BME280_driver/bme280)
target_include_directories(BME280_driver PUBLIC BME280_driver/)
add_compile_definitions(BME280_64BIT_ENABLE)

add_executable(Bme280SpiTest src/SpiDevice.cpp src/Bme280Spi.cpp src/main.cpp)
target_link_libraries(Bme280SpiTest sysapi BME280_driver)
target_include_directories(Bme280SpiTest PRIVATE include/)