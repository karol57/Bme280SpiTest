#include "Bme280Spi.hpp"
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>

namespace {
void ensureValidSpiSpeed(SpiDevice& spi)
{
    constexpr uint32_t Mhz20 = 20'000'000;
    if (const uint32_t spiSpeed = spi.getMaxSpeedHz(); spiSpeed > Mhz20)
    {
        spi.setMaxSpeedHz(Mhz20);
        std::cout << "Reduced SPI speed from " << spiSpeed << " Hz to " << Mhz20 << " Hz" << std::endl;
    }
}

void delayUs(uint32_t period, void */*intf_ptr*/)
{
    std::this_thread::sleep_for(std::chrono::microseconds(period));
}

int8_t spiRead(uint8_t reg_addr, uint8_t * const reg_data, uint32_t len, void * const intf_ptr)
{
    if (not reg_data or not intf_ptr)
        return BME280_E_NULL_PTR;

    SpiDevice& bme280 = *reinterpret_cast<SpiDevice*>(intf_ptr);
    spi_ioc_transfer xfer[2] = {};
    xfer[0].tx_buf = reinterpret_cast<uintptr_t>(&reg_addr);
    xfer[0].len = 1;
    xfer[1].rx_buf = reinterpret_cast<uintptr_t>(reg_data);
    xfer[1].len = len;
    bme280.transfer(xfer);
    return BME280_OK;
}

int8_t spiWrite(uint8_t reg_addr, const uint8_t * const reg_data, uint32_t len, void * const intf_ptr)
{
    if (not reg_data or not intf_ptr)
        return BME280_E_NULL_PTR;

    SpiDevice& bme280 = *reinterpret_cast<SpiDevice*>(intf_ptr);
    spi_ioc_transfer xfer[2] = {};
    xfer[0].tx_buf = reinterpret_cast<uintptr_t>(&reg_addr);
    xfer[0].len = 1;
    xfer[1].tx_buf = reinterpret_cast<uintptr_t>(reg_data);
    xfer[1].len = len;
    bme280.transfer(xfer);
    return BME280_OK;
}

[[gnu::cold, noreturn]]
static void throwBME280Error(int8_t error)
{
    switch (error)
    {
        case BME280_E_NULL_PTR: throw std::runtime_error("BME280: Null pointer");
        case BME280_E_DEV_NOT_FOUND: throw std::runtime_error("BME280: Device not found");
        case BME280_E_INVALID_LEN: throw std::runtime_error("BME280: Invalid length");
        case BME280_E_COMM_FAIL: throw std::runtime_error("BME280: Communication failure");
        case BME280_E_SLEEP_MODE_FAIL: throw std::runtime_error("BME280: Sleep mode failure");
        case BME280_E_NVM_COPY_FAILED: throw std::runtime_error("BME280: NVM copy failed");
        default:
            throw std::runtime_error("BME280: Unknown error " + std::to_string(error));
    }
}

static void verifyBme280Result(int8_t result)
{
    if (result < 0)
        throwBME280Error(result);

    switch (result)
    {
        case BME280_OK: return;
        case BME280_W_INVALID_OSR_MACRO:
            std::cerr << "BME280: Invalid osr macro";
            break;
        default:
            std::cerr << "BME280: Unknown warning " << result;
    }
}
}

Bme280Spi::Bme280Spi(const char* path)
    : m_spi(path)
{
    ensureValidSpiSpeed(m_spi);
    m_dev.intf_ptr = &m_spi;
    m_dev.intf = BME280_SPI_INTF;
    m_dev.read = spiRead;
    m_dev.write = spiWrite;
    m_dev.delay_us = delayUs;
    verifyBme280Result(bme280_init(&m_dev));

    // Temporary default initialization
    m_dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    m_dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    m_dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    m_dev.settings.filter = BME280_FILTER_COEFF_16;
    uint8_t settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
    verifyBme280Result(bme280_set_sensor_settings(settings_sel, &m_dev));
    verifyBme280Result(bme280_set_sensor_mode(BME280_NORMAL_MODE, &m_dev));
    std::this_thread::sleep_for(std::chrono::milliseconds(bme280_cal_meas_delay(&m_dev.settings)));
}

bme280_data Bme280Spi::getData() const
{
    bme280_data result;
    bme280_dev dev = m_dev;
    verifyBme280Result(bme280_get_sensor_data(BME280_ALL, &result, &dev));
    if (memcmp(&m_dev, &dev, sizeof(bme280_dev) != 0))
        throw std::runtime_error("Bme280Spi: Unsupported operation in BME280 API");
    return result;
}