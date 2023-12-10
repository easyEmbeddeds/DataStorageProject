#pragma once

#include <stdint.h>

typedef enum {
  DS_REBOOT_COUNTER = 0,
  DS_LED_CFG_1,
  DS_LED_CFG_2,
  DS_TOO_BIG,
  DS_LAST,
} ds_data_id_t;

typedef struct {
  const ds_data_id_t id;  /// Data id
  uint16_t addr;          /// Data address in eeprom
  const uint16_t size;    /// Sizeof data in bytes
} ds_data_info_t;

class DataStorage {
private:
  uint16_t size;    /// Eeprom size
  uint32_t marker;  /// marker to mark which data is set or not set in EEPROM memory
  static ds_data_info_t* ds_data_cfg;

  void erase(void);
  int mark_as_set(const ds_data_id_t id);
  int mark_as_free(const ds_data_id_t id);

public:
  void init(void);
  int set(const ds_data_id_t id, const uint8_t* source, const uint16_t source_len);
  int get(const ds_data_id_t id, uint8_t* destination, const uint16_t destination_len);
  int release(const ds_data_id_t id);
  int is_set(const ds_data_id_t id);
};