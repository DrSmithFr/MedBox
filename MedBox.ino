#include <Wire.h>
#include "HumiditySensor.h"

HumiditySensor sensor(GPIO_NUM_5, GPIO_NUM_15);

gpio_num_t BOX_BUTTON = GPIO_NUM_33;

// keep memory during deepsleep
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200); // The baudrate of Serial monitor is set in 9600
  Wire.begin(); // Wire communication begin
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BOX_BUTTON, INPUT);
  
  sensor.init();

  // Waiting for Serial Monitor
  while (!Serial);

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
}

void loop() {
  sensor.enable();
  sensor.print();
  sensor.disable();
  
  waitForBoxClosing();
  
  //Go to sleep now
  //esp_sleep_enable_timer_wakeup(10000000);
  esp_sleep_enable_ext0_wakeup(BOX_BUTTON, LOW);

  gpio_deep_sleep_hold_en();
  esp_deep_sleep_start();
  Serial.println("ERROR: deepsleep no enable on this device!");
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  // Clear Serial monitor output
  Serial.println("");

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("MedBox openned"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("MedBox wake-up by timer"); break;
    default : Serial.println("MedBox fresh start"); break;
  }
}

void waitForBoxClosing() {  
  while(digitalRead(BOX_BUTTON) == LOW) {
    delay(100);
  }

  Serial.println("MedBox closed");
}
