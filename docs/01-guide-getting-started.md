<h1 align="center">Game programming getting started guide</h1>

Welcome to the game programming project on the STM32L151 microcontroller! This repository provides a foundational source code base along with detailed documentation to help you quickly get familiar with the system architecture and start building your own game.

---

## Table of Contents

- [I. Create Your Own "Playground" (Fork)](#i-create-your-own-playground-fork)
- [II. Quick Start Guide (Environment Setup)](#ii-quick-start-guide-environment-setup)
- [III. Game Programming Workflow](#iii-game-programming-workflow)
  - [Step 1: Create your working directory](#step-1-create-your-working-directory)
  - [Step 2: Clone the repo to your machine](#step-2-clone-the-repo-to-your-machine)
  - [Step 3: Modify the Game](#step-3-modify-the-game)
  - [Step 4: Push your code to GitHub](#step-4-push-your-code-to-github)

---

## I. Create Your Own "Playground" (Fork)

To initialize your personal project, follow these steps:

### 1. Access the original repository

**Link:** [https://github.com/the-ak-foundation/ak-base-kit-stm32l151](https://github.com/the-ak-foundation/ak-base-kit-stm32l151)

### 2. Fork the repository

Click the **Fork** button in the top-right corner to create a copy of the project under your personal account.
You can also click the **Star** button next to **Fork** to support the author.

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_view_ak_foundation_repo.png" />
</p>

### 3. Create the fork

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_create_fork.png" />
</p>

> **Note:**
> - Name the repository after **your game's name**.
> - Add a brief description of your game in the **Description** field.

Once the fork is created, GitHub redirects you to the new repository — this is the "skeleton" you will use to develop and complete your game:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_repo_after_fork.png" />
</p>

---

## II. Quick Start Guide (Environment Setup)

To build the source code and flash firmware onto the kit, you need to set up the development environment on Ubuntu/Linux. Step-by-step instructions are available here:

**[AK Embedded Base Kit STM32L151 — Getting Started](https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started)**

---

## III. Game Programming Workflow

> **Note:** Since this is an embedded software project, you should use the **Terminal on an Ubuntu/Linux environment** to ensure the build and flashing process runs correctly.

### Step 1: Create your working directory

From your `Home` directory, create a folder named **Workspace** — this will hold all of your source code and programming tools. Inside `Workspace`, create two subdirectories: `Sources` and `Tools`.

You can do this quickly by opening your Terminal and running the following commands:

```bash
cd ~
mkdir Workspace
cd Workspace
mkdir Sources Tools
```

Your directory structure should now look like this:

```text
Workspace/
├── Sources/   # Holds your programming projects (where you will clone Panda Jump)
└── Tools/     # Holds the programming tools (see details in Section II)
```

---

### Step 2: Clone the repo to your machine

> **Note:** This step only needs to be done **once** when starting the project.

Open a **Terminal** inside the `Sources` directory and run the following command (replace the URL with your own repository link):

```bash
git clone https://github.com/<your-username>/<your-cloned-repo-name>.git
```

---

### Step 3: Modify the Game

Open **VSCode** on Linux, then open the freshly cloned repository to start coding.

First, set your game's name in the `Makefile.mk` file located in the `application/` directory:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_rename_repo_for_game.png" />
</p>

All game logic lives in the `application/sources/app` directory.

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_build_and_run_application.png" />
</p>

#### Example: Displaying the Chart screen in the Panda Jump game

**Step 3.1 —** Create a header file `scr_chart.h` in the `screens/` directory to declare the functions that draw the Chart screen:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_create_chart_header.png" />
</p>

**Step 3.2 —** Create `scr_chart.cpp` to handle the bitmap data and render the Chart graphics on the display:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_create_chart_source.png" />
</p>

**Step 3.3 —** Create a header file `screens_bitmap.h` in the `screens/` directory to declare shared bitmap data:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_create_bitmap_header.png" />
</p>

**Step 3.4 —** Create `screens_bitmap.cpp` containing the Chart's bitmap data:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_create_bitmap_source.png" />
</p>

**Step 3.5 —** Include the Chart header file in `screens.h`:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_include_headers_in_screens.png" />
</p>

**Step 3.6 —** Update the `screen_tran_menu()` function inside `scr_menu_game.cpp` to transition to the Chart screen when the user selects the Charts menu item:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_update_switch_case_in_scr_menu_game.png" />
</p>

**Step 3.7 —** Add the two files `scr_chart.cpp` and `screens_bitmap.cpp` to `Makefile.mk` inside the `screens/` directory so they get compiled:

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_add_bitmap_and_chart_to_project.png" />
</p>

**Step 3.8 —** Build and flash the firmware onto the kit (see detailed instructions in [Section II](#ii-quick-start-guide-environment-setup)):

<p align="center">
  <img style="max-width: 100%; height: auto;" alt="repo-ak" src="../resources/images/guide-getting-started/getting_started_final_result.jpg" />
</p>

---

### Step 4: Push your code to GitHub

After finishing a feature, save your progress to your personal repo with the commands below (run them from the **root directory** of the repo):

```bash
git add .
git commit -m "Create Chart screen for Panda Jump"
git push origin main
```
From here, anyone can visit your GitHub link to follow your progress and try out the game you've built.

---

## References

- Blog: [AK Embedded Software](https://epcb.vn/blogs/ak-embedded-software)

---

## Contact & Support

<p style="font-size: 20px;"><strong>Nguyen Hoang Minh Quoc</strong> — Firmware Engineer - Embedded system</p>

``` Note
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
<a href="mailto:quoc20053008@email.com">
  <img src="https://img.shields.io/badge/MinhQuocNguyenHoang-EA4335?style=flat&logo=gmail&logoColor=white"/>
</a>

<p align="center">
  <i>Happy coding, and may you build some truly fun games!</i>
</p>
