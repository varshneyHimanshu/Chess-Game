#include <bits/stdc++.h>
#include<conio.h>
using namespace std;

class Game;

// this class acts as an interface for all the Pieces
class Pieces
{
public:
    int id;
    int row;
    int col;
    bool captured;
    int color; // black -> 0 and white -> 1;
    int moves;
    int counter;

    virtual bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos) = 0;

    void Move(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);

        board[endPos.first][endPos.second] = board[startPos.first][startPos.second];
        board[startPos.first][startPos.second] = ' ';

        if (map[b] != NULL)
            map[b]->captured = true;

        map[b] = map[a];

        map[b]->row = b[0] - '0';
        map[b]->col = b[1] - '0';

        map[b]->moves++;

        map[a] = NULL;
    }

    bool isCheck(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &white, string &black, int &color)
    {
        // Determine the king's position based on the color
        cout << "Piece ke isCheck pr aaya hu" << endl;
        string kingPosition = (color == 0) ? black : white;
        int kingX = kingPosition[0] - '0';
        int kingY = kingPosition[1] - '0';

        bool kingColor = map[kingPosition]->color;

        // Iterate through all pieces in the map
        for (auto &entry : map)
        {
            Pieces *piece = entry.second;
            if (piece == NULL || piece->captured)
                continue;

            // Check if the piece belongs to the opponent
            if (kingColor != piece->color)
            {

                pair<int, int> startPos = {piece->row, piece->col};
                pair<int, int> endPos = {kingX, kingY};

                if (piece->isValidMove(board, map, white, black, startPos, endPos))
                {
                    return true; // King is in check
                }
            }
        }

        return false; // King is not in check
    }

    virtual bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos) = 0;

    bool isValidMoveForEnPassant(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos, int &color, pair<int, int> &removePos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;


        Pieces *lastPiece = map[to_string(endRow) + to_string(endCol)];
        Pieces *startPiece = map[to_string(startRow) + to_string(startCol)];
        if (lastPiece != NULL && lastPiece->color == startPiece->color)
            return false;

        vector<vector<char>> tempBoard = board;
        unordered_map<string, Pieces *> tempMap = map;
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);
        string c = to_string(startPos.first - 1) + to_string(endPos.second);

        bool btha = true;
        bool ctha = true;
        if (map[c] == NULL)
            ctha = false;
        if (map[b] == NULL)
            btha = false;
        if (map[c] != NULL && map[c]->captured == true)
            ctha = false;
        if (map[b] != NULL && map[b]->captured == true)
            btha = false;

        this->MoveEnPassant(tempBoard, tempMap, startPos, endPos, removePos);
        if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, color))
        {
            map[a]->row = a[0] - '0';
            map[a]->col = a[1] - '0';
            map[a]->moves--;
            if (map[b] != NULL && btha)
                map[b]->captured = false;

            if (map[c] != NULL && ctha)
                map[c]->captured = false;

            return false;
        }

        map[a]->row = a[0] - '0';
        map[a]->col = a[1] - '0';
        map[a]->moves--;
        if (map[b] != NULL && btha)
            map[b]->captured = false;

        if (map[c] != NULL && ctha)
            map[c]->captured = false;

        return true;
    }

    void MoveEnPassant(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, pair<int, int> &startPos, pair<int, int> &endPos, pair<int, int> &removePos)
    {
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);
        string c = to_string(removePos.first) + to_string(removePos.second);

        board[endPos.first][endPos.second] = board[startPos.first][startPos.second];
        board[startPos.first][startPos.second] = ' ';
        board[removePos.first][removePos.second] = ' ';

        if (map[b] != NULL)
            map[b]->captured = true;

        map[c]->captured = true;

        map[b] = map[a];

        map[b]->row = b[0] - '0';
        map[b]->col = b[1] - '0';

        map[b]->moves++;

        map[a] = NULL;
        map[c] = NULL;
    }

    void MoveCastle(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, pair<int, int> &startPos, pair<int, int> &endPos, pair<int, int> &rookPos)
    {
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);
        string c = to_string(rookPos.first) + to_string(rookPos.second);

        string d;
        if (rookPos.second > startPos.second)
        {
            d = to_string(rookPos.first) + to_string(endPos.second - 1);
        }
        else
        {
            d = to_string(rookPos.first) + to_string(endPos.second + 1);
        }

        board[endPos.first][endPos.second] = board[startPos.first][startPos.second];
        board[startPos.first][startPos.second] = ' ';
        board[rookPos.first][d[1] - '0'] = board[rookPos.first][rookPos.second];
        board[rookPos.first][rookPos.second] = ' ';

        cout << a << " " << b << " " << c << " " << d << endl;

        map[b] = map[a];
        map[b]->row = b[0] - '0';
        map[b]->col = b[1] - '0';
        map[a] = NULL;

        map[d] = map[c];
        map[d]->row = d[0] - '0';
        map[d]->col = d[0] - '0';

        map[c] = NULL;

        map[b]->moves++;
        map[d]->moves++;
    }

    virtual vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map) = 0;
};
class Rook : public Pieces
{
public:
    Rook(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }

    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;
        int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // Up, Right, Down, Left

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Check all four directions
        for (auto &direction : directions)
        {
            int newRow = this->row;
            int newCol = this->col;

            // Move in the current direction until hitting a boundary or another piece
            while (true)
            {
                newRow += direction[0];
                newCol += direction[1];

                if (!isInBounds(newRow, newCol))
                {
                    break;
                }

                char destinationPiece = board[newRow][newCol];
                if (destinationPiece == ' ')
                {
                    possibleMoves.push_back({newRow, newCol});
                }
                else
                {
                    // If there's an opponent's piece, add the move and stop in this direction
                    if ((this->color == 1 && islower(destinationPiece)) ||
                        (this->color == 0 && isupper(destinationPiece)))
                    {
                        possibleMoves.push_back({newRow, newCol});
                    }
                    break; // Stop at first piece (whether captured or not)
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Rook can move only horizontally or vertically
        bool isHorizontalMove = startRow == endRow && startCol != endCol;
        bool isVerticalMove = startCol == endCol && startRow != endRow;

        if (!isHorizontalMove && !isVerticalMove)
        {
            return false;
        }

        // Path Check
        if (isHorizontalMove)
        {
            int colDirection = (endCol > startCol) ? 1 : -1;
            int col = startCol + colDirection;
            while (col != endCol)
            {
                if (board[startRow][col] != ' ')
                {
                    return false;
                }
                col += colDirection;
            }
        }
        else if (isVerticalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int row = startRow + rowDirection;
            while (row != endRow)
            {
                if (board[row][startCol] != ' ')
                {
                    return false;
                }
                row += rowDirection;
            }
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        return true;
    }

    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Rook can move only horizontally or vertically
        bool isHorizontalMove = startRow == endRow && startCol != endCol;
        bool isVerticalMove = startCol == endCol && startRow != endRow;

        if (!isHorizontalMove && !isVerticalMove)
        {
            return false;
        }

        // Path Check
        if (isHorizontalMove)
        {
            int colDirection = (endCol > startCol) ? 1 : -1;
            int col = startCol + colDirection;
            while (col != endCol)
            {
                if (board[startRow][col] != ' ')
                {
                    return false;
                }
                col += colDirection;
            }
        }
        else if (isVerticalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int row = startRow + rowDirection;
            while (row != endRow)
            {
                if (board[row][startCol] != ' ')
                {
                    return false;
                }
                row += rowDirection;
            }
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        vector<vector<char>> tempBoard = board;
        unordered_map<string, Pieces *> tempMap = map;
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);

        bool btha = true;
        if (map[b] != NULL && map[b]->captured == true)
            btha = false;

        this->Move(tempBoard, tempMap, startPos, endPos);
        if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
        {
            map[a]->row = a[0] - '0';
            map[a]->col = a[1] - '0';
            map[a]->moves--;
            if (map[b] != NULL && btha)
                map[b]->captured = false;
            return false;
        }

        map[a]->row = a[0] - '0';
        map[a]->col = a[1] - '0';
        map[a]->moves--;
        if (map[b] != NULL && btha)
            map[b]->captured = false;

        return true;
    }
};
class Pawn : public Pieces
{
public:
    Pawn(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }
    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;
        int direction = (this->color == 1) ? -1 : 1; // Determine direction based on pawn color

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Single square forward move
        int newRow = this->row + direction;
        int newCol = this->col;
        if (isInBounds(newRow, newCol) && board[newRow][newCol] == ' ')
        {
            possibleMoves.push_back({newRow, newCol});
        }

        // Double square forward move from the starting position
        newRow = this->row + 2 * direction;
        if (isInBounds(newRow, newCol) && ((this->color == 1 && this->row == 6) || (this->color == 0 && this->row == 1)))
        {
            if (board[this->row + direction][newCol] == ' ' && board[newRow][newCol] == ' ')
            {
                possibleMoves.push_back({newRow, newCol});
            }
        }

        // Diagonal capture moves
        for (int colOffset : {-1, 1})
        {
            newRow = this->row + direction;
            newCol = this->col + colOffset;
            if (isInBounds(newRow, newCol) && board[newRow][newCol] != ' ')
            {
                char destinationPiece = board[newRow][newCol];
                if ((this->color == 1 && islower(destinationPiece)) ||
                    (this->color == 0 && isupper(destinationPiece)))
                {
                    possibleMoves.push_back({newRow, newCol});
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Pawn movement direction (1 for white moving up the board, -1 for black moving down)
        int direction = (this->color == 1) ? -1 : 1;

        // Single square forward move
        if ((endCol == startCol) && (endRow == (startRow + direction)))
        {
            // Check if the destination square is empty
            if (board[endRow][endCol] == ' ')
            {
                return true;
            }
        }

        // Double square forward move from the starting position
        if (endCol == startCol && (endRow == (startRow + 2 * direction)))
        {
            // Ensure pawn is in the starting row
            if ((this->color == 1 && startRow == 6) || (this->color == 0 && startRow == 1))
            {
                // Check if the path and destination square are empty
                if (board[startRow + direction][startCol] == ' ' && board[endRow][endCol] == ' ')
                {
                    return true;
                }
            }
        }

        // Diagonal capture move
        if (abs(endCol == startCol + 1 || endCol == startCol - 1) && endRow == startRow + direction)
        {
            // Check if the destination square contains an opponent's piece
            char destinationPiece = board[endRow][endCol];
            if (destinationPiece != ' ' &&
                ((this->color == 1 && islower(destinationPiece)) ||
                 (this->color == 0 && isupper(destinationPiece))))
            {
                return true;
            }
        }

        return false;
    }

    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Pawn movement direction (1 for white moving up the board, -1 for black moving down)
        int direction = (this->color == 1) ? -1 : 1;

        // Single square forward move
        if ((endCol == startCol) && (endRow == (startRow + direction)))
        {
            // Check if the destination square is empty
            if (board[endRow][endCol] == ' ')
            {
                vector<vector<char>> tempBoard = board;
                unordered_map<string, Pieces *> tempMap = map;
                string a = to_string(startPos.first) + to_string(startPos.second);
                string b = to_string(endPos.first) + to_string(endPos.second);

                bool btha = true;
                if (map[b] != NULL && map[b]->captured == true)
                    btha = false;

                this->Move(tempBoard, tempMap, startPos, endPos);
                if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
                {
                    map[a]->row = a[0] - '0';
                    map[a]->col = a[1] - '0';
                    this->moves--;
                    if (map[b] != NULL && btha)
                        map[b]->captured = false;
                    return false;
                }

                map[a]->row = a[0] - '0';
                map[a]->col = a[1] - '0';
                this->moves--;
                if (map[b] != NULL && btha)
                    map[b]->captured = false;

                return true;
            }
        }

        // Double square forward move from the starting position
        if (endCol == startCol && (endRow == (startRow + 2 * direction)))
        {
            // Ensure pawn is in the starting row
            if ((this->color == 1 && startRow == 6) || (this->color == 0 && startRow == 1))
            {
                // Check if the path and destination square are empty
                if (board[startRow + direction][startCol] == ' ' && board[endRow][endCol] == ' ')
                {
                    vector<vector<char>> tempBoard = board;
                    unordered_map<string, Pieces *> tempMap = map;
                    string a = to_string(startPos.first) + to_string(startPos.second);
                    string b = to_string(endPos.first) + to_string(endPos.second);

                    bool btha = true;
                    if (map[b] != NULL && map[b]->captured == true)
                        btha = false;

                    this->Move(tempBoard, tempMap, startPos, endPos);
                    if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, color))
                    {
                        map[a]->row = a[0] - '0';
                        map[a]->col = a[1] - '0';
                        map[a]->moves--;
                        if (map[b] != NULL && btha)
                            map[b]->captured = false;
                        return false;
                    }

                    map[a]->row = a[0] - '0';
                    map[a]->col = a[1] - '0';
                    map[a]->moves--;
                    if (map[b] != NULL && btha)
                        map[b]->captured = false;

                    return true;
                }
            }
        }

        // Diagonal capture move
        if (abs(endCol == startCol + 1 || endCol == startCol - 1) && endRow == startRow + direction)
        {
            // Check if the destination square contains an opponent's piece
            char destinationPiece = board[endRow][endCol];
            if (destinationPiece != ' ' &&
                ((this->color == 1 && islower(destinationPiece)) ||
                 (this->color == 0 && isupper(destinationPiece))))
            {
                vector<vector<char>> tempBoard = board;
                unordered_map<string, Pieces *> tempMap = map;
                string a = to_string(startPos.first) + to_string(startPos.second);
                string b = to_string(endPos.first) + to_string(endPos.second);

                bool btha = true;
                if (map[b] != NULL && map[b]->captured == true)
                    btha = false;

                this->Move(tempBoard, tempMap, startPos, endPos);
                if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, color))
                {
                    map[a]->row = a[0] - '0';
                    map[a]->col = a[1] - '0';
                    map[a]->moves--;
                    if (map[b] != NULL && btha)
                        map[b]->captured = false;
                    return false;
                }

                map[a]->row = a[0] - '0';
                map[a]->col = a[1] - '0';
                this->moves--;
                if (map[b] != NULL && btha)
                    map[b]->captured = false;

                return true;
            }
        }

