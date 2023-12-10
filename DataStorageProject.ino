/*
 Name:		DataStorageProject.ino
 Author:	DarthSidious
*/

#include "DeviceSettings.hpp"
#include "DataStorage.hpp"

DataStorage ds;  /// Create object of DataStorage class

led_channel_t led_1 = {
  .sunrise = { .hour = 10, .minute = 8, .seconds = 5 },
  .sunset = { .hour = 9, .minute = 4, .seconds = 2 },
  .day_power = 255,
  .night_power = 5,
};  /// Base led 1 channel configuration

led_channel_t led_2 = {
  .sunrise = { .hour = 5, .minute = 18, .seconds = 5 },
  .sunset = { .hour = 12, .minute = 14, .seconds = 21 },
  .day_power = 133,
  .night_power = 66,
};  /// Base led 2 channel configuration

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.println("Hello YouTube");

  ds.init();  /// Initialize DataStorage

  /// Get reboot counter from EEPROM
  uint32_t reboot_counter = 0;
  int result = ds.get(DS_REBOOT_COUNTER, (uint8_t*)&reboot_counter, sizeof(reboot_counter));
  Serial.println("Get DS_REBOOT_COUNTER result is " + String(result));
  Serial.println("reboot_counter is " + String(reboot_counter));

  /// Update reboot counter
  reboot_counter++;

  /// Set updated reboot counter in EEPROM
  result = ds.set(DS_REBOOT_COUNTER, (uint8_t*)&reboot_counter, sizeof(reboot_counter));
  Serial.println("Set DS_REBOOT_COUNTER result is " + String(result));

  /// Print and save in EEPROM base DS_LED_CFG_1 channel configuration
  print_led_cfg(&led_1);
  result = ds.set(DS_LED_CFG_1, (uint8_t*)&led_1, sizeof(led_1));
  Serial.println("Set DS_LED_CFG_1 result is " + String(result));

  /// Read DS_LED_CFG_2 from EEPROM
  led_channel_t led_read;
  result = ds.get(DS_LED_CFG_2, (uint8_t*)&led_read, sizeof(led_read));
  Serial.println("Get DS_LED_CFG_2 result is " + String(result));

  /// Save DS_LED_CFG_2 in EEPROM
  result = ds.set(DS_LED_CFG_2, (uint8_t*)&led_2, sizeof(led_1));
  Serial.println("Set DS_LED_CFG_2 result is " + String(result));

  /// Read DS_LED_CFG_2 from EEPROM
  result = ds.get(DS_LED_CFG_2, (uint8_t*)&led_read, sizeof(led_read));
  Serial.println("Get DS_LED_CFG_2 result is " + String(result));

  print_led_cfg(&led_read);

  int compare_result = memcmp(&led_read, &led_2, sizeof(led_channel_t));
  Serial.println(compare_result == 0 ? "DS_LED_CFG_2 wirte and read are the same" : "ERROR: Read and written DS_LED_CFG_2 are different");
}

// the loop function runs over and over again until power down or reset
void loop() {
}
