<div align="center">
  
![Repo Traffic](https://komarev.com/ghpvc/?username=MinhQuocNguyenHoang&label=Repo+Traffic&color=blue&style=flat-square)

</div>

# Panda Jump - Game built on AK Embedded Base Kit

<center><img width="1000" alt="CODING GAME PANDA JUMP" src="hardware/images/ak-embedded-base-kit-version-3.jpg" />
</center>

<hr>

## Gameplay Demo

<div align="center">
  <!-- Replace the video source below with your actual demo clip once uploaded to GitHub -->
  <video src="https://github.com/user-attachments/assets/1f219060-ba05-4864-b85f-053c3afea595" controls width="480"></video>
</div>

## Documentation

| File | Description |
|---|---|
| [README.md](README.md) | Main project overview, hardware information, gameplay rules, and object descriptions. |
| [docs/01-design-sequence-object.md](docs/01-design-sequence-object.md) | Runtime sequence diagrams and lifecycles for gameplay objects: Panda, Bug, Arrow, and Boom. |
| [docs/02-design-sequence-runtime.md](docs/02-design-sequence-runtime.md) | Runtime signal-processing flow for button inputs, AK task messages, timers, game-loop ticks, and object updates. |
| [docs/03-eeprom-data-storage.md](docs/03-eeprom-data-storage.md) | Configuration settings and high-score ranking persistence strategy (RAM storage layout and proposed EEPROM mappings). |
| [docs/04-display-design.md](docs/04-display-design.md) | Screen layouts (Dual-buffer playing area and HUD split), graphic sprites (OLED bitmap sizes), and FSM screen transitions. |
| [docs/05-buzzer-audio.md](docs/05-buzzer-audio.md) | Audio driver control, buzzer sound list, silent mode config, and playback priorities. |

## Introduction

Panda Jump is an action survival game built on top of the AK Embedded Base Kit — a hands-on platform for embedded programming enthusiasts to explore event-driven design in depth. While building and playing Panda Jump, you put the following core concepts of modern embedded engineering into practice:

- **System design:** Modelling complex logic flows with UML.
- **Process management:** Coordinating cooperative Tasks and scheduling them efficiently.
- **Communication:** Using Signals, Timers, and Messages to react in real time.
- **Control logic:** Building robust state machines for the player, the obstacles, and the overall match progression.

### I. Hardware

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/ak-embedded-base-kit-version-3.jpg" alt="AK Embedded Base Kit - STM32L151 - v3.0" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

[AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) is an evaluation kit aimed at intermediate and advanced embedded software learners.

The kit integrates a **1.54" OLED LCD**, **3 push buttons**, and **a buzzer** capable of playing short melodies, giving you everything you need to study **event-driven systems** through hands-on game-machine design.
It also exposes **RS485**, the **Qwiic Connect System**, and **Grove** connectors, so it doubles as a convenient prototyping board for real-world embedded projects.

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
    <td align="center"><img src="hardware/images/board-view-top.png" width="480"/></td>
    <td align="center"><img src="hardware/images/board-view-bottom.png" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 2:</em></strong> Board view Top + Bottom </p>

### II. Game Description and Objects

The following section describes the gameplay and core mechanics of **"Panda Jump"**. It serves as a reference for ongoing game design and firmware development.

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/ak-embedded-base-kit-version-3.jpg" alt="Game menu" width="480"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 3:</em></strong> Menu screen</p>

The game opens on the **Main Menu**, which offers the following options:

- **Panda Game:** Start a new match.
- **Setting:** Configure gameplay parameters such as starting difficulty, sound, and time limit.
- **Charts:** View the top 3 highest scores.
- **Exit:** Leave the menu and return to the standby screen.

<table align="center">
  <tr>
    <td align="center"><img src="hardware/images/ak-embedded-base-kit-version-3.jpg" alt="Gameplay" width="600"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 4:</em></strong> Gameplay screen</p>

#### Objects in the Game:

| Bitmap | Object Name | Description |
| :---: | :--- |:--- |
| <img src="resources/images/bitmap/bitmap_panda.gif" width="100"/> | **Panda** | The player character. Climbs vertically and jumps horizontally between the 3 bamboo lanes. Can switch side (left/right) on a bamboo. |
| <img src="resources/images/bitmap/bitmap_bug.gif" width="100"/> | **Bug** | Crawls up or down along the bamboo trunks. Squashed when stomped by Panda from above. Each Bug stomped is worth **10 to 30 points** based on difficulty. |
| <img src="resources/images/bitmap/bitmap_arrow.gif" width="100"/> | **Arrow** | Projectile obstacle launched horizontally from the left border. Flies to the right across the screen. |
| <img src="resources/images/bitmap/bitmap_bang.gif" width="100"/> | **Boom** | A short impact animation drawn wherever a Bug is squashed. Purely visual — it has no gameplay effect on its own. |
| <img src="resources/images/bitmap/bitmap_bamboo.png" width="100"/> | **Bamboo** | Three vertical poles representing the game lanes (Left, Middle, Right). |

> **Note:** For detailed object runtime sequences, see [Game Object Sequences](docs/01-design-sequence-object.md).

### III. How to Play:

- You control the **Panda**. Use the **[Up]** and **[Down]** buttons to jump between the 3 bamboo lanes. Pressing **[Up]** jumps **Right** (cycles lanes), and pressing **[Down]** jumps **Left** (cycles lanes).
- Press and quickly release the **[Mode]** button to climb **Up**. Press, hold, and then release the **[Mode]** button to climb **Down**.
- Avoid touching any incoming **Bugs** (unless stomping them from above) or **Arrows**.
- The goal is to survive as long as possible until the timer expires. The match ends when a Bug or Arrow hits the Panda.

#### Game Mechanics:

- **Scoring:** Points are earned by stomping on crawling Bugs from above. Landing on a Bug grants points depending on difficulty (Easy: +10 pts, Medium: +20 pts, Hard: +30 pts). The running score is displayed in the top-right corner HUD box.
- **Time limit & victory:** The remaining match duration is shown in the bottom-right corner HUD box. Survive until the countdown reaches 0 (30s, 60s, or 90s, configured in **Setting**) to achieve victory.
- **Difficulty settings:** Adjust the starting challenge in **Setting**:
  - **Easy:** 1 active Bug, lower spawn rate, slower speed.
  - **Medium:** Up to 2 active Bugs, faster speeds.
  - **Hard:** Up to 3 active Bugs, up to 2 active Arrows, highest speeds.
- **Screen Saver:** If left idle on menus for 15 seconds, the kit launches an animated "Bouncing Balls" screen saver to prevent OLED burn-in. Press **[Mode]** to wake up the system.
- **Game Over:** When a Bug or Arrow crashes into the Panda, the match ends, the objects reset, and the score is processed for the leaderboard. The **Game Over** screen offers 2 options:
    - **Retry:** Play again immediately.
    - **Home:** Return to the main menu.

---

### IV. Basic Game Sequence Logic

> **Note:** For a more detailed sequence flow, see [Runtime Signal Processing](docs/02-design-sequence-runtime.md).

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/design-sequence/pj_game_panda_jump_sequence.png" alt="basic panda jump game sequence logic" width="1000"/></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 7:</em></strong> Game sequence logic</p>

## Contact & Support

<p style="font-size: 20px;"><strong>[Your Name]</strong> - 3rd-Year Embedded Systems Intern</p>

``` Note
Thank you for visiting this repository.
If you have any questions, suggestions, or feedback about this project or firmware development, feel free to contact me directly.
```

<a href="https://github.com/your-username">
  <img src="https://img.shields.io/badge/GitHub-your--username-181717?style=for-the-badge&logo=github&logoColor=white"/>
</a>

<a href="https://linkedin.com/in/your-profile">
  <img src="https://img.shields.io/badge/LinkedIn-Your%20Name-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white"/>
</a>