        return false;
    }
};
class Knight : public Pieces
{
public:
    Knight(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }
    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;
        int moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}; // All possible knight moves

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Check all possible knight moves
        for (auto &move : moves)
        {
            int newRow = this->row + move[0];
            int newCol = this->col + move[1];

            if (isInBounds(newRow, newCol))
            {
                char destinationPiece = board[newRow][newCol];
                // Add the move if it's empty or contains an opponent's piece
                if (destinationPiece == ' ' ||
                    (this->color == 1 && islower(destinationPiece)) ||
                    (this->color == 0 && isupper(destinationPiece)))
                {
                    possibleMoves.push_back({newRow, newCol});
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos) override
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Possible moves for a knight
        int rowOffsets[] = {2, 2, -2, -2, 1, 1, -1, -1};
        int colOffsets[] = {1, -1, 1, -1, 2, -2, 2, -2};

        for (int i = 0; i < 8; ++i)
        {
            int newRow = startRow + rowOffsets[i];
            int newCol = startCol + colOffsets[i];

            if (newRow == endRow && newCol == endCol)
            {
                char destinationPiece = board[endRow][endCol];

                // Check if the destination square is either empty or contains an opponent's piece
                if (destinationPiece == ' ' ||
                    (this->color == 1 && islower(destinationPiece)) ||
                    (this->color == 0 && isupper(destinationPiece)))
                {
                    return true;
                }
            }
        }

        return false;
    }
    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos) override
    {

        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Possible moves for a knight
        int rowOffsets[] = {2, 2, -2, -2, 1, 1, -1, -1};
        int colOffsets[] = {1, -1, 1, -1, 2, -2, 2, -2};

        for (int i = 0; i < 8; ++i)
        {
            int newRow = startRow + rowOffsets[i];
            int newCol = startCol + colOffsets[i];

            if (newRow == endRow && newCol == endCol)
            {
                char destinationPiece = board[endRow][endCol];

                // Check if the destination square is either empty or contains an opponent's piece
                if (destinationPiece == ' ' ||
                    (this->color == 1 && islower(destinationPiece)) ||
                    (this->color == 0 && isupper(destinationPiece)))
                {
                    vector<vector<char>> tempBoard = board;
                    unordered_map<string, Pieces *> tempMap = map;
                    string a = to_string(startPos.first) + to_string(startPos.second);
                    string b = to_string(endPos.first) + to_string(endPos.second);

                    bool btha = true;
                    if (map[b] != NULL && map[b]->captured == true)
                        btha = false;

                    this->Move(tempBoard, tempMap, startPos, endPos);
                    if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
                    {
                        map[a]->row = a[0] - '0';
                        map[a]->col = a[1] - '0';
                        map[a]->moves--;
                        if (map[b] != NULL && btha)
                            map[b]->captured = false;
                        return false;
                    }

                    map[a]->row = a[0] - '0';
                    map[a]->col = a[1] - '0';
                    map[a]->moves--;
                    if (map[b] != NULL && btha)
                        map[b]->captured = false;

                    return true;
                }
            }
        }

        return false;
    }
};
class Queen : public Pieces
{
public:
    Queen(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }
    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Directions for the Queen's movement: horizontal, vertical, and diagonal
        vector<pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Horizontal and vertical
            {1, 1},
            {1, -1},
            {-1, 1},
            {-1, -1} // Diagonals
        };

        // Check all possible directions
        for (auto &direction : directions)
        {
            int newRow = this->row;
            int newCol = this->col;

            while (true)
            {
                newRow += direction.first;
                newCol += direction.second;

                if (!isInBounds(newRow, newCol))
                {
                    break;
                }

                char destinationPiece = board[newRow][newCol];

                if (destinationPiece == ' ')
                {
                    possibleMoves.push_back({newRow, newCol});
                }
                else
                {
                    if ((this->color == 1 && islower(destinationPiece)) ||
                        (this->color == 0 && isupper(destinationPiece)))
                    {
                        possibleMoves.push_back({newRow, newCol});
                    }
                    break; // The Queen cannot pass through pieces
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Queen can move diagonally, horizontally, or vertically
        bool isDiagonalMove = abs(startRow - endRow) == abs(startCol - endCol);
        bool isHorizontalMove = startRow == endRow && startCol != endCol;
        bool isVerticalMove = startCol == endCol && startRow != endRow;

        if (!isDiagonalMove && !isHorizontalMove && !isVerticalMove)
        {
            return false;
        }

        // Path Check
        if (isDiagonalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int colDirection = (endCol > startCol) ? 1 : -1;
            int row = startRow + rowDirection;
            int col = startCol + colDirection;
            while (row != endRow && col != endCol)
            {
                if (board[row][col] != ' ')
                {
                    return false;
                }
                row += rowDirection;
                col += colDirection;
            }
        }
        else if (isHorizontalMove)
        {
            int colDirection = (endCol > startCol) ? 1 : -1;
            int col = startCol + colDirection;
            while (col != endCol)
            {
                if (board[startRow][col] != ' ')
                {
                    return false;
                }
                col += colDirection;
            }
        }
        else if (isVerticalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int row = startRow + rowDirection;
            while (row != endRow)
            {
                if (board[row][startCol] != ' ')
                {
                    return false;
                }
                row += rowDirection;
            }
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {

                return false;
            }
        }
        return true;
    }

    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Queen can move diagonally, horizontally, or vertically
        bool isDiagonalMove = abs(startRow - endRow) == abs(startCol - endCol);
        bool isHorizontalMove = startRow == endRow && startCol != endCol;
        bool isVerticalMove = startCol == endCol && startRow != endRow;

        if (!isDiagonalMove && !isHorizontalMove && !isVerticalMove)
        {
            return false;
        }

        // Path Check
        if (isDiagonalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int colDirection = (endCol > startCol) ? 1 : -1;
            int row = startRow + rowDirection;
            int col = startCol + colDirection;
            while (row != endRow && col != endCol)
            {
                if (board[row][col] != ' ')
                {
                    return false;
                }
                row += rowDirection;
                col += colDirection;
            }
        }
        else if (isHorizontalMove)
        {
            int colDirection = (endCol > startCol) ? 1 : -1;
            int col = startCol + colDirection;
            while (col != endCol)
            {
                if (board[startRow][col] != ' ')
                {
                    return false;
                }
                col += colDirection;
            }
        }
        else if (isVerticalMove)
        {
            int rowDirection = (endRow > startRow) ? 1 : -1;
            int row = startRow + rowDirection;
            while (row != endRow)
            {
                if (board[row][startCol] != ' ')
                {
                    return false;
                }
                row += rowDirection;
            }
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        vector<vector<char>> tempBoard = board;
        unordered_map<string, Pieces *> tempMap = map;
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);

        bool btha = true;
        if (map[b] != NULL && map[b]->captured == true)
            btha = false;

        this->Move(tempBoard, tempMap, startPos, endPos);
        if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
        {
            map[a]->row = a[0] - '0';
            map[a]->col = a[1] - '0';
            this->moves--;
            if (map[b] != NULL && btha)
                map[b]->captured = false;
            return false;
        }

        map[a]->row = a[0] - '0';
        map[a]->col = a[1] - '0';
        this->moves--;
        if (map[b] != NULL && btha)
            map[b]->captured = false;

        return true;
    }
};
class King : public Pieces
{
public:
    string position;
    King(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }
    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Directions for the King's movement: one square in any direction
        vector<pair<int, int>> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Horizontal and vertical
            {1, 1},
            {1, -1},
            {-1, 1},
            {-1, -1} // Diagonals
        };

        // Check all possible directions
        for (auto &direction : directions)
        {
            int newRow = this->row + direction.first;
            int newCol = this->col + direction.second;

            if (isInBounds(newRow, newCol))
            {
                char destinationPiece = board[newRow][newCol];

                if (destinationPiece == ' ' ||
                    (this->color == 1 && islower(destinationPiece)) ||
                    (this->color == 0 && isupper(destinationPiece)))
                {
                    possibleMoves.push_back({newRow, newCol});
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // King can move one square in any direction
        int rowDiff = abs(endRow - startRow);
        int colDiff = abs(endCol - startCol);

        if (rowDiff > 1 || colDiff > 1)
        {
            return false;
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        return true;
    }
    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // King can move one square in any direction
        int rowDiff = abs(endRow - startRow);
        int colDiff = abs(endCol - startCol);

        if (rowDiff > 1 || colDiff > 1)
        {
            return false;
        }

        // Destination Check
        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && this->color == 0) ||
                                          (islower(destinationPiece) && this->color == 1);
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        vector<vector<char>> tempBoard = board;
        unordered_map<string, Pieces *> tempMap = map;
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);

        bool btha = true;
        if (map[b] != NULL && map[b]->captured == true)
            btha = false;

        this->Move(tempBoard, tempMap, startPos, endPos);
        this->position = b;

        if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
        {
            map[a]->row = a[0] - '0';
            map[a]->col = a[1] - '0';
            this->moves--;
            if (map[b] != NULL && btha)
                map[b]->captured = false;

            this->position = a;
            if (this->color == 1)
            {
                whiteKing = this->position;
            }
            else
                blackKing = this->position;

            return false;
        }

        map[a]->row = a[0] - '0';
        map[a]->col = a[1] - '0';
        this->moves--;
        if (map[b] != NULL && btha)
            map[b]->captured = false;

        this->position = to_string(endRow) + to_string(endCol);
        if (this->color == 1)
        {
            whiteKing = this->position;
        }
        else
            blackKing = this->position;

        return true;
    }
};
class Bishop : public Pieces
{
public:
    Bishop(int id, int row, int col, bool isCaptured, int color)
    {
        this->id = id;
        this->row = row;
        this->col = col;
        this->captured = isCaptured;
        this->color = color;
        this->moves = 0;
        this->counter = 0;
    }

