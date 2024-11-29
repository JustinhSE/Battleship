# Battleship Game Server

This project implements a server for a networked version of the classic Battleship game. It allows two players to connect and play against each other.

## Features

- **Dual Socket Networking**: Utilizes two separate sockets to handle connections from two players simultaneously.
- **Custom Game Logic**: Implements the rules and mechanics of Battleship, including ship placement and attack resolution.
- **Dynamic Board Management**: Supports variable board sizes and different ship configurations.
- **Real-time Gameplay**: Facilitates turn-based gameplay with immediate feedback to players.

## Technical Details

### Networking

- Uses TCP/IP protocol for reliable communication.
- Listens on two ports (2201 and 2202) for player connections.
- Implements socket programming with the `<sys/socket.h>` and `<arpa/inet.h>` libraries.

### Game State Management

- Maintains separate game boards for each player.
- Tracks ship placements, guesses, and remaining ships for both players.
- Utilizes 2D arrays to represent game boards and player actions.

## Code Structure

- **Main Function**: Sets up sockets, accepts connections, and manages the game flow.
- **Helper Functions**:
  - `ship_remains()`: Checks if a ship is still on the board.
  - `piece_guessed()`: Verifies if a position has been guessed.
  - `query()`: Generates game state updates for players.
  - `fill_shape()`: Handles ship placement on the board.
