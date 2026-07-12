<h1 align="center">EEPROM Data Storage</h1>

This document explains how persistent data (Leaderboard high scores and Game Settings) is stored in the EEPROM of the STM32L151.

## I. Overview

Because RAM is cleared every time the kit loses power, Panda Jump uses the on-chip EEPROM to save user data so that it persists across resets. 

The game stores two main types of data:
1. **Scores (`game_score_t`)**: The top 3 highest scores.
2. **Settings (`game_settings_t`)**: User preferences (Difficulty, Sound, Time Limit).

To ensure that the game does not load corrupted data (for example, if the kit loses power while writing, or if the EEPROM is completely blank on a new chip), we use a **Magic Number** and a **Checksum** for validation.

## II. Data Structures

The data structures are defined in `application/sources/app/app_eeprom.h`:

```c
#define PANDA_GAME_EEPROM_MAGIC_NUMBER ((uint32_t)0x504A554D) // "PJUM"

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
```

When written to the EEPROM, these structures are wrapped in a container that includes the magic number and the checksum byte:

```c
typedef struct {
    uint32_t magic_number;
    game_score_t data;
    uint8_t check_sum;
} game_score_eeprom_t;
```

## III. Protection Mechanism (Magic Number & Checksum)

### 1. Magic Number
A magic number is a unique constant identifier. For Panda Jump, it is `0x504A554D` (which is the ASCII representation of **"PJUM"**).
When reading from the EEPROM, the game first checks if the first 4 bytes match `0x504A554D`. If they don't, the game immediately knows the EEPROM is blank or contains data from a different project, and safely resets the scores/settings to zero/default.

### 2. Checksum
The checksum is an 8-bit additive hash of the magic number and the data payload.
When writing data, the game calculates the sum of all bytes in the data and saves it to `check_sum`:
```c
static uint8_t panda_game_eeprom_checksum(uint8_t* data, uint32_t size) {
    uint8_t check_sum = 0;
    for (uint32_t i = 0; i < size; i++) {
        check_sum += data[i];
    }
    return check_sum;
}
```
When reading data, the game recalculates the checksum. If `calculated_checksum != stored_check_sum`, it means the data was corrupted (e.g., power loss during a write cycle), and the game discards the corrupted data.

## IV. Leaderboard Ranking Logic

At the end of a match, when the Game Over or Victory screen triggers, the system calls `ranking(uint32_t score)` in `scr_panda_game.cpp`.

This function:
1. Checks if the new `score` is greater than the current 3rd place score.
2. If it is, it shifts the lower scores down to make room and inserts the new score into the correct 1st, 2nd, or 3rd position.
3. Finally, it creates a new `game_score_t` object and calls `panda_game_score_write()` to safely pack it with a new checksum and commit it to the EEPROM.

```c
if (score > top_scores[2])
{
    if (score > top_scores[0]) {
        top_scores[2] = top_scores[1];
        top_scores[1] = top_scores[0];
        top_scores[0] = score;
    } else if (score > top_scores[1]) {
        top_scores[2] = top_scores[1];
        top_scores[1] = score;
    } else {
        top_scores[2] = score;
    }

    game_score_t score_data;
    score_data.score_1st = top_scores[0];
    score_data.score_2nd = top_scores[1];
    score_data.score_3rd = top_scores[2];
    panda_game_score_write(&score_data);
}
```