    vector<pair<int, int>> getAllPossibleMoves(vector<vector<char>> &board, unordered_map<string, Pieces *> &map)
    {
        vector<pair<int, int>> possibleMoves;

        // Helper lambda function to check if a position is within the board bounds
        auto isInBounds = [](int row, int col) -> bool
        {
            return (row >= 0 && row < 8 && col >= 0 && col < 8);
        };

        // Directions for the Bishop's movement: diagonals
        vector<pair<int, int>> directions = {
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonals
        };

        // Check all possible directions
        for (auto &direction : directions)
        {
            int newRow = this->row;
            int newCol = this->col;

            while (true)
            {
                newRow += direction.first;
                newCol += direction.second;

                if (!isInBounds(newRow, newCol))
                {
                    break;
                }

                char destinationPiece = board[newRow][newCol];

                if (destinationPiece == ' ')
                {
                    possibleMoves.push_back({newRow, newCol});
                }
                else
                {
                    if ((this->color == 1 && islower(destinationPiece)) ||
                        (this->color == 0 && isupper(destinationPiece)))
                    {
                        possibleMoves.push_back({newRow, newCol});
                    }
                    break; // The Bishop cannot pass through pieces
                }
            }
        }

        return possibleMoves;
    }

    bool isValidMoveForGame(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Bishops move diagonally, so the absolute difference between rows and columns should be equal
        if (abs(startRow - endRow) != abs(startCol - endCol))
        {
            return false;
        }

        int rowDirection = (endRow > startRow) ? 1 : -1;
        int colDirection = (endCol > startCol) ? 1 : -1;

        int row = startRow + rowDirection;
        int col = startCol + colDirection;

        // Check all squares between the start and end positions to make sure they are empty
        while (row != endRow && col != endCol)
        {
            if (board[row][col] != ' ')
            { // Assuming ' ' represents an empty square
                return false;
            }
            row += rowDirection;
            col += colDirection;
        }

        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && (this->color == 0)) ||
                                          (islower(destinationPiece) && (this->color == 1));
            if (!isDestinationSameColor)
            {
                return false;
            }
        }
        // If all conditions are met, the move is valid
        return true;
    }
    bool isValidMove(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string &whiteKing, string &blackKing, pair<int, int> &startPos, pair<int, int> &endPos)
    {
        int startRow = startPos.first;
        int startCol = startPos.second;
        int endRow = endPos.first;
        int endCol = endPos.second;

        // Bishops move diagonally, so the absolute difference between rows and columns should be equal
        if (abs(startRow - endRow) != abs(startCol - endCol))
        {
            return false;
        }

        int rowDirection = (endRow > startRow) ? 1 : -1;
        int colDirection = (endCol > startCol) ? 1 : -1;

        int row = startRow + rowDirection;
        int col = startCol + colDirection;

        // Check all squares between the start and end positions to make sure they are empty
        while (row != endRow && col != endCol)
        {
            if (board[row][col] != ' ')
            { // Assuming ' ' represents an empty square
                return false;
            }
            row += rowDirection;
            col += colDirection;
        }

        char destinationPiece = board[endRow][endCol];
        if (destinationPiece != ' ')
        {
            bool isDestinationSameColor = (isupper(destinationPiece) && (this->color == 0)) ||
                                          (islower(destinationPiece) && (this->color == 1));
            if (!isDestinationSameColor)
            {
                return false;
            }
        }

        vector<vector<char>> tempBoard = board;
        unordered_map<string, Pieces *> tempMap = map;
        string a = to_string(startPos.first) + to_string(startPos.second);
        string b = to_string(endPos.first) + to_string(endPos.second);

        bool btha = true;
        if (map[b] != NULL && map[b]->captured == true)
            btha = false;

        this->Move(tempBoard, tempMap, startPos, endPos);
        if (this->isCheck(tempBoard, tempMap, whiteKing, blackKing, this->color))
        {
            map[a]->row = a[0] - '0';
            map[a]->col = a[1] - '0';
            map[a]->moves--;
            if (map[b] != NULL && btha)
                map[b]->captured = false;
            return false;
        }

        map[a]->row = a[0] - '0';
        map[a]->col = a[1] - '0';
        map[a]->moves--;
        if (map[b] != NULL && btha)
            map[b]->captured = false;

        // If all conditions are met, the move is valid
        return true;
    }
};
class Game
{
public:
    vector<vector<char>> board;
    unordered_map<string, Pieces *> map;

