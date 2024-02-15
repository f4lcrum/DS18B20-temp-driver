#ifndef DS18B20_LIBRARY_H
#define DS18B20_LIBRARY_H

#include <Arduino.h>

#define SKIP_ROM 0xCC
#define READ_SCRATCHPAD 0xBE
#define TEMP_CONV 0x44

class Temp_sensor_ds18b20 {
  private:
    int m_pin;

  public:

    Temp_sensor_ds18b20(int pin);

    int read();

    void write(byte data);

    bool init();

    double get_temp();

};

#endif
