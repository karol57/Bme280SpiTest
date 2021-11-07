#include "Bme280Spi.hpp"
#include <thread>
#include <iostream>

static const char* getDevicePath(int argc, char *argv[])
{
    if (argc < 2)
    {
        return "/dev/spidev0.0";
    }
    else if (argc == 2)
    {
        return argv[1];
    }
    throw std::runtime_error("Too many arguments.\nUsage: Bme280Spi [spi_dev_path]");
}

int main(int argc, char *argv[])
try
{
    Bme280Spi bme280spi(getDevicePath(argc, argv));
    for (;;)
    {
        const bme280_data sensorData = bme280spi.getData();
        printf("\r%0.2f°C %0.2f%% %0.2f hPa", sensorData.temperature / 100.0,
                                              sensorData.humidity / 1024.0,
                                              sensorData.pressure / 10000.0);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}