# 👾 Pac-Man — Terminal Edition

A two-level Pac-Man clone that runs entirely in the terminal, written in C with **ncurses**.

---

## Table of Contents

- [Installation — ncurses](#installation--ncurses)
- [Building & Running](#building--running)
- [Controls](#controls)
- [Gameplay Rules](#gameplay-rules)
- [Ghost Behaviour](#ghost-behaviour)
- [Level Differences](#level-differences)
- [File Overview](#file-overview)

---

## Installation — ncurses

### Linux (Debian / Ubuntu)
```bash
sudo apt update
sudo apt install libncursesw5-dev
```

### Linux (Arch / Manjaro)
```bash
sudo pacman -S ncurses
```

### Linux (Fedora / RHEL)
```bash
sudo dnf install ncurses-devel
```

### macOS
ncurses ships with macOS, but the Homebrew version is newer and recommended:
```bash
brew install ncurses
# Then point the compiler at it:
export LDFLAGS="-L/opt/homebrew/opt/ncurses/lib"
export CPPFLAGS="-I/opt/homebrew/opt/ncurses/include"
```

### Windows
ncurses does not run natively on Windows. Use one of the following:

| Option | Steps |
|--------|-------|
| **WSL 2** (recommended) | Install WSL 2, then follow the Linux instructions above inside the WSL shell. |
| **MSYS2 / MinGW-w64** | `pacman -S mingw-w64-x86_64-ncurses` inside an MSYS2 terminal. |
| **Cygwin** | Select the `libncurses-devel` package in the Cygwin installer. |

---

## Building & Running

### Build the game
```bash
make
```

### Run the game
```bash
make run
# or
./pacman_game
```

### Clean build artefacts
```bash
make clean
```

---

## Controls

### In-Menu Navigation
| Key | Action |
|-----|--------|
| `↑` `↓` Arrow keys | Move highlight up / down |
| `Enter` | Confirm selection |
| `q` | Quit |

### In-Game
| Key | Action |
|-----|--------|
| `w` | Move up |
| `s` | Move down |
| `a` | Move left |
| `d` | Move right |
| `q` | Quit to terminal |

> **Arrow keys do not work in-game.** Only `w a s d` and `q` are recognised during play.

### Pre-move (buffered input)
One move may be **queued in advance**. If Pac-Man is currently blocked (e.g. heading into a wall), the last valid key you pressed is remembered and executed automatically as soon as the path opens. This lets you pre-tap a turn before reaching a corner without losing momentum.

---

## Gameplay Rules

- **Objective** — eat every pacdot (`.`) on the map.
- **Game over** — the game ends when either all pacdots are consumed **or** all three lives are lost.
- **Lives** — you start with **3 lives** shown as ❤️ on screen. Colliding with a ghost (while not in Frightened mode) costs one life and resets all entity positions.
- **Power pellets** (`o`) — eating one switches all ghosts into **Frightened** mode for 7 seconds. While frightened, ghosts flee and can be eaten. Eaten ghosts respawn at their starting positions and are temporarily removed from play until Frightened mode ends.
- **Score** — one point is awarded for each pacdot eaten.

---

## Ghost Behaviour

Each ghost has a unique AI personality. Behaviour alternates between **Scatter** mode and **Chase** mode on a fixed timer, and switches to **Frightened** mode when a power pellet is eaten.

### Mode Timeline (per life)

| Time (s) | Mode |
|----------|------|
| 0 – 5 | Scatter |
| 5 – 25 | Chase |
| 25 – 30 | Scatter |
| 30 – 50 | Chase |
| 50 – 55 | Scatter |
| 55+ | Chase (permanent) |

---

### 🔴 Blinky — *The Shadow*

**Chase:** Blinky always targets Pac-Man's **exact current tile**. He is the most relentless ghost — the longer the game goes on, the more dangerous he becomes because he never stops pursuing. He will never voluntarily give up ground.

**Scatter:** Retreats to the **top-right corner** of the map.

---

### 🩷 Pinky — *The Speedy*

**Chase:** Pinky targets the tile **4 cells ahead** of Pac-Man in Pac-Man's current direction of travel. She tries to set an **ambush** — cutting off where Pac-Man is going rather than chasing from behind. She is most effective when Pac-Man is moving in a straight line.

**Scatter:** Retreats to the **top-left corner** of the map.

---

### 🩵 Inky — *The Bashful*

**Chase:** Inky uses the most complex targeting of all four ghosts. His target is calculated as follows:

1. Take the tile **2 cells ahead** of Pac-Man (a "pivot" point).
2. Draw a vector from **Blinky's position** to that pivot point.
3. **Double** that vector to get Inky's target tile.

This means Inky's behaviour is partially dependent on Blinky's position. When Blinky is far away, Inky targets tiles far from Pac-Man; when Blinky is close, Inky converges. Together they form a pincer.

**Scatter:** Retreats to the **bottom-right corner** of the map.

---

### 🟠 Clyde — *The Pokey*

**Chase:** Clyde uses a **proximity switch**:
- If Clyde is **more than 8 tiles** away from Pac-Man, he chases Pac-Man's exact tile (like Blinky).
- If Clyde is **8 tiles or closer**, he abandons the chase and instead heads to his scatter corner, effectively retreating.

This makes Clyde appear cowardly — he approaches, panics when close, and veers away. He is the least threatening ghost in a direct chase but can be unpredictable.

**Scatter:** Retreats to the **bottom-left corner** of the map.

---

### Frightened Mode (all ghosts)

When a power pellet is eaten, **all four ghosts immediately U-turn** and then begin fleeing. In Frightened mode, each ghost moves **away** from Pac-Man by picking the available direction that maximises distance from him. Ghosts that are eaten while frightened respawn in the center cage and are inactive until Frightened mode expires.

---

## Level Differences

The two levels use different pathfinding algorithms for ghost movement — and the results were not what we expected.

| Feature | Level 1 | Level 2 |
|---------|---------|---------|
| Ghost pathfinding | **Dijkstra's shortest path** — ghosts precompute the true shortest route through the maze and follow it optimally. | **Greedy / Manhattan distance** — ghosts pick the move that minimises straight-line distance to their target each step, ignoring walls. |
| Expected difficulty | Harder | Easier |
| Actual difficulty | **Easier** 😅 | **Harder** 😅 |

**The surprise:** Dijkstra's algorithm turned out to be *easier* than the greedy Manhattan distance approach. Because Dijkstra always takes the globally shortest path, ghost movement becomes very predictable — once you learn the routes, you can reliably evade them. The greedy approach, on the other hand, makes ghosts erratic and harder to read: they don't always take the sensible path, they get confused by dead ends, and that unpredictability is genuinely more stressful to play against. So if you want a real challenge, Level 2 is the one.

---

## File Overview

| File | Purpose |
|------|---------|
| `mainmenu.c` | Entry point; main menu and level selection |
| `pacman.c / pacman.h` | Level 1 game loop and rendering |
| `pacman2.c / pacman2.h` | Level 2 game loop and rendering |
| `movement.c / movement.h` | Entity movement, turn logic, spawn functions, greedy pathfinding |
| `ghosts.c / ghosts.h` | Level 1 ghost AI (Dijkstra-based) |
| `level2_ghosts.c / level2_ghosts.h` | Level 2 ghost AI (greedy Manhattan distance) |
| `level2_graph.c / level2_graph.h` | Graph construction, Dijkstra precomputation, direction lookup tables |
| `grid2copy.txt` | ASCII maze layout used by both levels |
| `adjlist.txt` | Pre-generated adjacency list for the maze graph |
| `Makefile` | Build rules for the game |