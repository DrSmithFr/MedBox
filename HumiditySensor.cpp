#include "HumiditySensor.h"
#include "DHT.h"

HumiditySensor::HumiditySensor(gpio_num_t power_pin, gpio_num_t data_pin): _dht(data_pin, DHT11) {
  _power_pin = power_pin;
  _data_pin = data_pin;
}

void HumiditySensor::init() {
  pinMode(_power_pin, OUTPUT);
  pinMode(_data_pin, INPUT);

  // disable humidity sensor on startup
  this->disable();
}

void HumiditySensor::enable() {
  digitalWrite(_power_pin, HIGH);
  _dht.begin();

  // sensor warmup
  delay(5000);
}

void HumiditySensor::disable() {
  digitalWrite(_power_pin, LOW);
}

void HumiditySensor::print() {
  if (digitalRead(_power_pin) == LOW) {
    Serial.println("Humidity sensor is disabled");
    return;
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = _dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = _dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = _dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));
}
