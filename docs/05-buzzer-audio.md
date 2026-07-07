# Buzzer Audio

This document explains the audio implementation, sound lists, and buzzer control rules for Panda Jump.

## I. Hardware & Audio Driver

*   **Buzzer Type:** Passive Buzzer.
*   **Driver Method:** Timer-driven PWM output.
*   **Control API:** `BUZZER_PlaySound(sound_id)` handles playing non-blocking sound sequences.

---

## II. Soundtrack List & Triggers

The game integrates four main sounds defined in the board support package:

| Sound Constant | Occurrences | Melody Description |
|---|---|---|
| `BUZZER_SOUND_LETS_GO` | Gameplay entry (`SCREEN_ENTRY`) | Upbeat starting melody |
| `BUZZER_SOUND_CLICK` | Menu navigation, settings, bug stomp | Short, sharp click tone |
| `BUZZER_SOUND_BANG` | Game over (Panda collision) | Deep descending crash tone |
| `BUZZER_SOUND_MERRY_CHRISTMAS` | Victory (Survival time limit reached) | Full Christmas melody fanfare |

---

## III. Sound Configuration (Silent Mode)

Sound playback is bound to `game_settings.sound_en` in FSM checks:
```cpp
if (game_settings.sound_en) {
    BUZZER_PlaySound(BUZZER_SOUND_CLICK);
}
```
If the user turns the `SOUND` option to `OFF` in the settings menu, all sound triggers are ignored, allowing quiet gameplay.
