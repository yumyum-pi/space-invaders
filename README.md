# Space Invaders (Terminal, C)

A minimalist **terminal-based Space Invaders–style game written in C**, built
to explore low-level programming, POSIX terminals, and core game-engine
concepts—without external libraries.

This project is intentionally simple and constrained: no graphics libraries,
no game frameworks, just C, a terminal, and a game loop.

---

## 🎯 Project Goals

This project exists primarily as a **learning exercise**, with focus on:

- Writing modern, readable **C (C11)**
- Understanding **raw terminal input** using POSIX APIs
- Building a **game loop** from scratch
- Separating input, update, and render phases
- Exploring engine-style thinking in a constrained environment

If you’re coming from higher-level languages or engines, this project is about
seeing what’s _actually_ happening under the hood.

---

## 🖥️ Platform Support

- **OS:** macOS (currently developed and tested)
- **Terminal:** POSIX-compliant terminal required
- **Input Mode:** Raw mode (`termios`)

> ⚠️ Linux has not been tested yet.
> If you try it on Linux and run into issues—or fix them—**contributions are
> very welcome**.

---

## 🛠️ Toolchain

- **Compiler:** `clang`
- **C Standard:** C11
- **Libraries:**

  - No external libraries
  - Uses only POSIX APIs (`termios`, `unistd`, `read`, etc.)

---

## 🚀 Build & Run

The project uses `make`.

### Build (Debug)

```sh
make debug
```

### Build (Release)

```sh
make release
```

### Run

```sh
make run
```

### Clean

```sh
make clean
```

## 🎮 Controls

|     Key | Action     |
| ------: | ---------- |
|     `W` | Move up    |
|     `A` | Move left  |
|     `S` | Move down  |
|     `D` | Move right |
| `Space` | Fire       |
|     `Q` | Quit game  |

**Note:**
Simultaneous input (e.g. move + shoot at the same time)
is **not supported** at the moment.

---

## 🧪 Current State

The game is **partially playable**.

### Implemented

- [x] Player movement
- [x] Shooting
- [x] Enemies
- [x] Enemy projectiles
- [x] Collision detection

### Not Yet Implemented

- [ ] Score system
- [ ] HUD
- [ ] Levels
- [ ] Walls / shields
- [ ] Game over screen

---

## 🗺️ Roadmap

Planned improvements and experiments:

- Alien formations
- Multiple levels
- Sound (maybe 😄)
- Refactoring toward engine-style modules
- ECS-style architecture exploration

This roadmap is flexible—the project evolves as learning goals change.

---

## 🤝 Contributing

Contributions are welcome, especially if you:

- Test the game on **Linux**
- Improve terminal compatibility
- Refactor or clean up code
- Add missing gameplay features
- Improve documentation

### Guidelines

- Keep changes **simple and readable**
- Avoid introducing external libraries
- Prefer clarity over cleverness
- Small, focused PRs are best

If you’re unsure about an idea, feel free to open an issue first.

---

## 📄 License

This project is licensed under the **Apache License 2.0**.
See the `LICENSE` file for details.

```text
# enemy
\\-v-//

#player
/=|^|=\
```
