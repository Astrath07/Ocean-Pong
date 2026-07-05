# 🌊 Ocean Pong

A modern Pong clone built in **C++** using **SFML**, featuring polished gameplay, multiple game modes, and a clean state-machine architecture.

---

## ✨ Features

- 🎮 Single-player mode with AI opponent
- 👥 Local two-player mode
- ⏸️ Pause menu
- 🎲 Randomized serves
- 🏆 Game Over screen with winner announcement
- 🖱️ Interactive menus with mouse hover effects
- ⚙️ State-machine based game flow

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