    int CurrentMoveNumber;

    King *whiteKing;
    King *blackKing;

public:
    Game()
    {
        this->CurrentMoveNumber = 1;

        board.resize(8, vector<char>(8, ' '));

        int count = 1; // here count act as an id for the Pieces ;

        // initialising the white pawns
        for (int j = 0; j < 8; j++)
        {
            int i = 6;
            board[i][j] = 'P';
            Pawn *p = new Pawn(count, i, j, false, 1);
            string s = to_string(i) + to_string(j);
            map[s] = p;
            count++;
        }

        // initialising the black pawns
        for (int j = 0; j < 8; j++)
        {
            int i = 1;
            Pawn *p = new Pawn(count, i, j, false, 0);
            board[i][j] = 'p';
            string s = to_string(i) + to_string(j);
            map[s] = p;
            count++;
        }

        // initialising the king
        // white king;
        
        King *k1 = new King(count, 7, 4, false, 1);
        string s = "74";
        board[7][4] = 'K';
        map[s] = k1;
        whiteKing = k1;
        k1->position = s;
        count++;

        // black king
        
        King *k2 = new King(count, 0, 4, false, 0);
        s = "04";
        board[0][4] = 'k';
        blackKing = k2;
        map[s] = k2;
        k2->position = s;
        count++;

        // initialising the queens
        // white queen
        Queen *q1 = new Queen(count, 7, 3, false, 1);
        s = "73";
        board[7][3] = 'Q';
        map[s] = q1;
        count++;

        // black queen
        Queen *q2 = new Queen(count, 0, 3, false, 0);
        s = "03";
        board[0][3] = 'q';
        map[s] = q2;
        count++;

        // iniialising the bishops
        // white ->white movable bishop
        Bishop *b1 = new Bishop(count, 7, 5, false, 1);
        s = "75";
        board[7][5] = 'B';
        map[s] = b1;
        count++;

        Bishop *b2 = new Bishop(count, 7, 2, false, 1);
        s = "72";
        board[7][2] = 'B';
        map[s] = b2;
        count++;

        // black ->white movable bishop
        Bishop *b3 = new Bishop(count, 0, 2, false, 0);
        board[0][2] = 'b';
        s = "02";
        map[s] = b3;
        count++;

        Bishop *b4 = new Bishop(count, 0, 5, false, 0);
        s = "05";
        board[0][5] = 'b';
        map[s] = b4;
        count++;

        // initialising the rooks ->
        // white ->left most Rook
        Rook *r1 = new Rook(count, 7, 0, false, 1);
        s = "70";
        map[s] = r1;
        board[7][0] = 'R';
        count++;

        Rook *r2 = new Rook(count, 7, 7, false, 1);
        s = "77";
        board[7][7] = 'R';
        map[s] = r2;
        count++;

        // black ->left most Rook
        Rook *r3 = new Rook(count, 0, 0, false, 0);
        s = "00";
        board[0][0] = 'r';
        map[s] = r3;
        count++;

        Rook *r4 = new Rook(count, 0, 7, false, 0);
        s = "07";
        board[0][7] = 'r';
        map[s] = r4;
        count++;

        // initialising the Knights->
        // white->left knight
        Knight *kn1 = new Knight(count, 7, 1, false, 1);
        s = "71";
        board[7][1] = 'N';
        map[s] = kn1;
        count++;

        Knight *kn2 = new Knight(count, 7, 6, false, 1);
        s = "76";
        board[7][6] = 'N';
        map[s] = kn2;
        count++;

        // black ->left most Knight
        Knight *k3 = new Knight(count, 0, 1, false, 0);
        s = "01";
        board[0][1] = 'n';
        map[s] = k3;
        count++;

        Knight *k4 = new Knight(count, 0, 6, false, 0);
        s = "06";
        board[0][6] = 'n';
        map[s] = k4;
        count++;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j] == ' ')
                {
                    string a = to_string(i) + to_string(j);
                    map[a] = NULL;
                }
            }
        }
    }

    bool isAppropriateLength(string &move)
    {
        if (move.size() != 5)
            return false;
        return true;
    }

    bool isMoveInsideBoard(string &s)
    {
        if (!isAppropriateLength(s))
            return false;

        int startCol = s[0] - 'a';
        int endCol = s[3] - 'a';

        int startRow = s[1] - '0';
        startRow = 8 - startRow;

        int endRow = s[4] - '0';
        endRow = 8 - endRow;

        if (startRow > 7 || startRow < 0 || startCol > 7 || startCol < 0 || endRow > 7 || endRow < 0 || endCol > 7 || endCol < 0)
            return false;

        return true;
    }

    vector<int> getCoordinates(string &s)
    {
        int startCol = s[0] - 'a';
        int endCol = s[3] - 'a';

        int startRow = s[1] - '0';
        startRow = 8 - startRow;

        int endRow = s[4] - '0';
        endRow = 8 - endRow;

        return {startRow, startCol, endRow, endCol};
    }

    void showBoard()
    {
        system("cls");
        cout << "    a     b     c     d     e     f     g     h  " << endl; // Column labels
        cout << "  -------------------------------------------------" << endl;
        for (int i = 0; i < 8; ++i)
        {
            cout << 8 - i << " |"; // Row label
            for (int j = 0; j < 8; ++j)
            {
                cout << "  " << board[i][j] << "  " << "|";
            }
            cout << " " << 8 - i << endl; // Row label again on the right
            cout << "  -------------------------------------------------" << endl;
        }

        cout << "    a     b     c     d     e     f     g     h  " << endl; // Column labels again at the bottom
    }

    Pieces *getCurrPeice(pair<int, int> &startPos)
    {
        string s = to_string(startPos.first) + to_string(startPos.second);
        return map[s];
    }

    bool isCheck(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, string white, string black, int &color)
    {
        // Determine the king's position based on the color
        string kingPosition = (color == 0) ? black : white;
        int kingX = kingPosition[0] - '0';
        int kingY = kingPosition[1] - '0';

        bool kingsColor = map[kingPosition]->color;

        // Iterate through all pieces in the map
        for (auto &entry : map)
        {
            Pieces *piece = entry.second;
            if (piece == NULL || piece->captured || color == piece->color)
                continue;

            // Check if the piece belongs to the opponent
            if (kingsColor != piece->color)
            {

               
                pair<int, int> startPos = {piece->row, piece->col};
                pair<int, int> endPos = {kingX, kingY};

                // Check if this piece has a valid move to the king's position
                if (piece->isValidMoveForGame(board, map, white, black, startPos, endPos))
                {

                    return true; // King is in check
                }
            }
        }

        return false; // King is not in check
    }

    bool isCheckMate(int color, King *currKing, vector<vector<char>> &board, unordered_map<string, Pieces *> &map, King *whiteKing, King *blackKing)
    {
        // Determine the king's position based on the color

        cout << "isCheckmate me aaya" << endl;

        string kingPosition = to_string(currKing->row) + to_string(currKing->col);
        int kingX = currKing->row;
        int kingY = currKing->col;

        // Check if the King is under attack using the currPiece
        bool temp = currKing->isCheck(board, map, whiteKing->position, blackKing->position, color);
        if (!temp)
        {
            cout << "Normally return hogya " << endl;
            return false; // Not in checkmate if the king is not in check
        }

        cout << "King has Some check" << endl;

        vector<pair<int, int>> validKingMoves = currKing->getAllPossibleMoves(board, map);

        // Check if any move is safe (i.e., not leading to a check)
        for (auto move : validKingMoves)
        {
            vector<vector<char>> tempBoard = board;
            unordered_map<string, Pieces *> tempMap = map;
            pair<int, int> startPos = {kingX, kingY};
            pair<int, int> endPos = {move.first, move.second};

            string b = to_string(move.first) + to_string(move.second);

            bool btha = true;
            if (map[b] == NULL)
                btha = false;
            if (map[b] != NULL && map[b]->captured == false)
                btha = true;

            // Simulate the move
            currKing->Move(tempBoard, tempMap, startPos, endPos);
            currKing->position = b;
            

            if (!this->isCheck(tempBoard, tempMap, whiteKing->position, blackKing->position, color))
            {
                currKing->row = kingX;
                currKing->col = kingY;
                currKing->position = kingPosition;
                currKing->moves--;

                if (map[b] != NULL && btha)
                    map[b]->captured = false;

              
                return false; // King has a valid move, so it's not checkmate
            }

            currKing->row = kingX;
            currKing->col = kingY;
            currKing->position = kingPosition;
            currKing->moves--;

            // again waking up the destination piece ...

            if (map[b] != NULL && btha)
                map[b]->captured = false;
        }

        

        // Check if any other piece can block the check or capture the attacking piece
        for (auto &entry : map)
        {
            Pieces *piece = entry.second;
            if (piece == nullptr || piece->color != color || piece->captured || piece->id == 17 || piece->id == 18)
                continue; // Skip empty squares or opponent's pieces

            vector<pair<int, int>> possibleMoves = piece->getAllPossibleMoves(board, map);
            for (auto move : possibleMoves)
            {
                vector<vector<char>> tempBoard = board;
                unordered_map<string, Pieces *> tempMap = map;

                string originalPos = to_string(piece->row) + to_string(piece->col);

                // Simulate the move
                pair<int, int> startPos = {piece->row, piece->col};
                pair<int, int> endPos = {move.first, move.second};

                string finalDestination = to_string(move.first) + to_string(move.second);

                piece->Move(tempBoard, tempMap, startPos, endPos);

                if (!this->isCheck(tempBoard, tempMap, whiteKing->position, blackKing->position, color))
                {
                    piece->row = originalPos[0] - '0';
                    piece->col = originalPos[1] - '0';
                    piece->moves--;
                    if (map[finalDestination] != NULL)
                        map[finalDestination]->captured = false;
                    return false; // Found a valid move to block the check or capture the attacker
                }

                // Revert the move
                piece->row = originalPos[0] - '0';
                piece->col = originalPos[1] - '0';
                piece->moves--;
                if (map[finalDestination] != NULL)
                    map[finalDestination]->captured = false;
            }
        }

        return true; // If no valid moves or blockages are found, it's a checkmate
    }

    bool isStaleMate(int color, King *currKing, vector<vector<char>> &board, unordered_map<string, Pieces *> &map, King *whiteKing, King *blackKing)
    {
        string kingPosition = to_string(currKing->row) + to_string(currKing->col);

       
        int kingX = currKing->row;
        int kingY = currKing->col;

        // Check if the King is under attack using the currPiece
        bool temp = this->isCheck(board, map, whiteKing->position, blackKing->position, color);
        if (temp)
            return false;

        

        vector<pair<int, int>> validKingMoves = currKing->getAllPossibleMoves(board, map);

        // Check if any move is safe (i.e., not leading to a check)
        for (auto move : validKingMoves)
        {
            vector<vector<char>> tempBoard = board;
            unordered_map<string, Pieces *> tempMap = map;
            pair<int, int> startPos = {kingX, kingY};
            pair<int, int> endPos = {move.first, move.second};

            string b = to_string(move.first) + to_string(move.second);

            bool btha = true;
            if (map[b] == NULL)
                btha = false;
            if (map[b] != NULL && map[b]->captured == false)
                btha = true;

            // Simulate the move
            currKing->Move(tempBoard, tempMap, startPos, endPos);
            currKing->position = b;

            if (!this->isCheck(tempBoard, tempMap, whiteKing->position, blackKing->position, color))
            {
                currKing->row = kingX;
                currKing->col = kingY;
                currKing->position = kingPosition;
                currKing->moves--;

                if (map[b] != NULL && btha)
                    map[b]->captured = false;

                cout << endPos.first << " " << endPos.second << endl;
                return false; // King has a valid move, so it's not checkmate
            }

            currKing->row = kingX;
            currKing->col = kingY;
            currKing->position = kingPosition;
            currKing->moves--;

            // again waking up the destination piece ...
            if (map[b] != NULL && btha)
                map[b]->captured = false;
        }

        

        for (auto &entry : map)
        {
            Pieces *piece = entry.second;
            if (piece == nullptr || piece->color != color || piece->captured || piece->id == 17 || piece->id == 18)
                continue; // Skip empty squares or opponent's pieces

            vector<pair<int, int>> possibleMoves = piece->getAllPossibleMoves(board, map);
            if (possibleMoves.size() != 0)
                return false;
        }

        return true;
    }

    void Promote(vector<vector<char>> &board, unordered_map<string, Pieces *> &map, pair<int, int> &Pos, Pieces *currPiece)
    {

       
        if (((currPiece->id >= 1 && currPiece->id <= 8) && currPiece->color == 1 && Pos.first == 0 && board[Pos.first][Pos.second] == 'P') || (currPiece->id >= 9 && currPiece->id <= 16 && currPiece->color == 0 && Pos.first == 7))
        {
            cout << "Please Enter the Input to Promote the Pawn :" << endl;
            cout << "1. Queen" << endl;
            cout << "2. knight" << endl;
            cout << "3. Bishop" << endl;
            cout << "4. Rook" << endl;

            while (true)
            {
                int move;
                cin >> move;
                if (move == 1)
                {
                    // to queen

                    Queen *newQueen = new Queen(currPiece->id, Pos.first, Pos.second, false, currPiece->color);
                    currPiece->captured = true;
                    if (currPiece->color == 0)
                        board[Pos.first][Pos.second] = 'q';
                    else
                        board[Pos.first][Pos.second] = 'Q';
                    string b = to_string(Pos.first) + to_string(Pos.second);
                    map[b] = newQueen;

                    break;
                }
                else if (move == 2)
                {
                    // to knight
                    Knight *newKnight = new Knight(currPiece->id, Pos.first, Pos.second, false, currPiece->color);
                    currPiece->captured = true;
                    if (currPiece->color == 0)
                        board[Pos.first][Pos.second] = 'n';
                    else
                        board[Pos.first][Pos.second] = 'N';
                    string b = to_string(Pos.first) + to_string(Pos.second);
                    map[b] = newKnight;

                    break;
                }
                else if (move == 3)
                {
                    // to bishop
                    Bishop *newBishop = new Bishop(currPiece->id, Pos.first, Pos.second, false, currPiece->color);
                    currPiece->captured = true;
                    if (currPiece->color == 0)
                        board[Pos.first][Pos.second] = 'b';
                    else
                        board[Pos.first][Pos.second] = 'B';
                    string b = to_string(Pos.first) + to_string(Pos.second);
                    map[b] = newBishop;

                    break;
                }
                else if (move == 4)
                {
                    // to rook
                    Rook *newRook = new Rook(currPiece->id, Pos.first, Pos.second, false, currPiece->color);
                    currPiece->captured = true;
                    if (currPiece->color == 0)
                        board[Pos.first][Pos.second] = 'r';
                    else
                        board[Pos.first][Pos.second] = 'R';
                    string b = to_string(Pos.first) + to_string(Pos.second);
                    map[b] = newRook;
                    break;
                }

                else
                {
                    cout << "please Enter a valid Number:" << endl;
                }
            }
        }
        else
            return;
    }

    bool isDraw()
    {
        // only two king are in the board;
        int activePieces = 0;
        int activeWhiteKnight = 0;
        int activeBlackKnight = 0;
        int activeWhiteBishop = 0;
        int activeBlackBishop = 0;

        int blackPieces = 0;
        int whitePieces = 0;

        for (auto val : this->map)
        {
            if (val.second == NULL)
                continue;
            if (val.second->captured == false)
            {
                activePieces++;
                int x = val.second->row;
                int y = val.second->col;
                if (this->board[x][y] == 'N')
                    activeWhiteKnight++;
                if (this->board[x][y] == 'n')
                    activeBlackKnight++;
                if (this->board[x][y] == 'B')
                    activeWhiteBishop++;
                if (this->board[x][y] == 'b')
                    activeBlackBishop++;

                if (isupper(this->board[x][y]))
                    whitePieces++;
                else
                    blackPieces++;
            }
        }

        if (activePieces == 2)
            return true;
        if (blackPieces == 3)
        {
            if (activeBlackKnight == 2 || (activeBlackKnight == 1 && activeBlackBishop == 1))
                blackPieces = 1;
            else
                blackPieces = 0;
        }

        else if (blackPieces == 4)
        {
            if (activeBlackKnight == 2 && activeBlackBishop == 1)
                blackPieces = 1;
            else
                blackPieces = 0;
        }

        if (whitePieces == 3)
        {
            if (activeWhiteKnight == 2 || (activeWhiteKnight == 1 && activeWhiteBishop == 1))
                whitePieces = 1;
            else
                whitePieces = 0;
        }
        else if (whitePieces == 4)
        {
            if (activeWhiteKnight == 2 && activeWhiteBishop == 1)
                whitePieces = 1;
            else
                whitePieces = 0;
        }
        if (blackPieces == 1 && whitePieces == 1)
            return true;

        return false;
    }
};
int main()
{
    bool whiteTurn = true;
    bool checkMate = false;
    Game g;

    while (!checkMate)
    {
        g.showBoard();
        if (whiteTurn)
        {
            if (g.isDraw())
            {
                cout << "Draw!!!!!!!" << endl;
                checkMate = true;
                continue;
            }
            cout << "Player 1 Turn:" << endl
                 << "Please Enter a Move:";
            string move;
            cin >> move;

            if (g.isMoveInsideBoard(move))
            { // is? move is of appropriate length is also validated inside this function
                vector<int> coordinates = g.getCoordinates(move);
                pair<int, int> startPos = {coordinates[0], coordinates[1]};
                pair<int, int> endPos = {coordinates[2], coordinates[3]};

                cout << g.blackKing->row << " " << g.blackKing->col << endl;
                cout << g.whiteKing->row << " " << g.whiteKing->col << endl;


                Pieces *currPeice = g.getCurrPeice(startPos);
                if (currPeice == NULL)
                {
                    // no Peice at the starting Position...
                    cout << "Play with Pieces Not With Air" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                    continue;
                }

                if (currPeice->color != 1)
                {
                    cout << "INVALID MOVE!! \nIt'sPlayer 1 turn" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                    continue;
                }
               

                if (currPeice->isValidMove(g.board, g.map, g.whiteKing->position, g.blackKing->position, startPos, endPos))
                {
                    
                    currPeice->Move(g.board, g.map, startPos, endPos);

                    currPeice->counter = g.CurrentMoveNumber;
                    g.CurrentMoveNumber++;

                    g.Promote(g.board, g.map, endPos, currPeice);
                    if (g.isCheckMate(0, g.blackKing, g.board, g.map, g.whiteKing, g.blackKing))
                    {
                        cout << "Player 1 Wins!!" << endl;
                        checkMate = true;
                        break;
                    }
                    else if (g.isStaleMate(0, g.blackKing, g.board, g.map, g.whiteKing, g.blackKing))
                    {
                        cout << "It's a Stalemate" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                        break;
                    }

                    whiteTurn = !whiteTurn;
                }

                else if (currPeice->id >= 1 && currPeice->id <= 8 && currPeice->row == 3 && endPos.first == 2)
                {
                   
                    string c = to_string(startPos.first) + to_string(endPos.second);
                    pair<int, int> removePos = {startPos.first, endPos.second};

                    Pieces *removePiece = g.map[c];
                    if (removePiece->id >= 9 && removePiece->id <= 16 && removePiece->moves == 1 && currPeice->isValidMoveForEnPassant(g.board, g.map, g.whiteKing->position, g.blackKing->position, startPos, endPos, currPeice->color, removePos))
                    {
                       
                        int x = removePiece->counter;
                        int y = g.CurrentMoveNumber;
                        if (x + 1 == y)
                        {
                            currPeice->MoveEnPassant(g.board, g.map, startPos, endPos, removePos);
                            if (g.isDraw())
                            {
                                cout << "Draw!!!!!!!" << endl;
                                checkMate = true;
                                continue;
                            }
                            currPeice->counter = g.CurrentMoveNumber;
                            g.CurrentMoveNumber++;
                            whiteTurn = !whiteTurn;
                        }
                        else
                        {

                            // cout << x << " " << y << endl;
                            cout<<"INVALID MOVE!!"<<endl;
                            cout << "You Lost Your Turn To Perform this Move!! Now Move something Different..." << endl;
                            cout << "Press Enter to continue..." << std::endl;
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                            cin.get();
                            continue;
                        }
                    }
                    else
                    {
                        cout << "Better try to en Passant..Better Luck Next Time !!" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                    }
                }

                else if (currPeice->id == 17 && currPeice->row == 7 && currPeice->col == 4 && currPeice->moves == 0)
                {

                    bool isNotCheck = !g.isCheck(g.board, g.map, g.whiteKing->position, g.blackKing->position, currPeice->color);
                  
                    if (isNotCheck && endPos.first == 7 && (endPos.second == 2 || endPos.second == 6))
                    {

                        string b;
                        int direction = 0;
                        if (endPos.second > startPos.second)
                        {
                            b = to_string(endPos.first) + to_string(endPos.second + 1);
                            direction = 1;
                        }
                        else
                        {
                            b = to_string(endPos.first) + to_string(endPos.second - 1);
                            direction = -1;
                        }

                        bool isThereIsNoPieceInBetween = true;
                        King *currKing = NULL;
                        if (currPeice->color == 0)
                        {
                            currKing = g.blackKing;
                        }
                        else
                        {
                            currKing = g.whiteKing;
                        }

                        for (int col = startPos.second + direction; col != endPos.second; col += direction)
                        {
                            if (g.board[startPos.first][col] != ' ')
                            {
                                isThereIsNoPieceInBetween = false;
                                break;
                            }

                            vector<vector<char>> tempBoard = g.board;
                            unordered_map<string, Pieces *> tempMap = g.map;
                            string a = to_string(startPos.first) + to_string(startPos.second);
                            string b = to_string(endPos.first) + to_string(endPos.second);

                            bool btha = true;
                            if (g.map[b] == NULL)
                                btha = false;
                            if (g.map[b] != NULL && g.map[b]->captured == true)
                                btha = false;

                            currKing->Move(tempBoard, tempMap, startPos, endPos);
                            currKing->position = b;
                            if (currKing->isCheck(tempBoard, tempMap, g.whiteKing->position, g.blackKing->position, currPeice->color))
                            {
                                g.map[a]->row = a[0] - '0';
                                g.map[a]->col = a[1] - '0';
                                g.map[a]->moves--;
                                if (g.map[b] != NULL && btha)
                                    g.map[b]->captured = false;

                                currKing->position = a;
                                isThereIsNoPieceInBetween = false;
                                break;
                            }

                            g.map[a]->row = a[0] - '0';
                            g.map[a]->col = a[1] - '0';
                            g.map[a]->moves--;
                            if (g.map[b] != NULL && btha)
                                g.map[b]->captured = false;

                            currKing->position = a;
                        }

                        if (!isThereIsNoPieceInBetween)
                        {
                            cout << "There are Pieces in Between King and Rook !!" << endl;
                            cout << "Press Enter to continue..." << std::endl;
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                            cin.get();
                            continue;
                        }
                        else
                        {

                            pair<int, int> rookPos = {0, 0};
                            Pieces *currHathi = NULL;
                            if (endPos.second > startPos.second)
                            {
                                currHathi = g.map["77"];
                                rookPos.first = 7;
                                rookPos.second = 7;
                            }
                            else
                            {
                                currHathi = g.map["70"];
                                rookPos.first = 7;
                                rookPos.second = 0;
                            }

                            if (currHathi == NULL)
                            {
                                cout << "There is No Rook There" << endl;
                                cout << "INVALID MOVE" << endl;
                                cout << "Press Enter to continue..." << std::endl;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                                cin.get();
                                continue;
                            }

                            if (currHathi->moves == 0)
                            {
                                pair<int, int> finalPosForKing = endPos;
                                currPeice->MoveCastle(g.board, g.map, startPos, finalPosForKing, rookPos);
                                currPeice->counter = g.CurrentMoveNumber;
                                g.CurrentMoveNumber++;
                                currKing->position = to_string(finalPosForKing.first) + to_string(finalPosForKing.second);
                            }
                            else
                            {
                                cout << "Rook move previously" << endl;
                                cout << "INVALID MOVE !!!" << endl;
                                cout << "Press Enter to continue..." << std::endl;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                                cin.get();
                                continue;
                            }
                        }

                        whiteTurn = !whiteTurn;
                    }

                    else
                    {
                        cout << "INVALID MOVE !!!" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                    }
                }
                else
                {
                    cout << "INVALID MOVE!!!" << endl;
                    cout << "Are U New To Chess?? Then first Go and Learn Some Basics" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                }
            }
            else
            {
                cout << "You Only Have a 8X8 board to Play Cause it's a chess not the football" << endl;
                cout << "Move is Outside the Board" << endl;
                cout << "Press Enter to continue..." << std::endl;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                cin.get();
            }

            // g.showBoard();
        }
        else
        {
            // black's turn
            if (g.isDraw())
            {
                cout << "Draw!!!!!!!" << endl;
                checkMate = true;
                continue;
            }
            cout << "Player 2 Turn:" << endl
                 << "Please Enter a Move:";
            string move;
            cin >> move;

            if (g.isMoveInsideBoard(move))
            { // is? move is of appropriate length is also validated inside this function
                vector<int> coordinates = g.getCoordinates(move);
                pair<int, int> startPos = {coordinates[0], coordinates[1]};
                pair<int, int> endPos = {coordinates[2], coordinates[3]};

                Pieces *currPeice = g.getCurrPeice(startPos);
                if (currPeice == NULL)
                {
                    // no Peice at the starting Position...
                    cout << "Play with Pieces Not With Air" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                    continue;
                }
                if (currPeice->color != 0)
                {
                    cout << "INVALID MOVE!! It's Player 2 Turn" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                    continue;
                }

                if (currPeice->isValidMove(g.board, g.map, g.whiteKing->position, g.blackKing->position, startPos, endPos))
                {
                   
                    currPeice->Move(g.board, g.map, startPos, endPos);
                    if (g.isDraw())
                    {
                        cout << "Draw!!!!!!!" << endl;
                        checkMate = true;
                        continue;
                    }
                    currPeice->counter = g.CurrentMoveNumber;
                    g.CurrentMoveNumber++;
                    g.Promote(g.board, g.map, endPos, currPeice);
                    if (g.isCheckMate(1, g.whiteKing, g.board, g.map, g.whiteKing, g.blackKing))
                    {
                        cout << "Player 2 Wins!!" << endl;
                        checkMate = true;
                        break;
                    }
                    else if (g.isStaleMate(1, g.whiteKing, g.board, g.map, g.whiteKing, g.blackKing))
                    {
                        cout << "It's a Stalemate" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                        break;
                    }
                    whiteTurn = !whiteTurn;
                }
                else if (currPeice->id >= 9 && currPeice->id <= 16 && currPeice->row == 4 && endPos.first == 5)
                {
                
                    string c = to_string(startPos.first) + to_string(endPos.second);
                    pair<int, int> removePos = {startPos.first, endPos.second};

                    Pieces *removePiece = g.map[c];
                    if (removePiece->id >= 1 && removePiece->id <= 8 && removePiece->moves == 1 && currPeice->isValidMoveForEnPassant(g.board, g.map, g.whiteKing->position, g.blackKing->position, startPos, endPos, currPeice->color, removePos))
                    {
                        
                        int x = removePiece->counter;
                        int y = g.CurrentMoveNumber;
                        if (x + 1 == y)
                        {
                            currPeice->MoveEnPassant(g.board, g.map, startPos, endPos, removePos);

                            currPeice->counter = g.CurrentMoveNumber;
                            g.CurrentMoveNumber++;

                            whiteTurn = !whiteTurn;
                        }
                    }
                    else
                    {
                        cout << "Better try to en Passant..Better Luck Next Time !!" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                    }
                }
                else if (currPeice->id == 18 && currPeice->row == 0 && currPeice->col == 4 && currPeice->moves == 0)
                {

                    bool isNotCheck = !g.isCheck(g.board, g.map, g.whiteKing->position, g.blackKing->position, currPeice->color);
                    if (isNotCheck && endPos.first == 0 && (endPos.second == 2 || endPos.second == 6))
                    {

                        string b;
                        int direction = 0;
                        if (endPos.second > startPos.second)
                        {
                            b = to_string(endPos.first) + to_string(endPos.second + 1);
                            direction = 1;
                        }
                        else
                        {
                            b = to_string(endPos.first) + to_string(endPos.second - 1);
                            direction = -1;
                        }

                        bool isThereIsNoPieceInBetween = true;
                        King *currKing = NULL;
                        if (currPeice->color == 0)
                        {
                            currKing = g.blackKing;
                        }
                        else
                        {
                            currKing = g.whiteKing;
                        }

                        for (int col = startPos.second + direction; col != endPos.second; col += direction)
                        {
                            if (g.board[startPos.first][col] != ' ')
                            {
                                isThereIsNoPieceInBetween = false;
                                break;
                            }

                            vector<vector<char>> tempBoard = g.board;
                            unordered_map<string, Pieces *> tempMap = g.map;
                            string a = to_string(startPos.first) + to_string(startPos.second);
                            string b = to_string(endPos.first) + to_string(endPos.second);

                            bool btha = true;
                            if (g.map[b] != NULL && g.map[b]->captured == true)
                                btha = false;

                            currKing->Move(tempBoard, tempMap, startPos, endPos);
                            currKing->position = b;
                            if (currKing->isCheck(tempBoard, tempMap, g.whiteKing->position, g.blackKing->position, currPeice->color))
                            {
                                g.map[a]->row = a[0] - '0';
                                g.map[a]->col = a[1] - '0';
                                g.map[a]->moves--;
                                if (g.map[b] != NULL && btha)
                                    g.map[b]->captured = false;

                                currKing->position = a;
                                isThereIsNoPieceInBetween = false;
                                break;
                            }

                            g.map[a]->row = a[0] - '0';
                            g.map[a]->col = a[1] - '0';
                            g.map[a]->moves--;
                            if (g.map[b] != NULL && btha)
                                g.map[b]->captured = false;

                            currKing->position = a;
                        }

                        if (!isThereIsNoPieceInBetween)
                        {
                            cout << "There are Pieces in Between King and Rook !!" << endl;
                            cout << "Press Enter to continue..." << std::endl;
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                            cin.get();
                            continue;
                        }
                        else
                        {

                            pair<int, int> rookPos = {0, 0};
                            Pieces *currHathi = NULL;
                            if (endPos.second > startPos.second)
                            {
                                currHathi = g.map["07"];
                                rookPos.first = 0;
                                rookPos.second = 7;
                            }
                            else
                            {
                                currHathi = g.map["00"];
                                rookPos.first = 0;
                                rookPos.second = 0;
                            }

                            if (currHathi == NULL)
                            {
                                cout << "INVALID MOVE" << endl;
                                cout << "There is NO Rook At the desired Place!!" << endl;
                                cout << "Press Enter to continue..." << std::endl;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                                cin.get();
                                continue;
                            }

                            if (currHathi->moves == 0)
                            {
                                pair<int, int> finalPosForKing = endPos;
                                currPeice->MoveCastle(g.board, g.map, startPos, finalPosForKing, rookPos);
                                currPeice->counter = g.CurrentMoveNumber;
                                g.CurrentMoveNumber++;
                                currKing->position = to_string(finalPosForKing.first) + to_string(finalPosForKing.second);
                            }
                            else
                            {
                                cout << "INVALID MOVE" << endl;
                                cout << "Nice Try!! But Rook Moved Previously..." << endl;
                                cout << "Press Enter to continue..." << std::endl;
                                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                                cin.get();
                            }
                        }

                        whiteTurn = !whiteTurn;
                    }
                    else
                    {
                        cout << "INVALID MOVE !!!" << endl;
                        cout << "Press Enter to continue..." << std::endl;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cin.get();
                    }
                }
                else
                {
                    cout << "INVALID MOVE!!!" << endl;
                    cout << "Are U New To Chess?? Then first Go and Learn Some Basics" << endl;
                    cout << "Press Enter to continue..." << std::endl;
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    cin.get();
                }
            }
            else
            {
                cout<<"INVALID MOVE!!!"<<endl;
                cout << "You Only Have a 8X8 board to Play Cause it's a chess not the football" << endl;
                cout << "Move is Outside the Board" << endl;
                cout << "Press Enter to continue..." << std::endl;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                cin.get();
            }
        }
    }
    g.showBoard();
    cout << "Want to Start New Game ReLaunch It!!!" << endl;
    return 0;
}