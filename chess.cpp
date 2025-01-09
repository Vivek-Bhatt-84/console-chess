#include <bits/stdc++.h>

using namespace std;


// function to display board 

void setBoard(vector<vector<char>> &board)
{
    cout << "   a   b   c   d   e   f   g   h" << endl;
    cout << " +---+---+---+---+---+---+---+---+ " << endl;

    for (int i = 0; i < 8; i++)
    {
        cout << 8 - i  << "|";
        for (int j = 0; j < 8; j++)
        {
            cout << " " << board[i][j] << " |";
        }
        cout << 8 - i << endl;
        cout << " +---+---+---+---+---+---+---+---+ " << endl;
    }
    cout << "   a   b   c   d   e   f   g   h" << endl;

     
     cout << "P represent white  " << endl; 
     cout << "p represent black " << endl ;  
}


// function to parse position into row and column indices 

pair<int, int> parsePos(string pos)
{

    int row = 8 - (pos[1] - '0');
    int col = pos[0] - 'a';

    return {row, col};
}

// function for pawn movement 

bool isValidPawnMove(int srcRow, int srcCol, int desRow, int desCol, vector<vector<char>> &board, char piece)
{
    // determine direction white move up (+1) , black move down (-1)
    int direction = (piece == 'P') ? -1 : 1;

    //check pawn do not move backward
    if ((piece == 'P' && desRow > srcRow) || (piece == 'p' && desRow < srcRow))
    {
        return false;
    }

    // single starting move
    if (srcCol == desCol && board[desRow][desCol] == '.' && desRow == srcRow + direction)
    {
        return true;
    }

    // double start move
    if (srcCol == desCol && board[desRow][desCol] == '.' && desRow == srcRow + 2 * direction)
    {   
        int forward = srcRow + direction;
        if (board[forward][srcCol] == '.')  
        {
            return true;
        }
    }

    // capture mechznics
    if (abs(desCol - srcCol) == 1 && desRow == srcRow + direction && board[desRow][desCol] != '.' &&
        islower(piece) != islower(board[desRow][desCol]))
    {
        return true;
    }

    return false;
}

// function for knight's movement 
bool isValidKnightMove(int srcRow, int srcCol, int desRow, int desCol)
{
     // l shape movement 
    bool row = abs(srcRow - desRow) == 2 && abs(srcCol - desCol) == 1;
    bool col = abs(srcRow - desRow) == 1 && abs(srcCol - desCol) == 2;

    return row || col;
}

// function move bishop movement 
bool isValidBishopMove(int srcRow, int srcCol, int desRow, int desCol, vector<vector<char>> &board)
{
    // check diagnol movement 
    if (abs(srcRow - desRow) != abs(srcCol - desCol))
    {
        return false;
    }

    // check movement left right +1 and left -1 
    int rowDir = (desRow > srcRow) ? 1 : -1;
    int colDir = (desCol > srcCol) ? 1 : -1;

    int currRow = srcRow + rowDir;
    int currCol = srcCol + colDir;

    // path block logic
    while (currRow != desRow && currCol != desCol)
    {
        if (board[currRow][currCol] != '.')
        {
            return false; // Path is blocked
        }
        currRow += rowDir;
        currCol += colDir;
    }

    return true;
}


// function for rook's movement 

bool isValidRookMove(int srcRow, int srcCol, int desRow, int desCol, vector<vector<char>> &board)
{   
    // check for staright movement
    if (srcRow != desRow && srcCol != desCol)
    {
        return false;
    }

    if (srcRow == desRow)
    { // Horizontal movement 
        int colDir = (desCol > srcCol) ? 1 : -1;
        for (int col = srcCol + colDir; col <= desCol; col += colDir)
        {
            if (board[srcRow][col] != '.')
            {
                return false;
            }
        }
    }

    else
    { // Vertical movement 
        int rowDir = (desRow > srcRow) ? 1 : -1;
        for (int row = srcRow + rowDir; row != desRow; row += rowDir)
        {
            if (board[row][srcCol] != '.')
            {
                return false;
            }
        }
    }

    return true;
}

 // function for queen movement 
bool isValidQueenMove(int srcRow, int srcCol, int desRow, int desCol, vector<vector<char>> &board)
{
    // combination of bishop and rook's movement 
    return isValidRookMove(srcRow, srcCol, desRow, desCol, board) ||
           isValidBishopMove(srcRow, srcCol, desRow, desCol, board);
}


// function for king's movement

bool isValidKingMove(int srcRow, int srcCol, int desRow, int desCol)
{
    // only one move in either 4 direction

    return abs(srcRow - desRow) <= 1 && abs(srcCol - desCol) <= 1;
}

