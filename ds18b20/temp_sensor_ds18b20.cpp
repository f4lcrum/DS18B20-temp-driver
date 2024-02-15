#include "temp_sensor_ds18b20.h"
#include <Arduino.h>

Temp_sensor_ds18b20::Temp_sensor_ds18b20(int pin) {
    m_pin = pin;
}

int Temp_sensor_ds18b20::read() {
    int data = 0;
    for (int i = 0; i < 8; i++)
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH); // read time slot init
        delayMicroseconds(2);
        digitalWrite(m_pin, LOW); // after this command, read time slot is initiated
        delayMicroseconds(1);


        digitalWrite(m_pin, HIGH); // p
        pinMode(m_pin, INPUT);
        delayMicroseconds(5);
        data >>= 1;
        if (digitalRead(m_pin)) data |= 0x80;
        delayMicroseconds(55);
    }
    return data;
}

void Temp_sensor_ds18b20::write(byte data) {
    pinMode(m_pin, OUTPUT);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(m_pin, LOW);
        delayMicroseconds(1);
        data & 1 ? digitalWrite(m_pin, HIGH) : digitalWrite(m_pin, LOW);
        delayMicroseconds(50);
        digitalWrite(m_pin, HIGH);
        data >>= 1;
    }
}

bool Temp_sensor_ds18b20::init() {
    pinMode(m_pin, OUTPUT);

    digitalWrite(m_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(m_pin, LOW);
    delayMicroseconds(960); // Reset pulse maximum, see figure 11
    digitalWrite(m_pin, HIGH);
    pinMode(m_pin, INPUT);
    int tick = 0;
    while (digitalRead(m_pin)) // waiting for resistor pull up, max 60 microseconds
    {
        tick++;
        if (tick > 60) return false;
        delayMicroseconds(1);
    }
    pinMode(m_pin, OUTPUT);
    delayMicroseconds(240); // max presence pulse
    digitalWrite(m_pin, HIGH);
    return true;
}

double Temp_sensor_ds18b20::get_temp() {
    if (!init()) return 0;
    write(SKIP_ROM);
    write(TEMP_CONV);
    if (!init()) return 0;
    write(SKIP_ROM);
    write(READ_SCRATCHPAD);
    int temp = read();
    temp |= read() << 8;
    return temp * 0.0625;
}


int get_nth_bit(char data, int n) {
   return ((data>>n)&0x01);
}




