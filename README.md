# 🌊 Rain Pong

A modern Pong clone built in **C++** using **SFML**, featuring polished gameplay, multiple game modes, and a clean state-machine architecture.

---

## Features

- 🎮 One Player (AI) and Two Player modes
- ⏸️ Pause menu with Resume, Restart and Exit
- 🏆 Win detection and Game Over screen
- 🎵 Dynamic background music
- 🔄 Smooth menu ↔ gameplay music transitions
- 🔊 Interactive sound effects
  - Paddle collisions
  - Wall collisions
  - Score splash
  - Button hover & click
  - Victory & defeat sounds
- 🌧️ Rain-themed atmosphere
- 🤖 AI opponent
- ⚡ Increasing ball speed
- 🎨 Pixel-art inspired UI

---

## 🎮 Controls

### Player 1
| Key | Action |
|------|--------|
| **W** | Move Up |
| **S** | Move Down |

### Player 2
| Key | Action |
|------|--------|
| **↑** | Move Up |
| **↓** | Move Down |

### General
| Key / Mouse | Action |
|-------------|--------|
| Mouse | Navigate menus |
| Left Click | Select option |
| Esc | Pause / Resume |

---

## 🖥️ Screens

- Main Menu
- Gameplay
- Pause Menu
- Game Over Screen

---

## 🛠️ Built With

- **C++17**
- **SFML 3**
- Visual Studio Code
- Git & GitHub

---

## 🧠 Architecture

The game is organized using a **state machine**, making transitions between screens clean and easy to manage.

Current game states include:

- Menu
- Serve
- Single Player
- Two Player
- Paused
- Game Over

---

## 📂 Project Structure

```
Ocean-Pong/
│
├── assets/
│   └── Font/
│
├── main.cpp
├── .gitignore
└── README.md
```

---

## 🚀 Building

Compile using:

```bash
g++ main.cpp -o Pong.exe -lsfml-graphics -lsfml-window -lsfml-system
```

---

## 🎯 Future Improvements

- 🔊 Sound effects
- 🎵 Background music
- ✨ Particle effects
- 🌠 Ball trail
- 💥 Screen shake
- ⚙️ Settings menu
- 🎮 AI difficulty levels
- 🖥️ Fullscreen mode

---

## 📸 Future Showcase

- Gameplay GIF
- Screenshots
- Release builds

---

## 👨‍💻 Author

Astrath

First game project built while learning **C++**, **SFML**, and **game development**.

---

⭐ If you like the project, consider starring the repository!
