#include "DataStorage.hpp"

#include <Arduino.h>
#include <EEPROM.h>

#include <assert.h>

#define BASE_ADDRESS  (0) ///< Base address od eeprom memory to keep uniqal number of DS version
#define MARKER_ADDRES (1) ///< Address of marker

const uint8_t ds_magic_number = 0x0B;

extern ds_data_info_t data_storage_config[];

ds_data_info_t* DataStorage::ds_data_cfg = data_storage_config;


void DataStorage::erase(void)
{
	Serial.println("Erasing eeprom memory.");

	for (uint16_t i = 0; i < this->size; i++) {
		EEPROM.write(i, 0);
		Serial.print("Progress is ");
		Serial.print(((uint32_t)100 * i) / this->size);
		Serial.println("%");
	}
	Serial.println("Erasing eeprom memory done.");
}

int DataStorage::mark_as_set(const ds_data_id_t id)
{
	if (id >= DS_LAST) {
		return -1;
	}

	uint8_t* marker_ptr = (uint8_t*)&this->marker;

	for (uint8_t i = 0; i < sizeof(this->marker); i++) {
		*(marker_ptr + i) = EEPROM.read(MARKER_ADDRES + i);
	}

	this->marker |= (1 << id);

	for (uint8_t i = 0; i < sizeof(this->marker); i++) {
		EEPROM.write(MARKER_ADDRES + i, *(marker_ptr + i));
	}

	return 0;
}

int DataStorage::mark_as_free(const ds_data_id_t id)
{
	if (id >= DS_LAST) {
		return -1;
	}

	uint8_t* marker_ptr = (uint8_t*)&this->marker;

	for (uint8_t i = 0; i < sizeof(this->marker); i++) {
		*(marker_ptr + i) = EEPROM.read(MARKER_ADDRES + i);
	}

	this->marker &= ~(1 << id);

	for (uint8_t i = 0; i < sizeof(this->marker); i++) {
		EEPROM.write(MARKER_ADDRES + i, *(marker_ptr + i));
	}

	return 0;
}

void DataStorage::init(void)
{
	this->size = EEPROM.length();
	this->marker = 0;

	uint8_t read_magin_number = EEPROM.read(BASE_ADDRESS);

	if (read_magin_number != ds_magic_number) {
		this->erase();
		EEPROM.write(BASE_ADDRESS, ds_magic_number);
	}

	Serial.println("EEPROM size is " + String(this->size));

	uint16_t base_address = sizeof(ds_magic_number) + sizeof(this->marker);

	Serial.println("-----------------------------------");

	for (uint8_t id = 0; id < DS_LAST; id++) {
		if (base_address + this->ds_data_cfg[id].size > this->size) {
			Serial.println("Not enough free space in eeprom memory id is: " + String(id));
			Serial.println("Size this data is: " + String(this->ds_data_cfg[id].size) + " bytes");
			Serial.println("Left space is " + String(this->size - base_address) + " bytes");
			delay(100);
			assert(false);
		}

		this->ds_data_cfg[id].addr = base_address;
		base_address += this->ds_data_cfg[id].size;

		Serial.println(String(id) + "\t" + String(this->ds_data_cfg[id].addr) + "\t" + String(this->ds_data_cfg[id].size));

	}

	Serial.println("-----------------------------------");

	Serial.print("Left space is: ");
	Serial.println(this->size - (this->ds_data_cfg[DS_LAST - 1].addr + this->ds_data_cfg[DS_LAST - 1].size));

	Serial.println("-----------------------------------");

}

int DataStorage::set(const ds_data_id_t id, const uint8_t* source, const uint16_t source_len)
{
	if (id >= DS_LAST || source == NULL || source_len != this->ds_data_cfg[id].size) {
		return -1;
	}

	for (uint16_t i = 0; i < this->ds_data_cfg[id].size; i++) {
		EEPROM.write(this->ds_data_cfg[id].addr + i, source[i]);
	}

	this->mark_as_set(id);

	return 0;
}

int DataStorage::get(const ds_data_id_t id, uint8_t* destination, const uint16_t destination_len)
{
	if (id >= DS_LAST || destination == NULL || destination_len != this->ds_data_cfg[id].size) {
		return -1;
	}

	int result = this->is_set(id);

	if (result) {
		return result;
	}

	for (uint16_t i = 0; i < this->ds_data_cfg[id].size; i++) {
		*(destination + i) = EEPROM.read(this->ds_data_cfg[id].addr + i);
	}

	return 0;
}

int DataStorage::release(const ds_data_id_t id)
{
	if (id >= DS_LAST) {
		return -1;
	}

	return this->is_set(id);
}

int DataStorage::is_set(const ds_data_id_t id)
{
	if (id >= DS_LAST) {
		return -1;
	}

	uint8_t* marker_ptr = (uint8_t*)&this->marker;

	for (uint8_t i = 0; i < sizeof(this->marker); i++) {
		*(marker_ptr + i) = EEPROM.read(MARKER_ADDRES + i);
	}

	if (this->marker & (1 << id)) {
		return 0;
	}

	return -2;
}
