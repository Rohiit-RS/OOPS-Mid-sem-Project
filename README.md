# UNO (C++ Console Game)

A full-featured, interactive, console-based multiplayer implementation of the classic **UNO** card game built in modern C++.

Designed with clean Object-Oriented Programming (OOP) principles, pass-and-play local multiplayer support (2–10 players), and faithful enforcement of the official UNO rules.

---

## Features

- **2 to 10 Players Support:** Seamless pass-and-play local multiplayer with custom player names.
- **Complete Card Deck (108 Cards):**
  - Number cards (0–9) across 4 colors: **Red**, **Yellow**, **Green**, and **Blue**.
  - Action cards: **Skip (S)**, **Reverse (R)**, and **Draw Two (D)**.
  - Wild cards: **Wild (W)** and **Wild Draw Four (4)**.
- **Authentic UNO Rules & Mechanics:**
  - **Start Card Effects:** Action start cards (Skip, Reverse, Draw Two, Wild) trigger their respective effects on round 1.
  - **Two-Player Special Rule:** Reverse acts as a Skip in a 2-player match.
  - **Wild Draw Four Challenge:** Challengers can challenge a played Wild Draw 4. If the player held a matching active color card, they draw 4 cards instead as penalty; otherwise, the challenger draws 6 cards.
  - **Automatic UNO Call:** Alerts all players when someone is down to their final card.
  - **Deck Recycling:** Discard pile is automatically reshuffled into the draw pile when cards run out.
  - **Hand Sorting:** Hands are automatically organized by color and rank for easy readability.
  - **Turn Hiding / Screen Masking:** Screen clearing and turn confirmation prompts prevent opponents from peeking at each other's hands.
  - **Turn Event Logs:** Displays what action occurred between a player's turns.
  - **Match Progression & Stats:** Dealer rotates each match, tracking overall games played.

---

## Object-Oriented Architecture

This project highlights core C++ OOP paradigms:

| Class / File | Responsibility |
|---|---|
| [`Card`](Card.h) / [`Card.cpp`](Card.cpp) | Represents an individual card (Color & Value). Implements comparison (`==`, `<`), string conversion, and `operator<<`. |
| [`Deck`](Deck.h) / [`Deck.cpp`](Deck.cpp) | Manages the draw and discard piles, deck initialization (108 cards), shuffling, and pile recycling. |
| [`Player`](Player.h) / [`Player.cpp`](Player.cpp) | Represents a player, their hand, playable card validation, hand sorting, stream I/O (`>>`, `<<`), and `operator+=`. |
| [`Game`](Game.h) / [`Game.cpp`](Game.cpp) | Coordinates game loop, turn flow, challenges, dealer rotation, screen management, and Rule of Three memory management. |
| [`Utils.h`](Utils.h) | Utility functions, input validation helpers, and generic templates (`swapValues`, `shuffleArray`). |
| [`main.cpp`](main.cpp) | Entry point that initializes random seeding and starts the game session. |

---

## File Structure

```text
UNO/
├── Card.h        # Card class header
├── Card.cpp      # Card class implementation
├── Deck.h        # Deck class header
├── Deck.cpp      # Deck class implementation
├── Player.h      # Player class header
├── Player.cpp    # Player class implementation
├── Game.h        # Game controller header
├── Game.cpp      # Game engine implementation
├── Utils.h       # Utility functions and template helpers
├── main.cpp      # Application entry point
├── .gitignore    # Git ignore rules for C++ builds and IDEs
└── README.md     # Project documentation
```

---

## Getting Started

### Prerequisites

You need a C++ compiler supporting C++11 or higher:
- **GCC / MinGW** (`g++`)
- **Clang** (`clang++`)
- **MSVC** (Visual Studio)

---

### Compilation & Execution

#### Using GCC / G++ (Linux / macOS / Windows MinGW)
```bash
g++ -std=c++11 -O2 -o uno main.cpp Card.cpp Player.cpp Deck.cpp Game.cpp
```
Run the executable:
- **Linux / macOS:** `./uno`
- **Windows:** `.\uno.exe`

#### Using Microsoft Visual C++ (MSVC Developer Command Prompt)
```cmd
cl /EHsc /std:c++14 main.cpp Card.cpp Player.cpp Deck.cpp Game.cpp /Fe:uno.exe
uno.exe
```

---

## How to Play

1. **Launch the game:** Enter the number of players (2 to 10) and their names.
2. **Taking Turns:**
   - The active player receives a prompt to take the device.
   - The player views the top discard card, active color, play direction, and their sorted hand.
   - Enter the number corresponding to the card you wish to play, or enter `0` to draw from the deck.
3. **Playing a Drawn Card:** If the card drawn matches the active card/color, you will be prompted if you want to play it immediately.
4. **Wild Cards:** When playing a Wild or Wild Draw 4, pick a color (1: Red, 2: Yellow, 3: Green, 4: Blue).
5. **Wild Draw 4 Challenge:** The targeted player can challenge the play if they suspect the opponent had a card matching the active color.
6. **Winning:** The first player to empty their hand wins the round!

