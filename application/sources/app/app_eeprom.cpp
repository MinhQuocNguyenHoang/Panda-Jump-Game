#include "app_eeprom.h"
#include "eeprom.h"

typedef struct {
	uint32_t magic_number;
	game_settings_t data;
	uint8_t check_sum;
} game_setting_eeprom_t;

typedef struct {
	uint32_t magic_number;
	game_score_t data;
	uint8_t check_sum;
} game_score_eeprom_t;

#define PANDA_GAME_SETTING_CHECKSUM_SIZE	(sizeof(uint32_t) + sizeof(game_settings_t))
#define PANDA_GAME_SCORE_CHECKSUM_SIZE		(sizeof(uint32_t) + sizeof(game_score_t))

static uint8_t panda_game_eeprom_checksum(uint8_t* data, uint32_t size) {
	uint8_t check_sum = 0;

	for (uint32_t i = 0; i < size; i++) {
		check_sum += data[i];
	}

	return check_sum;
}

static void panda_game_eeprom_update_checksum(uint32_t* magic_number, uint8_t* check_sum, uint32_t check_sum_size) {
	*magic_number = PANDA_GAME_EEPROM_MAGIC_NUMBER;
	*check_sum    = panda_game_eeprom_checksum((uint8_t*)magic_number, check_sum_size);
}

static bool panda_game_eeprom_is_valid(uint32_t* magic_number, uint8_t check_sum, uint32_t check_sum_size) {
	return (*magic_number == PANDA_GAME_EEPROM_MAGIC_NUMBER) &&
		   (check_sum == panda_game_eeprom_checksum((uint8_t*)magic_number, check_sum_size));
}

/*****************************************************************************/
/* Setting                                                                    */
/*****************************************************************************/
static void panda_game_setting_set_default(game_settings_t* data) {
	data->difficulty = 1; /* Default is MEDIUM (1) */
	data->sound_en   = 0; /* Default is OFF (0) */
	data->time_limit = 1; /* Default is 60s (1) */
}

bool panda_game_setting_read(game_settings_t* data) {
	game_setting_eeprom_t eeprom_data;

	uint8_t ret = eeprom_read(EEPROM_SETTING_START_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data));

	if (ret == EEPROM_DRIVER_OK &&
		panda_game_eeprom_is_valid(&eeprom_data.magic_number, eeprom_data.check_sum, PANDA_GAME_SETTING_CHECKSUM_SIZE)) {
		*data = eeprom_data.data;
		return true;
	}

	panda_game_setting_set_default(data);
	return false;
}

bool panda_game_setting_write(game_settings_t* data) {
	game_setting_eeprom_t eeprom_data;

	eeprom_data.data = *data;
	panda_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.check_sum, PANDA_GAME_SETTING_CHECKSUM_SIZE);

	return eeprom_write(EEPROM_SETTING_START_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}

/*****************************************************************************/
/* Score                                                                      */
/*****************************************************************************/
static void panda_game_score_set_default(game_score_t* data) {
	data->score_1st = 0;
	data->score_2nd = 0;
	data->score_3rd = 0;
}

bool panda_game_score_read(game_score_t* data) {
	game_score_eeprom_t eeprom_data;

	uint8_t ret = eeprom_read(EEPROM_SCORE_START_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data));

	if (ret == EEPROM_DRIVER_OK &&
		panda_game_eeprom_is_valid(&eeprom_data.magic_number, eeprom_data.check_sum, PANDA_GAME_SCORE_CHECKSUM_SIZE)) {
		*data = eeprom_data.data;
		return true;
	}

	panda_game_score_set_default(data);
	return false;
}

bool panda_game_score_write(game_score_t* data) {
	game_score_eeprom_t eeprom_data;

	eeprom_data.data = *data;
	panda_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.check_sum, PANDA_GAME_SCORE_CHECKSUM_SIZE);

	return eeprom_write(EEPROM_SCORE_START_ADDR, (uint8_t*)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}
