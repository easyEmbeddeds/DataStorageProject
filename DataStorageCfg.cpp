#include "DataStorage.hpp"
#include "DeviceSettings.hpp"

ds_data_info_t data_storage_config[] = {
  { DS_REBOOT_COUNTER, 0, sizeof(uint32_t) },
  { DS_LED_CFG_1, 0, sizeof(led_channel_t) },
  { DS_LED_CFG_2, 0, sizeof(led_channel_t) },
  { DS_TOO_BIG, 0, 999 },
};  /// Array with DataStorage data configuration
