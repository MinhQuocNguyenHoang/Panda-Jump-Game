#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

#define EEPROM_START_ADDR								(0X0000)
#define EEPROM_END_ADDR									(0X1000)

#define EEPROM_SCORE_START_ADDR							(0X0010)
#define EEPROM_SETTING_START_ADDR						(0X0100)

#define PANDA_GAME_EEPROM_MAGIC_NUMBER					((uint32_t)0x504A554D) // "PJUM"

/* Struct to store game configuration settings */
typedef struct
{
	uint8_t difficulty; /* 0: EASY, 1: MEDIUM, 2: HARD */
	uint8_t sound_en;   /* 0: OFF, 1: ON */
	uint8_t time_limit; /* 0: 30s, 1: 60s, 2: 90s */
} game_settings_t;

/* Struct to store high scores for the leaderboard */
typedef struct
{
	uint32_t score_1st;
	uint32_t score_2nd;
	uint32_t score_3rd;
} game_score_t;

#ifdef __cplusplus
extern "C" {
#endif

	extern game_settings_t game_settings;
	extern uint32_t top_scores[3];

	extern bool panda_game_setting_read(game_settings_t *data);
	extern bool panda_game_setting_write(game_settings_t *data);

	extern bool panda_game_score_read(game_score_t *data);
	extern bool panda_game_score_write(game_score_t *data);

#ifdef __cplusplus
}
#endif

#endif //__APP_EEPROM_H__
