
# Chess Game

A fully functional chess game built with C++ and SFML (Simple and Fast Multimedia Library) featuring a graphical interface, drag-and-drop piece movement, and complete chess rule implementation.

## Features

### Core Gameplay
- **Complete Chess Rules**: All standard chess piece movements (Pawn, Knight, Bishop, Rook, Queen, King)
- **Special Moves**: Pawn promotion to Queen when reaching the opposite end
- **Drag and Drop Interface**: Intuitive piece movement using mouse controls
- **Move Validation**: Prevents illegal moves and ensures pieces move according to chess rules
- **Turn Management**: Alternating turns between white and black players

### Game States
- **Check Detection**: Visual warning when a king is in check (red highlight)
- **Checkmate Detection**: Game automatically ends when checkmate occurs
- **Stalemate Detection**: Recognizes draw conditions
- **Self-Check Prevention**: Blocks moves that would put your own king in check

### Visual Features
- **Classic Chessboard**: Traditional checkered pattern with coordinate labels (a-h, 1-8)
- **Piece Graphics**: High-quality sprites for all chess pieces
- **Square Highlighting**: Yellow highlight shows selected square
- **Check Warning**: Red highlight on king when in check
- **Captured Pieces Display**: Shows captured pieces on the side panel
- **Turn Indicator**: Displays whose turn it is
- **Game Status Messages**: Shows checkmate, stalemate, and check warnings

## Requirements

### Dependencies
- **SFML 2.x** (Graphics, Window, System modules)
- **C++ Compiler** with C++11 support or higher
- **Operating System**: Windows (paths are Windows-specific but can be adapted)

### Assets Required
The following image files must be in the specified directory:
- `white-king.png`
- `white-queen.png`
- `white-rook.png`
- `white-bishop.png`
- `white-knight.png`
- `white-pawn.png`
- `black-king.png`
- `black-queen.png`
- `black-rook.png`
- `black-bishop.png`
- `black-knight.png`
- `black-pawn.png`
- `inter.ttf` (font file)

**Note**: Update file paths in the code to match your asset locations.

## Installation

1. **Install SFML**:
   - Download SFML from [sfml-dev.org](https://www.sfml-dev.org/)
   - Follow installation instructions for your IDE/compiler

2. **Clone or Download** this project

3. **Update File Paths**:
   - Locate all instances of `"D:/A folder of mine/FAST/C++/Chess/x64/Debug/"` in the code
   - Replace with your actual asset directory path

4. **Configure Your IDE**:
   - Link SFML libraries (sfml-graphics, sfml-window, sfml-system)
   - Set include directories for SFML headers

5. **Compile and Run**

## How to Play

1. **Starting the Game**: Launch the application to see the standard chess starting position

2. **Making Moves**:
   - Click and hold on any piece to pick it up
   - Drag the piece to the desired square
   - Release the mouse button to drop the piece
   - Valid moves will be executed; invalid moves will return the piece to its original position

3. **Game Flow**:
   - White moves first
   - Players alternate turns
   - The current turn is displayed on the right panel
   - Console output shows move coordinates and status

4. **Winning**:
   - Checkmate your opponent's king to win
   - The game announces the winner when checkmate occurs

## Game Rules Implemented

- **Pawn**: Moves forward one square (two on first move), captures diagonally
- **Knight**: Moves in L-shape (2+1 squares)
- **Bishop**: Moves diagonally any number of squares
- **Rook**: Moves horizontally or vertically any number of squares
- **Queen**: Combines rook and bishop movement
- **King**: Moves one square in any direction
- **Pawn Promotion**: Pawns automatically promote to Queens upon reaching the last rank
- **Check**: King cannot move into or remain in check
- **Checkmate**: King is in check with no legal moves to escape
- **Stalemate**: No legal moves available but king is not in check

## Controls

- **Left Mouse Button**: Select and move pieces
- **Close Button**: Exit the game

## Code Structure

### Main Components
- `initializeBoard()`: Sets up the initial chess position
- `isValidMove()`: Validates moves according to chess rules
- `makeMove()`: Executes moves and updates board state
- `isCheck()`: Detects if the current player's king is in check
- `isCheckMate()`: Determines if checkmate has occurred
- `isStalemate()`: Checks for stalemate condition

### Piece Movement Functions
- `isValidPawnMove()`
- `isValidKnightMove()`
- `isValidBishopMove()`
- `isValidRookMove()`
- `isValidQueenMove()`
- `isValidKingMove()`

## Troubleshooting

**Problem**: Game won't launch or crashes
- **Solution**: Verify SFML is properly installed and linked
- Check that all asset files are in the correct directory
- Update file paths in the code

**Problem**: Pieces don't appear
- **Solution**: Ensure all PNG files are in the specified directory
- Check file names match exactly (case-sensitive)

**Problem**: Font not loading
- **Solution**: Verify `inter.ttf` is in the correct location
- Try using a different TrueType font file

## Credits

Developed using C++ and SFML library for graphics rendering and window management.
