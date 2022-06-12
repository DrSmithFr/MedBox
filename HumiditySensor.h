#ifndef MY_HUMIDITY_SENSOR
#define MY_HUMIDITY_SENSOR

#include <Arduino.h>
#include "DHT.h"

class HumiditySensor {
  private:
    gpio_num_t _power_pin;
    gpio_num_t _data_pin;
    DHT _dht;
  public:
    HumiditySensor(gpio_num_t power_pin, gpio_num_t data_pin);
    void init();
    void enable();
    void disable();
    void print();
};

#endif