bool legalMove(string move, vector<vector<char>> &board, bool turn)
{
    string src = move.substr(0, 2);
    string dest = move.substr(3, 2);

    // function to parse pos
    pair<int, int> from = parsePos(src);
    pair<int, int> to = parsePos(dest);

    int srcRow = from.first;
    int srcCol = from.second;
    int desRow = to.first;
    int desCol = to.second;

    char piece = board[srcRow][srcCol];

    if (board[srcRow][srcCol] == '.')
    {
        cout << "Invalid Move!!!" << endl;
        return false;
    }

    if (desRow < 0 || desRow >= 8 || desCol < 0 || desCol >= 8)
    {
        cout << "Invalid move: Destination out of bounds!" << endl;
        return false;
    }

    if ((turn && islower(piece)) || (!turn && isupper(piece)))
    {
        cout << "Not your turn!" << endl;
        return false;
    }

    // capture mechanics
    if (islower(piece) == board[desRow][desCol])
    {
        cout << "Invalid capture!!!" << endl;
        return false;
    }

    // validate each piece movement 

    bool valid = false;

    if (tolower(piece) == 'p')
    {
        valid = isValidPawnMove(srcRow, srcCol, desRow, desCol, board, piece);
    }
    else if (tolower(piece) == 'n')
    {
        valid = isValidKnightMove(srcRow, srcCol, desRow, desCol);
    }
    else if (tolower(piece) == 'b')
    {
        valid = isValidBishopMove(srcRow, srcCol, desRow, desCol, board);
    }
    else if (tolower(piece) == 'r')
    {
        valid = isValidRookMove(srcRow, srcCol, desRow, desCol, board);
    }
    else if (tolower(piece) == 'q')
    {
        valid = isValidQueenMove(srcRow, srcCol, desRow, desCol, board);
    }
    else if (tolower(piece) == 'k')
    {
        valid = isValidKingMove(srcRow, srcCol, desRow, desCol);
    }

    // move piece if valid
    if (valid)
    {
        board[desRow][desCol] = board[srcRow][srcCol];
        board[srcRow][srcCol] = '.';
    }

    return valid;
}


// function to find if king in check

bool kingIncheck(int kingRow, int kingCol, vector<vector<char>> &board, bool turn) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            char piece = board[i][j];

            // Skip pieces of the same color
            if (turn && isupper(piece)) continue; // White's turn, skip white pieces
            if (!turn && islower(piece)) continue; // Black's turn, skip black pieces

            bool canAttack = false;
            char lowerPiece = tolower(piece);

            // Pawn attack
            if (lowerPiece == 'p') {
                int direction = (piece == 'P') ? -1 : 1;
                if (abs(j - kingCol) == 1 && i == kingRow + direction) {
                    canAttack = true;
                }
            }

            // Knight attack
            else if (lowerPiece == 'n') {
                canAttack = isValidKnightMove(i, j, kingRow, kingCol);
            }

            // Bishop attack
            else if (lowerPiece == 'b') {
                canAttack = isValidBishopMove(i, j, kingRow, kingCol, board);
            }

            // Rook attack
            else if (lowerPiece == 'r') {
                canAttack = isValidRookMove(i, j, kingRow, kingCol, board);
            }

            // Queen attack
            else if (lowerPiece == 'q') {
                canAttack = isValidQueenMove(i, j, kingRow, kingCol, board);
            }

            // Opponent king attack (1 square in any direction)
            else if (lowerPiece == 'k') {
                canAttack = isValidKingMove(i, j, kingRow, kingCol);
            }

            // If the piece can attack the king, return true
            if (canAttack) {
                cout << "King in Check " << endl ;
                return true;
            }
        }
    }

    return false; // No attack found, king is safe
}

//function for checkomate implementaion

bool checkMate(vector<vector<char>>& board , bool turn){
    int kingR , kingC;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if((turn && board[i][j] == 'K') || (!turn && board[i][j] == 'k')){
                kingR = i ;
                kingC = j ;
                break;
            }
        }
    }

    if(kingIncheck(kingR,kingC,board,turn)){
        return true ;
    }

    return false;
}


int main()
{
    vector<vector<char>> board(8, vector<char>(8, '.'));
    // black pieces
    board[0] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
    board[1] = {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};

    // white pieces
    board[6] = {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};
    board[7] = {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'};

    bool turn = true; // white turn

    while (true)
    {
        setBoard(board);

        // player input
        if(turn){
            cout << "White's turn " << endl ;
        }
        else{
            cout << "Black's turn " << endl ;
        }
        cout << "Enter your move (eg: e2 e4) " << endl;
        string move;
        getline(cin, move);

        if (move == "q")
        {
            cout << "Game Ended!!!" << endl;
            break;
        }

        if (!legalMove(move, board ,turn))
        {
            cout << "Illegal Move !! Try again !!!" << endl;
            continue;   
        }

        if(checkMate(board,turn)){
            setBoard(board);
            cout << " CheckMate !! " ;
            if(turn) cout << " White wins " << endl;
            else cout << " Black Wins " << endl ;
            break;
        }

        turn = !turn;
    }

    return 0;
}
