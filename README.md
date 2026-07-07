<div align="center">

<!-- Dãy Badges tối giản -->
<img src="https://img.shields.io/badge/language-C%2B%2B-brightgreen?style=flat-square&logo=c%2B%2B" alt="C++"/>
<img src="https://img.shields.io/badge/mcu-STM32L151-green?style=flat-square" alt="STM32L151"/>
<img src="https://img.shields.io/badge/framework-Active_Kernel-green?style=flat-square" alt="AK"/>
<img src="https://img.shields.io/badge/hardware-AK_Base_Kit-green?style=flat-square" alt="AK Kit"/>

</div>

# Panda Jump — Game built on AK Embedded Base Kit

<center>
<img width="1200" height="600" alt="PANDA JUMP BANNER" src="resources/images/screens/PANDA-JUMP.gif" />
</center>

<hr>

## Gameplay Demo

<div align="center">
  <video src="https://github.com/user-attachments/assets/5a945645-c364-4636-951e-c80039b7927a" controls width="480"></video>
</div>

## Documentation

| File | Description |
|---|---|
| [README.md](README.md) | Project overview, hardware specs, game mechanics, and object descriptions. |

## Introduction

Panda Jump is a **vertical time-attack climbing game** built on the **AK Embedded Base Kit**, powered by the **Active Kernel (AK)** event-driven framework. The player controls a panda gripping one of three bamboo trunks, dodging vertically-crawling bugs and horizontally-flying arrows — and must **survive long enough for the countdown to reach zero** to win.

What makes Panda Jump stand out in the AK game series:

- **A win condition:** Unlike most AK games that end only on death, Panda Jump has a timer-based **victory state** alongside the Game Over state.
- **Dual-axis threat system:** Bugs move vertically along bamboo trunks; arrows cut horizontally across all lanes — requiring the player to make two completely different spatial decisions simultaneously.
- **Direction-aware stomp collision:** At the moment of bounding-box overlap between panda and bug, the game checks `panda.y < bug.y`. If true — the panda is above the bug — it is a stomp (score + explosion). Any other angle is a crash (Game Over).
- **Static memory discipline:** The entire game runs within 16 KB RAM using fixed-size entity pools (`MAX_BUGS = 5`, `MAX_ARROWS = 5`) with zero heap allocation.

### I. Hardware

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/ak-embedded-base-kit-version-3.jpg" alt="AK Embedded Base Kit - STM32L151 - v3.0" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

[AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) is an evaluation kit for advanced embedded software learners.

The kit integrates a **1.54" OLED LCD**, **3 push buttons**, and **a buzzer** capable of playing tonal sequences — everything needed to study event-driven systems through hands-on game design. It also exposes **RS485**, **Qwiic**, and **Grove** connectors for broader embedded prototyping.

**MCU Overview:**

```text
SoC Name : STM32L151CBT6
RAM      : 16 KB

Flash Partitions Layout
----------------------
[ 0x08000000 - 0x08001FFF ] : Bootloader Partition (8 KB)
=> AK Bootloader

[ 0x08002000 - 0x08002FFF ] : BSF Shared Partition (4 KB)
=> Used for data sharing between Bootloader and Application

[ 0x08003000 - 0x0801FFFF ] : Application Partition (116 KB)
=> Panda Jump firmware
```

**MCU Naming Convention:**

| Part | Meaning |
|---|---|
| `STM32` | STMicroelectronics 32-bit MCU family. |
| `L` | Low-power series. |
| `151` | STM32L151 product line. |
| `C` | 48-pin package. |
| `B` | 128 KB Flash memory. |
| `T` | LQFP package. |
| `6` | Industrial temperature grade. |

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/board-view-top-bottom.png" alt="AK Embedded Base Kit - Board view Top + Bottom" width="900"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 2:</em></strong> Board view Top + Bottom</p>

---

### II. Game Description and Objects

