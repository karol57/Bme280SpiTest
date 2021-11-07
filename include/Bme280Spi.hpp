#pragma once
#include "SpiDevice.hpp"
#include "bme280.h"

class Bme280Spi
{
public:
    Bme280Spi(const char* path);

    bme280_data getData();
private:
    SpiDevice m_spi;
    bme280_dev m_dev;
};