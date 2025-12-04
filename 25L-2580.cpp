#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

const int SIZE = 8;
char board[SIZE][SIZE];

bool whiteTurn = true;
bool gameOver = false;
Sprite sprites[32];
bool actual_move;
int white_count = 0;
int black_count = 0;

// Function declarations
void initializeBoard();
bool isValidMove(int sx, int sy, int dx, int dy);
void makeMove(int sx, int sy, int dx, int dy);
bool algebraicToCoords(const string& alg, int& row, int& col);

// Check if position is within board boundaries
bool isInsideBoard(int r, int c)
{
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

// Returns true if destination piece belongs to opponent
bool isOwnPiece(int sx, int sy, char d_piece)
{
    char s_piece = board[sx][sy];
    bool white_s_piece = false;
    bool white_d_piece = false;

    // Uppercase characters represent white pieces
    if (s_piece >= 'A' && s_piece <= 'Z')
        white_s_piece = true;

    if (d_piece >= 'A' && d_piece <= 'Z')
        white_d_piece = true;

    // Same color means invalid capture
    if (white_s_piece == white_d_piece)
        return false;

    return true;
}

// Computes absolute movement difference
void calculateDifference(int sx, int sy, int dx, int dy, int& x_diff, int& y_diff)
{
    x_diff = dx - sx;
    y_diff = dy - sy;

    if (x_diff < 0)
        x_diff *= -1;

    if (y_diff < 0)
        y_diff *= -1;
}

// Checks diagonal movement and path clearance
bool isValidBishopMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    // Bishop must move diagonally
    if (x_diff != y_diff)
        return false;

    // No movement is invalid
    if (x_diff == 0)
        return false;

    // Direction of movement
    int x_step = (dx - sx) / x_diff;
    int y_step = (dy - sy) / y_diff;

    // Check for blocking pieces
    int crnt_x = sx + x_step;
    int crnt_y = sy + y_step;

    while (crnt_x != dx || crnt_y != dy)
    {
        if (board[crnt_x][crnt_y] != '.')
            return false;

        crnt_x += x_step;
        crnt_y += y_step;
    }

    char d_piece = board[dx][dy];

    // Empty destination is valid
    if (d_piece == '.')
        return true;

    // Capture only enemy
    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

// Checks L shape movement for knight
bool isValidKnightMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    // Knight must move in L shape
    bool l_shape = false;
    if ((x_diff == 2 && y_diff == 1) || (x_diff == 1 && y_diff == 2))
        l_shape = true;

    if (l_shape == false)
        return false;

    char d_piece = board[dx][dy];

    // Empty destination
    if (d_piece == '.')
        return true;

    // Capture only enemy
    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidKingMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    // No movement is invalid
    if (x_diff == 0 && y_diff == 0)
        return false;

    // King can move only one square in any direction
    if (x_diff > 1 || y_diff > 1)
        return false;

    char d_piece = board[dx][dy];

    // Empty destination is valid
    if (d_piece == '.')
        return true;

    // Capture only enemy piece
    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidRookMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    // Rook must move in straight line
    if (x_diff != 0 && y_diff != 0)
        return false;

    // No movement is invalid
    if (x_diff == 0 && y_diff == 0)
        return false;

    int x_step = 0;
    int y_step = 0;

    // Determine direction of movement
    if (x_diff != 0)
        x_step = (dx - sx) / x_diff;
    if (y_diff != 0)
        y_step = (dy - sy) / y_diff;

    // Check for blocking pieces in path
    int crnt_x = sx + x_step;
    int crnt_y = sy + y_step;
    while (crnt_x != dx || crnt_y != dy)
    {
        if (board[crnt_x][crnt_y] != '.')
        {
            return false;
        }

        crnt_x += x_step;
        crnt_y += y_step;
    }

    char d_piece = board[dx][dy];

    // Empty destination
    if (d_piece == '.')
        return true;

    // Capture only enemy
    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidQueenMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    // Queen combines rook and bishop logic
    bool flag = (isValidRookMove(board, sx, sy, dx, dy) || isValidBishopMove(board, sx, sy, dx, dy));
    return flag;
}