When powered on, the kit walks through: **Startup (AK Logo)** → **QR Code** (pointing to this repository) → **Welcome** — where a panda climbs the right bamboo, a bug crawls the left one, and an arrow flies across the screen as a live gameplay preview — then lands on the **Main Menu**.

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/scr_menugame.jpg" alt="Panda Jump menu" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 3:</em></strong> Main Menu screen</p>

The **Main Menu** offers four options:
- **Panda Game:** Start a new time-attack climbing round.
- **Setting:** Configure difficulty, sound toggle, and time limit (30 s / 60 s / 90 s).
- **Charts:** View the Top-3 leaderboard rendered as a vector-drawn podium with a crown for 1st place.
- **Exit:** Return to the Welcome screen.

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/scr_gameplay.jpg" alt="Panda Jump gameplay" width="600"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 4:</em></strong> Gameplay screen</p>

#### Objects in the Game:

| Bitmap | Object Name | Description |
| :---: | :--- | :--- |
| <img src="resources/images/bitmap/panda.gif" width="90"/> | **Panda** | The player character. Grips the left or right face of one of three bamboo trunks (`lane` 0–2, `side` 0–1). Climbs vertically with **[Mode]**; jumps lanes and swaps sides with **[Up]** / **[Down]**. |
| <img src="resources/images/bitmap/bug.gif" width="90"/> | **Bamboo Bug** | A spiked insect that crawls vertically along a bamboo face in both directions. Stomped from above → explosion + score. Touched from the side or below → Game Over. |
| <img src="resources/images/bitmap/arrow.gif" width="90"/> | **Arrow** | A horizontal projectile crossing the full screen width at variable speed. Any contact with the panda → instant Game Over. |
| <img src="resources/images/bitmap/bamboo.jpg" height="90"/> | **Bamboo Trunk** | Three fixed vertical columns defining the lane grid. Both panda and bugs are snapped to the left or right face of each trunk. |
| <img src="resources/images/bitmap/boom.jpg" width="90"/> | **Boom** | A 4-tick explosion sprite drawn at the bug's coordinates after a successful stomp. Visual confirmation only — no hitbox or area effect. |

> **Note:** For detailed object runtime sequences, see [Game Object Sequences](docs/01-design-sequence-object.md).

---

### III. How to Play

- Press **[Up]** to jump right — swaps the panda to the other face of the same trunk, or leaps to the next trunk on the right.
- Press **[Down]** to jump left — swaps face or leaps to the next trunk on the left.
- **Short-press [Mode]** to climb up 4 px; **long-press [Mode]** to slide down 4 px.
- Stomp bugs from above to score. Avoid touching arrows or running into bugs from the side.
- On Victory / Game Over screens: **[Up]** = Retry, **[Mode]** = Return to main menu.

#### Game Mechanics:

- **Scoring:** A successful stomp scores **10 pts** (Easy) / **20 pts** (Medium) / **30 pts** (Hard).
- **Difficulty:** Moving from Easy → Medium → Hard increases max simultaneous bugs (1→2→3), spawn probability per 100 ms tick (6%→12%→20%), and arrow speed range (1→1–2→2–3 px/tick).
- **Victory condition:** Survive until the countdown reaches zero. The kit plays the *Merry Christmas* melody and shows a trophy animation with sparkling stars.
- **Defeat condition:** Any arrow contact, or any non-stomp bug collision, triggers Game Over immediately with a `BANG` sound.
- **Leaderboard:** Top-3 scores and configuration settings are persistently written to and loaded from the EEPROM. High scores and game settings persist across power resets, protected by magic-number validation and checksum verification (see [docs/03-eeprom-data-storage.md](docs/03-eeprom-data-storage.md)).
- **Screen saver:** After 15 s of inactivity on any menu screen, the display transitions to a bouncing-bubble animation to protect the OLED panel from burn-in.

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/scr_victory.jpg" alt="Victory screen" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 5:</em></strong> Victory screen — survive the countdown to win</p>

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/screens/scr_gameover.jpg" alt="Game Over screen" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 6:</em></strong> Game Over screen — final score and survival time displayed</p>

---

### IV. Software Architecture

