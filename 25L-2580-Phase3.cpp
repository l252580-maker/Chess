#include <iostream>
#include <string>
using namespace std;

const int SIZE = 8;
char board[SIZE][SIZE];
bool whiteTurn = true;
bool gameOver = false;

// Function prototypes
void initializeBoard();
void printBoard();
bool isValidMove(int sx, int sy, int dx, int dy);
void makeMove(int sx, int sy, int dx, int dy);

// --- Student task: implement this function ---
// Convert algebraic like "e2" -> returns true and fills row,col.
// If input is invalid, return false.
bool algebraicToCoords(const string& alg, int& row, int& col);

// Helper: check inside board
bool isInsideBoard(int r, int c) {
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

bool isOwnPiece(int sx, int sy, char d_piece)
{
    char s_piece = board[sx][sy];
    bool white_s_piece = false;
    bool white_d_piece = false;
    if (s_piece >= 'A' && s_piece <= 'Z')
        white_s_piece = true;

    if (d_piece >= 'A' && d_piece <= 'Z')
        white_d_piece = true;

    if (white_s_piece == white_d_piece)
        return false;

    return true;
}

void calculateDifference(int sx, int sy, int dx, int dy, int& x_diff, int& y_diff)
{
    x_diff = dx - sx;
    y_diff = dy - sy;
    if (x_diff < 0)
        x_diff *= -1;
    if (y_diff < 0)
        y_diff *= -1;
}

bool isValidBishopMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    if (x_diff != y_diff)
        return false;

    if (x_diff == 0)
        return false;

    int x_step = (dx - sx) / x_diff;
    int y_step = (dy - sy) / y_diff;

    int crnt_x = sx + x_step;
    int crnt_y = sy + y_step;
    while(crnt_x != dx || crnt_y != dy)
    {
        if (board[crnt_x][crnt_y] != '.')
        {
            return false;
        }

            crnt_x += x_step;
            crnt_y += y_step;
    }

    char d_piece = board[dx][dy];
    if (d_piece == '.')
        return true;

    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidKnightMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    bool l_shape = false;
    if ((x_diff == 2 && y_diff == 1) || (x_diff == 1 && y_diff == 2))
        l_shape = true;

    if (l_shape == false)
        return false;

    char d_piece = board[dx][dy];
    if (d_piece == '.')
        return true;

    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidKingMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    if (x_diff == 0 && y_diff == 0)
        return false;

    if (x_diff > 1 || y_diff > 1)
        return false;

    char d_piece = board[dx][dy];
    if (d_piece == '.')
        return true;

    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidRookMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

    if (x_diff != 0 && y_diff != 0)
        return false;

    if (x_diff == 0 && y_diff == 0)
        return false;

    int x_step = 0;
    int y_step = 0;

    if (x_diff != 0)
        x_step = (dx - sx) / x_diff;
    if (y_diff != 0)
        y_step = (dy - sy) / y_diff;

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
    if (d_piece == '.')
        return true;

    bool own_piece = isOwnPiece(sx, sy, d_piece);
    return own_piece;
}

bool isValidQueenMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    bool flag = (isValidRookMove(board, sx, sy, dx, dy) || isValidBishopMove(board, sx, sy, dx, dy));
    return flag;
}

bool isValidPawnMove(char board[8][8], int sx, int sy, int dx, int dy)
{
    bool white;
    if (board[sx][sy] == 'P')
        white = true;
    else
        white = false;

    int x_step;
    if (white == true)
        x_step = -1;
    else
        x_step = 1;

    int x_diff, y_diff;
    calculateDifference(sx, sy, dx, dy, x_diff, y_diff);

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

        if(board[dx][dy] != '.')
            return false;

        return true;
    }
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

            if(board[dx][dy] != '.')
                return false;

            return true;
        }
    else if (y_diff == 1 && x_diff == 1)
    {
        char d_piece = board[dx][dy];
        if (d_piece == '.')
            return false;

        bool own_piece = isOwnPiece(sx, sy, d_piece);
        return own_piece;
    }

    return false;
}