bool isValidPawnMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    bool white;

    // Check pawn color
    if (board[sx][sy] == 'P')
        white = true;
    else
        white = false;

    int x_step;

    // Pawn moves forward based on color
    if (white == true)
        x_step = -1;
    else
        x_step = 1;

    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    // First move two squares forward
    if ((y_diff == 0 && x_diff == 2) && (sx == 1 || sx == 6))
    {
        int crnt_x = sx + x_step;
        while (crnt_x != dx)
        {
            if (board[crnt_x][sy] != '.')
            {
                return false;
            }

            crnt_x += x_step;
        }

        // Destination must be empty
        if (board[dx][dy] != '.')
            return false;

        return true;
    }

    // Normal one square forward move
    else if (y_diff == 0 && x_diff == 1)
    {
        int crnt_x = sx + x_step;
        while (crnt_x != dx)
        {
            if (board[crnt_x][sy] != '.')
            {
                return false;
            }

            crnt_x += x_step;
        }

        // Destination must be empty
        if (board[dx][dy] != '.')
            return false;

        return true;
    }

    // Diagonal capture
    else if (y_diff == 1 && x_diff == 1)
    {
        char d_piece = board[dx][dy];

        // Pawn cannot move diagonally without capture
        if (d_piece == '.')
            return false;

        // Capture only enemy
        bool own_piece = isOwnPiece(sx, sy, d_piece);
        return own_piece;
    }

    return false;
}