The game is split into four independent AK Tasks. The diagram below shows the **static structure** — which tasks exist and which signals wire them together:

```mermaid
graph TD
    HW["Button IRQ (10ms poll)"]
    HW -->|"UP / DOWN / MODE signals"| D

    T["100ms Periodic Timer\nAC_DISPLAY_PANDA_GAME_UPDATE"]
    T --> D

    D["AC_TASK_DISPLAY_ID\nScreen FSM · Collision check · OLED render"]

    D -->|"SETUP · JUMP_LEFT/RIGHT · UP/DOWN"| P
    D -->|"SETUP · UPDATE"| B
    D -->|"SETUP · UPDATE"| A

    P["PJ_PANDA_GAME_ID\nlane / side / y · score · survival_ticks"]
    B["PJ_BUG_GAME_ID\nspawn · crawl · explosion_ticks"]
    A["PJ_ARROW_GAME_ID\nspawn · fly · deactivate"]
```

---

### V. Basic Game Sequence Logic

The diagram below shows the **runtime flow** — the time-ordered sequence of messages and actions that occur during a single 100 ms game-loop tick, from the timer firing all the way through to the OLED frame being rendered.

> **Note:** For a more detailed sequence flow, see [Runtime Signal Processing](docs/02-design-sequence-runtime.md).

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/design-sequence/pj_game_sequence_logic.png" alt="Panda Jump basic game sequence logic" width="1000"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 7:</em></strong> Basic game sequence logic</p>

---

### VI. How to Run, Build & Flash

You can choose either of the two methods below to load and run the game on your board:

---

#### Option 1: Flash the Pre-built Binary (Fastest)

If you do not want to install any compiler toolchains, you can directly upload the pre-compiled binary included in the repository.

##### 1. Flash the Binary
Connect the board to your PC via USB, then run the `ak_flash` tool pointing to the pre-built binary:
```sh
ak_flash /dev/ttyUSB0 resources/bin/ak-base-kit-stm32l151-application.bin 0x08003000
```

---

#### Option 2: Build and Compile from Source Code

##### 1. Setup the Toolchain
Make sure you have the `arm-none-eabi-gcc` toolchain installed and added to your system path. You can verify the installation by checking the version:
```sh
arm-none-eabi-gcc --version
```

##### 2. Compile the Firmware
Navigate to the `application/` directory and compile the program:
```sh
cd application
make
```
*This will generate the build folder `build_ak-base-kit-stm32l151-application/` containing the compiled binary output `ak-base-kit-stm32l151-application.bin`.*

##### 3. Upload to the Board
You can flash the binary directly through the kit's USB serial port using the `ak_flash` CLI tool:
```sh
ak_flash /dev/ttyUSB0 build_Panda_Jump-application/Panda_Jump-application.bin 0x08003000
```
Alternatively, if you are using an ST-LINK programmer, you can compile and flash in one command using `make`:
```sh
make flash
```

---

#### Monitor Serial Debug Logs
To open the serial communication interface (`minicom` terminal) at `115200` baudrate to monitor real-time game logs:
```sh
make com dev=/dev/ttyUSB0
```


---

## Contact & Support

<p style="font-size: 20px;"><strong>Nguyen Hoang Minh Quoc</strong> — Firmware Engineer - Embedded system</p>

```Note
Thank you for visiting this repository.
If you have any questions or feedback about the game design or firmware, feel free to reach out directly.
```

**My contact:** <br/>
<a href="https://github.com/MinhQuocNguyenHoang">
  <img src="https://img.shields.io/badge/MinhQuocNguyenHoang-181717?style=flat&logo=github&logoColor=white"/>
</a>
<a href="https://www.linkedin.com/in/minhquoc-hcmus">
  <img src="https://img.shields.io/badge/MinhQuocNguyenHoang-0A66C2?style=flat&logo=linkedin&logoColor=white"/>
</a>
<a href="quoc20053008@email.com">
  <img src="https://img.shields.io/badge/MinhQuocNguyenHoang-EA4335?style=flat&logo=gmail&logoColor=white"/>
</a>
