#  Bme280SpiTest

A simple application to test if BME280 sensor is correctly connected on SPI interface

## Usage

Build it and run. The program will configure the sensor on SPI0.0 interface and will read the measurements continuosly.
```
> Bme280SpiTest
24.98°C 40.63% 998.42 hPa
```

You can pass path to SPI interface as an argument:
```
> Bme280SpiTest /dev/spidev0.1
24.98°C 40.63% 998.42 hPa
```

Press <kbd>Ctrl</kbd> + <kbd>C</kbd> to stop the program

## License
[MIT](https://choosealicense.com/licenses/mit/)