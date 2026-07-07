# Game Object Sequences

This document outlines the lifecycles, states, and coordinates updates of the gameplay entities in Panda Jump.

## I. Overview

Entities are updated asynchronously on separate task IDs. Interactions are resolved by the display task controller acting as an orchestrator.

---

## II. Panda Lifecycle (`PJ_PANDA_GAME_ID`)

The Panda starts at the center trunk.
*   **UP / DOWN Signals:** Directly modify `panda.y`. Coordinates are clamped within `[0, AXIS_Y_PANDA_MAX]` (48 pixels) to keep the Panda visible.
*   **JUMP LEFT / RIGHT Signals:** Change `panda.lane` (0, 1, or 2). A jump changes the lane and flips the side offset (`panda.side` left/right).

---

## III. Bug Spawn & Update Cycle (`PJ_BUG_GAME_ID`)

Bugs are managed as a fixed array `bugs[MAX_BUGS]`.
1.  **Spawn Conditions:** When the active count is below the difficulty threshold, a pseudo-random generator determines the direction (upward/downward crawl), bamboo lane, and side.
2.  **Explosion Sequence:** When a stomp is detected by the Collision Engine:
    *   Set `bugs[i].explosion_ticks = 4`.
    *   Render `boom` bitmap over the bug coordinates.
    *   After 4 frames, set `bugs[i].active = false` (returns bug to the inactive pool).

---

## IV. Arrow Update Cycle (`PJ_ARROW_GAME_ID`)

Arrows are spawned on the left boundary (`x = 0`) at a random vertical coordinate `y` and fly horizontally.
*   **Movement:** `arrows[i].x += arrows[i].speed`.
*   **Destruction:** If `arrows[i].x > 102` (reaching the game layout boundary line), the arrow is deactivated (`active = false`).
