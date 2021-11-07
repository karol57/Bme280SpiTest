#pragma once
#include <sysapi/sys/ioctl.hpp>
#include <linux/spi/spidev.h>
#include <cstdint>

class SpiDevice
{
public:
    SpiDevice(const char* path);
    ~SpiDevice();

    uint32_t getMaxSpeedHz() const;
    void setMaxSpeedHz(uint32_t hz) const;

    template<std::size_t N>
    void transfer(const spi_ioc_transfer (&xfer)[N])
    {
        sysapi::ioctl(m_fd, SPI_IOC_MESSAGE(N), xfer);
    }
private:
    int m_fd;
};