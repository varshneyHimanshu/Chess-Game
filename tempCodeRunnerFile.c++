void setConsoleColor(int textColor, int backgroundColor) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backgroundColor << 4) | textColor);
    }

    void resetConsoleColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Resets to default
    }

    void showBoard() {
        system("cls");

        cout << "   A  B  C  D  E  F  G  H" << endl;

        for (int i = 0; i < 8; ++i) {
            cout << 8 - i << " "; // Row label on the left
            for (int j = 0; j < 8; ++j) {
                // Alternating background colors for white and black squares
                if ((i + j) % 2 == 0) {
                    setConsoleColor(0, 7); // Black text on white background
                } else {
                    setConsoleColor(7, 0); // White text on black background
                }
                cout << " " << board[i][j] << " ";
                resetConsoleColor(); // Reset to default after each piece
            }
            cout << " " << 8 - i << endl; // Row label on the right
        }

        cout << "   A  B  C  D  E  F  G  H" << endl;
    }