# EEPROM Data Storage

This document details the configuration and future planning of persistent data storage for Panda Jump.

## I. Current Architecture (RAM-Based)

Currently, settings and high scores are managed in system RAM to prioritize low latency and ease of debugging during the initial development cycle:
*   **Game Settings:** Struct `game_settings_t` stores the current selected Difficulty, Sound toggle, and Game Time Limit in RAM.
*   **Leaderboard Scores:** An array `top_scores[3]` is stored inside `scr_chart.cpp`. When a game ends, `ranking(score)` checks if the new score qualifies for the top 3 and updates the array in-place.

---

## II. Planned EEPROM Mapping

For the production-ready version of the software, settings and score details will be persisted to the STM32L151's internal EEPROM or an external I2C EEPROM (integrated into the AK Base Kit).

### 2.1 EEPROM Address Layout

| Memory Range | Payload Struct | Description |
|---|---|---|
| `0x0000 - 0x000F` | `uint32_t magic` + `uint8_t checksum` | EEPROM Header Validation |
| `0x0010 - 0x001F` | `game_settings_t` | Persistent volume settings, difficulty, time limit |
| `0x0020 - 0x003F` | `uint32_t top_scores[3]` | Persistent Top 3 Leaderboard scores |

### 2.2 Record Validation Strategy

To prevent corrupt reads on startup, all EEPROM blocks will be enclosed in a wrapper structure:
```cpp
struct eeprom_record_t {
    uint32_t magic_number;  // E.g., 0x504A554D ('PJUM')
    uint8_t  payload[16];   // Data bytes
    uint8_t  checksum;      // XOR or additive checksum
};
```
On boot, if the `magic_number` or `checksum` fails, the firmware falls back to defaults initialized in `scr_setting.cpp`.
