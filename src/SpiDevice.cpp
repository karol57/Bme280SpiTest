#include "SpiDevice.hpp"
#include <sysapi/fcntl.hpp>
#include <sysapi/unistd.hpp>
#include <linux/spi/spidev.h>
#include <stdexcept>
#include <iostream>

SpiDevice::SpiDevice(const char* path)
    : m_fd{ sysapi::open(path, O_RDWR) } {}

SpiDevice::~SpiDevice()
try
{
    sysapi::close(m_fd);
}
catch(const std::exception& e)
{
    std::cerr << "SpiDevice::~SpiDevice: " << e.what() << std::endl;
    return;
}

uint32_t SpiDevice::getMaxSpeedHz() const
{
    uint32_t result;
    sysapi::ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &result);
    return result;
}

void SpiDevice::setMaxSpeedHz(uint32_t hz) const
{
    sysapi::ioctl(m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &hz);
}