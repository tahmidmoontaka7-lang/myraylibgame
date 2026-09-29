# 🚀 Pokemon Shooter: Ultimate Boss Edition

A high-performance, lightweight 2D arcade shooter built from the ground up using the core **C programming language** and the **Raylib graphics engine**. This project demonstrates system architecture design, memory management optimization, and robust low-level compiler linking on Windows environments.
---

## 🛠️ Systems Architecture & Core Features

- **Finite State Machine (FSM):** The entire execution architecture is controlled via an isolated FSM engine (`enum GameState`), managing layout transitions smoothly between Menu, Dynamic Difficulty Selection, Active Gameplay, Rules, and Game Over sequences.
- **Dynamic 3-Tier Difficulty Scaling:** Features isolated tracking layers for Easy, Medium, and Hard configurations. Complexity metrics dynamically adapt system variables like player lives, baseline target velocities, and recursive vector transformations.
- **Persistent High-Score Registry (File I/O):** Built a standard C file handling layer using secure data streams (`fopen`, `fscanf`, `fprintf`) to dynamically read and write separate local text files (`.txt`) for cross-session data storage.
- **Autonomous Boss Engine & Counter-Attacks:** Triggered at specific score thresholds, the runtime context hot-swaps standard linear arrays to spawn a massive multi-layered Titan Class Carrier complete with a separate visual health bar tracking matrix and automatic plasma projectile attacks.
- **Resource Management & Streaming Audio:** Implemented efficient audio buffer configurations for tracking cyberpunk-themed background music and low-latency weapon output triggers (`PlaySound`, `UpdateMusicStream`), backed by standard garbage collection cleanup protocols to eliminate memory drops.

---

## 📁 Repository Structure

```text
├── main.c                          # Main compilation logic and FSM architecture
├── raylib.h                        # Raylib engine bindings
├── libraylib.a                     # 64-bit precompiled environment asset binary
├── Cyberpunk Moonlight Sonata.mp3  # Streaming BGM track file
├── shoot.wav                       # Low-latency weapon SFX buffer
└── explosion.wav                   # Procedural impact SFX buffer
```
---
## 💻 Compilation & Installation

This project is built inside an isolated 64-bit Windows compiler toolchain environment using `w64devkit` (GCC/MinGW) to guarantee zero structural execution friction.

To link the dependencies and compile the execution binary locally, drop the following block into your toolchain terminal workspace:

```bash
gcc main.c -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
./game.exe
```
---

## 🎮 How to Play

- **Navigation:** Use `Arrow Keys` to manipulate the reactive spaceship matrix dynamically across all coordinate axes.
- **Offensive Actions:** Smash the `Spacebar` to fire high-energy deep purple plasma laser lines.
- **Tactical Directives:** Score **100+ points** to call forth the Dreaded Boss Fleet. Avoid collision matrices with structural items, or your vitals reset instantly!
- **Tribute:** *Dedicated to the Light Queen.* 👑
