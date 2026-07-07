# Display Design

This document details screen layouts, rendering flows, and UI design principles implemented in Panda Jump.

## I. Display Hardware Specs

*   **Display Panel:** 1.54-inch Monochrome OLED Display.
*   **Resolution:** 128x64 Pixels.
*   **Controller Protocol:** SPI Interface.
*   **Rendering Library:** Adafruit GFX (wrapped in `view_render` API).

---

## II. Screen Layout Layouts

### 2.1 Gameplay Screen Layout
The screen is divided into two areas at coordinate `x = 102`:

```text
0                                      102  104     127
+--------------------------------------+----+---------+  0
|     |              |              |  |    +-----+  |
|     |  [Bamboo 1]  |  [Bamboo 2]  |  |    |Score|  |
|     |              |              |  |    +-----+  | 29
|    (Panda)                        |  |             |
|                    (Bug)          |  |    +-----+  | 33
|                                   |  |    |Time |  |
|   ======> (Arrow)                 |  |    +-----+  |
+--------------------------------------+----+---------+  64
```

*   **Playfield (0 to 101):** Renders the 3 bamboos, leaf drop particles, and active sprites (Panda, Bugs, Arrows, Explosion booms).
*   **Side HUD (104 to 127):** Consists of two boxes:
    *   **Score Box (Y=2):** Centered high score.
    *   **Timer Box (Y=33):** Centered countdown timer in seconds.

### 2.2 Leaderboard Screen Layout
Renders a 3-column podium using round rectangles:
*   **1st Place (Center):** solid white column, decorated with a custom drawn vector crown and stars.
*   **2nd/3rd Place (Sides):** hollow outline columns.

### 2.3 Menu Screen Layout
The menu displays 4 items horizontally using rounded boxes:
*   **Box Dimensions:** Enlarged to 28x28 pixels to increase icon visibility.
*   **Box Layout X Coordinates:** Centered horizontally at offsets `[3, 34, 65, 96]` spanning from `x = 3` to `124`.
*   **Labels:** Centered at `y = 50` underneath the boxes. The navigation help text was removed to maximize vertical spacing.

### 2.4 Settings Screen Layout
*   **Alignment:** Setting items (Difficulty, Sound, Time Limit, Back) are left-aligned starting at `x = 15`.
*   **Selector Pointer:** The `>` character is rendered at `x = 5` for the currently selected item.

---

## III. Screen Transitions
FSM transition signals are triggered using `SCREEN_TRAN(next_handler, &next_screen)`. All screens release their local timers and clean up display queues upon `SCREEN_EXIT` signal.
