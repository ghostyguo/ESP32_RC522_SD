# ESP32_RC522_SD
This demo is to show how to access the Data Logging shield (SD card) with RC522 RFID reader on the ESP32. The demo code, ESP32_RC522_SD.ino, uses the following examples:
  - SD\examples\ReadWrite
  - MFRC522\examples\ReadNUID
## disable ESP32 SD library 
Edit the following line in the file: C:\Users\user\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.4\libraries\SD\library.properties:
```sh
architectures=esp88
```
where 'esp88' can be any string except original 'esp32'.

## Arduino SD Library Settings
Add lines in the file: C:\Program Files (x86)\Arduino\libraries\SD\src\utility\Sd2PinMap.h

```sh
...
#endif	// Arduino ARC
#elif defined (ARDUINO_ARCH_ESP32)
  #include <Arduino.h>
  // Two Wire (aka I2C) ports
  //uint8_t const SDA_PIN = 21;
  //uint8_t const SCL_PIN = 22;

  // SPI port
  //uint8_t const SS_PIN = 5;
  uint8_t const MOSI_PIN = 23;
  uint8_t const MISO_PIN = 19;
  uint8_t const SCK_PIN = 18;
#else
...
```

# Demo Video
Youtube: https://www.youtube.com/watch?v=JWypqkTJkaU