bool isCheck()
{
    int king_x, king_y;
    bool flag = true;

    // Locate current player's king
    for (int i = 0; i < SIZE && flag == true; i++)
    {
        for (int j = 0; j < SIZE && flag == true; j++)
        {
            if (whiteTurn == true)
            {
                if (board[i][j] == 'K')
                {
                    king_x = i;
                    king_y = j;
                    flag = false;
                }
            }
            else
            {
                if (board[i][j] == 'k')
                {
                    king_x = i;
                    king_y = j;
                    flag = false;
                }
            }
        }
    }

    // Check if any enemy piece can attack the king
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            char piece = board[i][j];
            bool enemy = false;

            // Identify enemy pieces
            if (whiteTurn == true)
            {
                if (board[i][j] >= 'a' && board[i][j] <= 'z')
                    enemy = true;
            }
            else if (whiteTurn == false)
            {
                if (board[i][j] >= 'A' && board[i][j] <= 'Z')
                    enemy = true;
            }

            if (enemy == true)
            {
                bool attack = false;

                // Test attack based on piece type
                if (piece == 'B' || piece == 'b')
                    attack = isValidBishopMove(board, i, j, king_x, king_y);
                else if (piece == 'N' || piece == 'n')
                    attack = isValidKnightMove(board, i, j, king_x, king_y);
                else if (piece == 'K' || piece == 'k')
                    attack = isValidKingMove(board, i, j, king_x, king_y);
                else if (piece == 'R' || piece == 'r')
                    attack = isValidRookMove(board, i, j, king_x, king_y);
                else if (piece == 'Q' || piece == 'q')
                    attack = isValidQueenMove(board, i, j, king_x, king_y);
                else if (piece == 'P' || piece == 'p')
                    attack = isValidPawnMove(board, i, j, king_x, king_y);

                // King is under attack
                if (attack)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool isCheckMate()
{
    // If not in check, cannot be checkmate
    bool flag = isCheck();
    if (flag == false)
        return false;

    // Try every possible move to escape check
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            char piece;
            int sx = 0, sy = 0, dx, dy;
            bool valid;

            // Select only current player's pieces
            if (whiteTurn)
            {
                if (board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    piece = board[i][j];
                    sx = i;
                    sy = j;
                }
            }
            else
            {
                if (board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    piece = board[i][j];
                    sx = i;
                    sy = j;
                }
            }

            if (board[sx][sy] != '.')
            {
                for (int x = 0; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        dx = x, dy = y;

                        // Check if move is legal
                        valid = isValidMove(sx, sy, dx, dy);
                        if (valid == true)
                        {
                            // Save current board state
                            char s_piece = board[sx][sy];
                            char d_piece = board[dx][dy];

                            // Simulate move
                            actual_move = false;
                            makeMove(sx, sy, dx, dy);
                            actual_move = true;

                            // Test if check still exists
                            bool check = isCheck();

                            // Undo move
                            board[sx][sy] = s_piece;
                            board[dx][dy] = d_piece;

                            // If escape exists, not checkmate
                            if (check == false)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    // No move can save king
    return true;
}

bool algebraicToCoords(const string& alg, int& row, int& col) {
    // Input must be exactly 2 characters (e.g., e4)
    if (alg.length() != 2)
        return false;

    char file = alg[0];
    char rank = alg[1];
    bool flag = false;

    // File must be between a-h or A-H
    if ((file >= 'a' && file <= 'h') || (file >= 'A' && file <= 'H'))
        flag = true;
    if (flag == false)
        return false;

    // Rank must be between 1 and 8
    if (rank < '1' || rank > '8')
        return false;

    // Convert uppercase file to lowercase
    if (file >= 'A' && file <= 'H')
        file += 32;

    // Convert algebraic notation to array indexes
    col = file - 97;
    row = 8 - (rank - 48);

    return true;
}

void initializeBoard() {
    // Initial chess setup
    char start_pcs[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    // Copy initial setup into board
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = start_pcs[i][j];
        }
    }
}

bool isValidMove(int sx, int sy, int dx, int dy)
{
    // No piece at source square
    if (board[sx][sy] == '.')
    {
        return false;
    }

    char piece = board[sx][sy];

    // Prevent black from moving white pieces
    if (whiteTurn == false && (piece >= 'A' && piece <= 'Z'))
    {
        return false;
    }

    // Prevent white from moving black pieces
    else if (whiteTurn == true && (piece >= 'a' && piece <= 'z'))
    {
        return false;
    }

    bool flag = false;

    // Validate move based on piece type
    if (piece == 'B' || piece == 'b')
        flag = isValidBishopMove(board, sx, sy, dx, dy);
    else if (piece == 'N' || piece == 'n')
        flag = isValidKnightMove(board, sx, sy, dx, dy);
    else if (piece == 'K' || piece == 'k')
        flag = isValidKingMove(board, sx, sy, dx, dy);
    else if (piece == 'R' || piece == 'r')
        flag = isValidRookMove(board, sx, sy, dx, dy);
    else if (piece == 'Q' || piece == 'q')
        flag = isValidQueenMove(board, sx, sy, dx, dy);
    else if (piece == 'P' || piece == 'p')
        flag = isValidPawnMove(board, sx, sy, dx, dy);

    // Simulate move and check for self-check
    if (flag == true)
    {
        char s_piece = board[sx][sy];
        char d_piece = board[dx][dy];

        actual_move = false;
        makeMove(sx, sy, dx, dy);
        actual_move = true;

        bool check = isCheck();

        // Undo move
        board[sx][sy] = s_piece;
        board[dx][dy] = d_piece;

        // If move results in check, it is invalid
        if (check == true)
        {
            return false;
        }
    }

    return flag;
}

void makeMove(int sx, int sy, int dx, int dy)
{
    // If this is a real move (not a simulation)
    if (actual_move == true)
    {
        // If a piece is captured
        if (board[dx][dy] != '.')
        {
            char piece = board[dx][dy];
            // Remove captured sprite from screen
            for (int i = 0; i < 32; i++)
            {
                Vector2f piece_pos = sprites[i].getPosition();
                int piece_xpos = (piece_pos.y - 50) / 100;
                int piece_ypos = (piece_pos.x - 50) / 100;
                if (piece_xpos == dx && piece_ypos == dy)
                {
                    if (piece >= 'A' && piece <= 'Z')
                    {
                        sprites[i].setPosition(840.f + (white_count * 30), 100.f);
                        white_count++;
                    }
                    else
                    {
                        sprites[i].setPosition(840.f + (black_count * 30), 700.f);
                        black_count++;
                    }
                }
            }
        }
    }

    // Move piece in board array
    board[dx][dy] = board[sx][sy];
    board[sx][sy] = '.';

    // Pawn promotion check
    if (board[dx][dy] == 'P' || board[dx][dy] == 'p')
    {
        if (dx == 0)
            board[dx][dy] = 'Q';
        else if (dx == 7)
            board[dx][dy] = 'q';
    }
}

void makeChessBoard(RectangleShape square, RenderWindow& window)
{
    float position2 = 0.f;

    // Draw 8x8 checkered board
    for (int i = 1; i <= 8; i++)
    {
        float position1 = 0.f;
        for (int j = 1; j <= 8; j++)
        {
            // Alternate square colors
            if (((i + j) % 2) == 0)
                square.setFillColor(Color(240, 240, 211));
            else
                square.setFillColor(Color(122, 153, 90, 100));

            square.setPosition(Vector2f(position1, position2));
            window.draw(square);
            position1 += 100.f;
        }
        position2 += 100.f;
    }
}

void placePieces(string piece_name, float x, float y, Texture textures[], Sprite sprites[], int& index, string piece_names[])
{
    // Assign texture and position
    sprites[index].setTexture(textures[index]);
    sprites[index].setPosition(x + 50, y + 50);

    // Resize sprite and center it
    sprites[index].setScale(0.7f, 0.7f);
    sprites[index].setOrigin(textures[index].getSize().x / 2.f, textures[index].getSize().y / 2.f);

    // Store piece identifier
    piece_names[index] = piece_name;
    index++;
}

void showHihglight(int& selected_box_xpos, int& selected_box_ypos, RenderWindow& window, RectangleShape& highlight)
{
    // Highlight selected square
    highlight.setFillColor(Color(255, 255, 0, 100));
    highlight.setPosition(selected_box_xpos * 100.f, selected_box_ypos * 100.f);
    window.draw(highlight);
}

void chessBoardCoords(Event& event, RenderWindow& window, int& selected_box_xpos, int& selected_box_ypos)
{
    // Detect left mouse button click
    if (event.type == Event::MouseButtonPressed)
        if (event.mouseButton.button == Mouse::Left)
        {
            // Click must be inside board
            if (event.mouseButton.x < 800)
            {
                // Convert pixel to board coordinate
                int box_x = (event.mouseButton.x / 100) + 65;
                int box_y = (8 - (event.mouseButton.y / 100));

                selected_box_xpos = event.mouseButton.x / 100;
                selected_box_ypos = event.mouseButton.y / 100;

                // Print coordinates
                cout << "The coordinates of this box are : ("
                    << char(box_x) << ","
                    << box_y << ")" << endl;
            }
        }
}

void dragPiece(bool& drag, int& drg_indx, RenderWindow& window, Sprite sprites[], Vector2f& snap_fix)
{
    // Move sprite with mouse while dragging
    if (drag == true && drg_indx != -1)
    {
        Vector2i mouse_pos = Mouse::getPosition(window);
        sprites[drg_indx].setPosition(mouse_pos.x + snap_fix.x, mouse_pos.y + snap_fix.y);
        window.draw(sprites[drg_indx]);
    }
}


void displayLabels(RenderWindow& window, Font& font)
{
    // Draw file labels (a-h)
    for (int i = 0; i < 8; i++)
    {
        Text rank;
        rank.setFont(font);
        rank.setFillColor(Color::Black);
        rank.setCharacterSize(19);
        rank.setString(char('a' + i));
        rank.setPosition(3 + i * 100, 770);
        window.draw(rank);
    }

    // Draw rank labels (8-1)
    for (int i = 0; i < 8; i++)
    {
        Text file;
        file.setFont(font);
        file.setFillColor(Color::Black);
        file.setCharacterSize(19);
        file.setString(to_string(8 - i));
        file.setPosition(780, 3 + i * 100);
        window.draw(file);
    }
}

bool isStalemate()
{
    bool check = isCheck();
    if (check == true)
        return false;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for (int x = 0; x < SIZE; x++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    int dx = x, dy = y;
                    if (isValidMove(i, j, dx, dy))
                        return false;
                }
            }
        }
    }
    return true;
}

int main()
{
    // Initialize chess board
    initializeBoard();

    int x_input = -1;
    int y_input = -1;

    int selected_box_xpos = -1, selected_box_ypos = -1;
    float xpos, ypos;

    // Piece identifiers
    string chess_pieces[12] = {"white-rook", "white-knight", "white-bishop", "white-queen", "white-king", "white-pawn", "black-rook", "black-knight", "black-bishop", "black-queen", "black-king", "black-pawn"
    };

    // Create window
    RenderWindow window(VideoMode(1600, 800), "Chess Game", Style::Titlebar | Style::Close);

    RectangleShape square(Vector2f(100.f, 100.f));
    RectangleShape highlight(Vector2f(100.f, 100.f));
    RectangleShape red_hilight(Vector2f(100.f, 100.f));
    red_hilight.setFillColor(Color::Transparent);

    Texture textures[32];
    string piece_names[32];
    Font font;

    // Load font
    if (!font.loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/inter.ttf"))
    {
        cout << "Failed to load font!" << endl;
    }

    // Message display text
    Text display;
    display.setFont(font);
    display.setFillColor(Color::White);
    display.setCharacterSize(40);
    display.setPosition(840.f, 300.f);

    // Turn display
    Text turn;
    turn.setFont(font);
    turn.setFillColor(Color::White);
    turn.setCharacterSize(32);
    turn.setPosition(840.f, 400.f);

    // Check warning display
    Text check_display;
    check_display.setFont(font);
    check_display.setFillColor(Color::White);
    check_display.setCharacterSize(32);
    check_display.setPosition(840.f, 500.f);

    bool drag = false;
    int drg_indx = -1;
    Vector2f snap_fix;
    int index;
    bool tex_loaded = false;

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            // Close button
            if (event.type == Event::Closed)
                window.close();

            // Detect board coordinate click
            chessBoardCoords(event, window, selected_box_xpos, selected_box_ypos);

            bool stop = false;

            // Mouse press handling
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && stop == false)
            {
                // Stop interaction if game is over
                if (gameOver == true)
                    stop = true;

                if (stop == false)
                {
                    bool flag = true;

                    // Check if any piece is clicked
                    for (int i = 0; i < index; i++)
                    {
                        Vector2f piece_pos = sprites[i].getPosition();
                        if (flag == true && event.mouseButton.x >= piece_pos.x - 50 && event.mouseButton.x <= piece_pos.x + 50 && event.mouseButton.y >= piece_pos.y - 50 && event.mouseButton.y <= piece_pos.y + 50)
                        {
                            // Start dragging piece
                            drag = true;
                            drg_indx = i;
                            snap_fix = piece_pos - Vector2f(event.mouseButton.x, event.mouseButton.y);
                            cout << piece_names[i] << endl;

                            // Store original board position
                            x_input = (piece_pos.y - 50) / 100;
                            y_input = (piece_pos.x - 50) / 100;

                            flag = false;
                        }
                    }
                }
            }

            // When the left mouse button is released
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                // If a piece was being dragged
                if (drg_indx != -1)
                {
                    // Convert mouse position into board coordinates
                    int dx = event.mouseButton.y / 100;
                    int dy = event.mouseButton.x / 100;

                    // Store original position of the piece
                    int sx = x_input;
                    int sy = y_input;

                    // If the drop is outside the chessboard
                    if (dx < 0 || dx > 7 || dy < 0 || dy > 7)
                    {
                        // Return piece to original square
                        sprites[drg_indx].setPosition((sy * 100) + 50, (sx * 100) + 50);
                        drag = false;
                        drg_indx = -1;
                        continue;
                    }

                    // Check if the move is valid
                    if (isValidMove(sx, sy, dx, dy))
                    {
                        // Apply move in game logic
                        makeMove(sx, sy, dx, dy);

                        // Move sprite to new square
                        sprites[drg_indx].setPosition(dy * 100 + 50, dx * 100 + 50);

                        // Switch turns
                        whiteTurn = !whiteTurn;

                        // Update turn text
                        if (whiteTurn == true)
                            turn.setString("White's Turn");
                        else
                            turn.setString("Black's Turn");

                        // Handle pawn promotion to queen (white)
                        if (board[dx][dy] == 'Q')
                        {
                            textures[drg_indx].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/white-queen.png");
                            sprites[drg_indx].setTexture(textures[drg_indx]);
                        }
                        // Handle pawn promotion to queen (black)
                        else if (board[dx][dy] == 'q')
                        {
                            textures[drg_indx].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/black-queen.png");
                            sprites[drg_indx].setTexture(textures[drg_indx]);
                        }

                        // Print move status
                        cout << "Move OK\n";

                        // Check for checkmate and check
                        bool end = isCheckMate();
                        bool check = isCheck();

                        // If checkmate
                        if (end == true)
                        {
                            gameOver = true;

                            // Determine winner
                            if (whiteTurn == true)
                            {
                                cout << "\nCheckmate! Black wins.";
                                display.setString("Checkmate! Black wins");
                            }
                            else
                            {
                                cout << "\nCheckmate! White wins.";
                                display.setString("Checkmate! White wins");
                                check_display.setString("");
                            }
                        }
                        else if (isStalemate())
                        {
                            gameOver = true;
                            display.setString("Stalemate");
                            cout << "Stalemate!" << endl;
                        }
                        // If check only
                        else if (check == true)
                        {
                            cout << "\nCheck";
                            check_display.setString("Check!");

                            // Find the king and highlight it
                            bool red = false;
                            for (int i = 0; i < SIZE && red == false; i++)
                            {
                                for (int j = 0; j < SIZE && red == false; j++)
                                {
                                    if ((whiteTurn == true && board[i][j] == 'K') ||
                                        (whiteTurn == false && board[i][j] == 'k'))
                                    {
                                        red_hilight.setFillColor(Color(255, 130, 132));
                                        red_hilight.setPosition(j * 100.f, i * 100.f);
                                        red = true;
                                    }
                                }
                            }
                        }
                        // No check
                        else
                        {
                            check_display.setString("");
                            red_hilight.setFillColor(Color::Transparent);
                        }
                    }
                    // Invalid move
                    else
                    {
                        // Restore original position
                        sprites[drg_indx].setPosition(sy * 100 + 50, sx * 100 + 50);
                        cout << "Invalid move\n";
                    }
                }

                // Stop dragging
                drag = false;
                drg_indx = -1;
            }

            // Clear the window with background color
            window.clear(Color(38, 38, 38));

            if (tex_loaded == false)
            {
                index = 0;
                // Draw white major pieces (left to middle)
                xpos = 0, ypos = 700;
                for (int i = 0; i < 5; i++)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[i] + ".png");
                    placePieces(chess_pieces[i], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }

                // Draw white major pieces (right side mirrored)
                xpos = 500, ypos = 700;
                for (int i = 2; i > -1; i--)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[i] + ".png");
                    placePieces(chess_pieces[i], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }

                // Draw black major pieces (top row, left to middle)
                xpos = 0, ypos = 0;
                for (int i = 6; i < 11; i++)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[i] + ".png");
                    placePieces(chess_pieces[i], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }

                // Draw black major pieces (right side mirrored)
                xpos = 500, ypos = 0;
                for (int i = 8; i > 5; i--)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[i] + ".png");
                    placePieces(chess_pieces[i], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }

                // Draw black pawns
                xpos = 0, ypos = 100;
                for (int i = 0; i < 8; i++)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[11] + ".png");
                    placePieces(chess_pieces[11], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }

                // Draw white pawns
                xpos = 0, ypos = 600;
                for (int i = 0; i < 8; i++)
                {
                    textures[index].loadFromFile("D:/A folder of mine/FAST/C++/Chess/x64/Debug/" + chess_pieces[5] + ".png");
                    placePieces(chess_pieces[5], xpos, ypos, textures, sprites, index, piece_names);
                    xpos += 100.f;
                }
                tex_loaded = true;
            }

            // Draw the chessboard squares
            makeChessBoard(square, window);

            // Draw file and rank labels (a-h, 1-8)
            displayLabels(window, font);

            // Highlight selected square
            showHihglight(selected_box_xpos, selected_box_ypos, window, highlight);

            // Draw red highlight for king in check
            window.draw(red_hilight);

            // Draw all pieces except the one being dragged
            for (int i = 0; i < index; i++)
            {
                if (i != drg_indx) // Skip piece being dragged
                {
                    window.draw(sprites[i]);
                }
            }

            // Draw the piece being dragged following mouse
            dragPiece(drag, drg_indx, window, sprites, snap_fix);

            // Draw text displays: messages, turn, scores, check status
            window.draw(display);
            window.draw(turn);
            window.draw(check_display);

            // Show everything on the window
            window.display();
        }
    }
    return 0;
}
