# Runtime Signal Processing

This document describes the runtime signal processing, button inputs, task messaging, and the event-driven game loop logic for Panda Jump.

## I. Overview

Panda Jump runs on the ActiveKit (AK) Framework, where tasks are scheduled asynchronously using a message-passing queue. Tasks do not poll hardware states in blocking loops. Instead, hardware IRQs or timer timeouts trigger signals, which are packed into messages and posted to the appropriate task handler.

---

## II. Task Handlers and Signals

The game leverages four primary tasks defined in `task_list.cpp`:

1.  **Display Task (`AC_TASK_DISPLAY_ID`):** Manages screen states (FSM), menu navigations, gameplay rendering, and collision updates.
2.  **Panda Task (`PJ_PANDA_GAME_ID`):** Handles the movement coordinates and status of the Panda character.
3.  **Bug Task (`PJ_BUG_GAME_ID`):** Manages spawning rates, movement paths, and squash states for Crawling Bugs.
4.  **Arrow Task (`PJ_ARROW_GAME_ID`):** Computes movement of horizontal flying arrows.

---

## III. Game Loop & Frame Ticks

When entering the gameplay screen, a periodic timer is configured to send tick events:

```cpp
timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_PANDA_GAME_UPDATE, 100, TIMER_PERIODIC);
```

On every `AC_DISPLAY_PANDA_GAME_UPDATE` tick (100ms interval):
1.  Post update request to `PJ_BUG_GAME_ID` (`PJ_BUG_GAME_UPDATE`).
2.  Post update request to `PJ_ARROW_GAME_ID` (`PJ_ARROW_GAME_UPDATE`).
3.  Evaluate game-ending time limits.
4.  Perform collision checking between the Panda, Bugs, and Arrows.
5.  Request the display engine to refresh screen buffer coordinates.

---

## IV. Signal Sequence Flow

A simplified flow diagram of button presses transitioning into FSM signal updates:

```text
[Button Press IRQ]
       |
       v (Post msg to Display Task)
[AC_DISPLAY_BUTON_UP_PRESSED] / [AC_DISPLAY_BUTON_DOWN_PRESSED]
       |
       v (Display Task forwards state-specific command)
[PJ_PANDA_JUMP_RIGHT] / [PJ_PANDA_JUMP_LEFT]
       |
       v (Panda Task updates coordinates)
[AC_DISPLAY_PANDA_GAME_UPDATE]
       |
       v (Display Task renders new frame)
[OLED Screen Buffer Update]
```