bool isCheck()
{
    int king_x, king_y;
    bool flag = true;
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

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            char piece = board[i][j];
            bool enemy = false;
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

                if (attack)
                    return true;
            }
        }
    }
    return false;
}

bool isCheckMate()
{
    bool flag = isCheck();
    if (flag == false)
        return false;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            char piece;
            int sx = 0, sy = 0, dx, dy;
            bool valid;
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
                        valid = isValidMove(sx, sy, dx, dy);
                        if (valid == true)
                        {
                            char s_piece = board[sx][sy];
                            char d_piece = board[dx][dy];
                            makeMove(sx, sy, dx, dy);
                            bool check = isCheck();
                            if (check == false)
                            {
                                board[sx][sy] = s_piece;
                                board[dx][dy] = d_piece;
                                return false;
                            }
                            else
                            {
                                board[sx][sy] = s_piece;
                                board[dx][dy] = d_piece;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    initializeBoard();

    while (!gameOver) {
        printBoard();
        cout << (whiteTurn ? "White" : "Black") << "'s turn: ";

        string from, to;
        if (!(cin >> from >> to)) break; // handle EOF

        int fromR, fromC, toR, toC;
        bool ok1 = algebraicToCoords(from, fromR, fromC);
        bool ok2 = algebraicToCoords(to, toR, toC);

        if (!ok1 || !ok2) {
            cout << "Invalid input format. Use e.g. e2 e4 (columns a-h, rows 1-8)." << endl;
            continue;
        }

        if (!isInsideBoard(fromR, fromC) || !isInsideBoard(toR, toC)) {
            cout << "Coordinates out of range." << endl;
            continue;
        }

        if (isValidMove(fromR, fromC, toR, toC)) {
            makeMove(fromR, fromC, toR, toC);
            whiteTurn = !whiteTurn;

            if (isCheck() == true)
                cout << "Your king is under check!";
        }
        else {
            cout << "Invalid move! Try again." << endl;
        }
    }

    cout << "Game ended." << endl;
    return 0;
}

// --------------------
// TODO: Student implements below
// --------------------

bool algebraicToCoords(const string& alg, int& row, int& col) {
    if (alg.length() != 2)
        return false;

    char file = alg[0];
    char rank = alg[1];
    bool flag = false;
    if ((file >= 'a' && file <= 'h') || (file >= 'A' && file <= 'H'))
        flag = true;
    if (flag == false)
        return false;
    if (rank < '1' || rank > '8')
        return false;

    if (file >= 'A' && file <= 'H')
        file += 32;

    col = file - 97;
    row = 8 - (rank - 48);

    return true;
}

// (Other functions left as skeletons for students)
void initializeBoard() {
    // TODO: fill board with starting pieces (uppercase White, lowercase Black),
    // e.g. 'R','N','B','Q','K','B','N','R' on rank 1 and 'P' pawns on rank 2.
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

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = start_pcs[i][j];
        }
    }
}
void printBoard() {
    // TODO: print board with ranks/files labels (a-h across top, 8-1 along side)
    cout << "\n    a b c d e f g h";
    cout << "\n    ---------------\n";
    for (int i = 0; i < SIZE; i++)
    {
        cout << 8 - i << " | ";
        for (int j = 0; j < SIZE; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << "    ---------------\n";
}

bool isValidMove(int sx, int sy, int dx, int dy) {
    // TODO: check piece exists, belongs to player, call piece-specific validators.
    if (board[sx][sy] == '.')
    {
        cout << "There is no piece in this box!";
        return false;
    }

    char piece = board[sx][sy];

    if (whiteTurn == false && (piece >= 'A' && piece <= 'Z'))
    {
        cout << "Black's turn! Not yours";
        return false;
    }
    else if (whiteTurn == true && (piece >= 'a' && piece <= 'z'))
    {
        cout << "White's turn! Not yours";
        return false;
    }

    bool flag = false;
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

    return flag;
}

void makeMove(int sx, int sy, int dx, int dy) {
    // TODO: move piece (capture if necessary), set source to empty.
    board[dx][dy] = board[sx][sy];
    board[sx][sy] = '.';
}
