#include <stdio.h>
#include <stdlib.h>

#define SIZE 8

char board[SIZE][SIZE] = {
    {' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'},
    {'o', ' ', 'o', ' ', 'o', ' ', 'o', ' '},
    {' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'x', ' ', 'x', ' ', 'x', ' ', 'x', ' '},
    {' ', 'x', ' ', 'x', ' ', 'x', ' ', 'x'},
    {'x', ' ', 'x', ' ', 'x', ' ', 'x', ' '}
};

void clearTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void drawBoard() {
    clearTerminal();
    printf("\n");
    printf("    1   2   3   4   5   6   7   8\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int row = 0; row < SIZE; row++) {
        printf("%d |", row + 1);
        for (int col = 0; col < SIZE; col++) {
            printf(" %c |", board[row][col]);
        }
        printf("\n");
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    1   2   3   4   5   6   7   8\n");
}

int countPieces(char piece) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == piece || board[i][j] == piece - 'x' + 'X' || board[i][j] == piece - 'o' + 'O') {
                count++;
            }
        }
    }
    return count;
}

int isKing(char piece) {
    return piece == 'X' || piece == 'O';
}

int canCapture(int row, int col, int dRow, int dCol, char piece) {
    char enemy = piece == 'x' || piece == 'X' ? 'o' : 'x';
    char enemyKing = piece == 'x' || piece == 'X' ? 'O' : 'X';
    int captureRow = row + dRow;
    int captureCol = col + dCol;
    int landingRow = row + 2 * dRow;
    int landingCol = col + 2 * dCol;

    if (isKing(piece)) {
        while (captureRow >= 0 && captureRow < SIZE && captureCol >= 0 && captureCol < SIZE) {
            if ((board[captureRow][captureCol] == enemy || board[captureRow][captureCol] == enemyKing) &&
                landingRow >= 0 && landingRow < SIZE && landingCol >= 0 && landingCol < SIZE &&
                board[landingRow][landingCol] == ' ') {
                return 1;
            }
            if (board[captureRow][captureCol] != ' ') {
                break;
            }
            captureRow += dRow;
            captureCol += dCol;
            landingRow += dRow;
            landingCol += dCol;
        }
    } else {
        if ((board[captureRow][captureCol] == enemy || board[captureRow][captureCol] == enemyKing) &&
            landingRow >= 0 && landingRow < SIZE && landingCol >= 0 && landingCol < SIZE &&
            board[landingRow][landingCol] == ' ') {
            return 1;
        }
    }

    return 0;
}

int canMoveOrCapture(int row, int col, char piece) {
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (int i = 0; i < 4; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        int dRow = directions[i][0];
        int dCol = directions[i][1];

        if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE && board[newRow][newCol] == ' ') {
            return 1;
        }
        if (canCapture(row, col, dRow, dCol, piece)) {
            return 1;
        }
    }
    return 0;
}

int canAnyPieceCapture(char piece) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == piece || board[i][j] == piece - 'x' + 'X' || board[i][j] == piece - 'o' + 'O') {
                int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
                for (int k = 0; k < 4; k++) {
                    int dRow = directions[k][0];
                    int dCol = directions[k][1];
                    if (canCapture(i, j, dRow, dCol, board[i][j])) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int canPlayerMove(char piece) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == piece || board[i][j] == piece - 'x' + 'X' || board[i][j] == piece - 'o' + 'O') {
                if (canMoveOrCapture(i, j, board[i][j])) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void makeMove(int row, int col, int newRow, int newCol, char piece) {
    board[newRow][newCol] = piece;
    board[row][col] = ' ';
}

void promoteToKing(int row, int col) {
    if (board[row][col] == 'x' && row == 0) {
        board[row][col] = 'X';
    } else if (board[row][col] == 'o' && row == SIZE - 1) {
        board[row][col] = 'O';
    }
}

int continueCapturing(int row, int col, char piece) {
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (int i = 0; i < 4; i++) {
        int dRow = directions[i][0];
        int dCol = directions[i][1];
        if (canCapture(row, col, dRow, dCol, piece)) {
            return 1;
        }
    }
    return 0;
}

void movePiece(int *row, int *col, char piece) {
    int newRow, newCol;
    int capture = 0;

    while (1) {
        printf("Moving piece from %d %d\n", *row + 1, *col + 1);
        printf("Where do you want to move it? \n");

        printf("Row: ");
        while (scanf("%d", &newRow) != 1) {
            printf("Invalid input! Please enter a number for the row: ");
            while (getchar() != '\n'); // clear the input buffer
        }
        printf("Column: ");
        while (scanf("%d", &newCol) != 1) {
            printf("Invalid input! Please enter a number for the column: ");
            while (getchar() != '\n'); // clear the input buffer
        }
        newRow = newRow - 1;
        newCol = newCol - 1;

        int dRow = (newRow - *row) != 0 ? (newRow - *row) / abs(newRow - *row) : 0;
        int dCol = (newCol - *col) != 0 ? (newCol - *col) / abs(newCol - *col) : 0;

        int isKingPiece = isKing(piece);

        if (canCapture(*row, *col, dRow, dCol, piece)) {
            capture = 1;
            int captureRow = *row + dRow;
            int captureCol = *col + dCol;
            int landingRow = *row + 2 * dRow;
            int landingCol = *col + 2 * dCol;

            while (isKingPiece && board[captureRow][captureCol] == ' ') {
                captureRow += dRow;
                captureCol += dCol;
                landingRow += dRow;
                landingCol += dCol;
            }

            char enemyPiece = piece == 'x' || piece == 'X' ? 'o' : 'x';
            char enemyKing = piece == 'x' || piece == 'X' ? 'O' : 'X';
            if (board[captureRow][captureCol] == enemyPiece || board[captureRow][captureCol] == enemyKing) {
                makeMove(*row, *col, landingRow, landingCol, piece);
                board[captureRow][captureCol] = ' ';
                *row = landingRow;
                *col = landingCol;
                if (!isKingPiece) {
                    promoteToKing(*row, *col);
                }
                drawBoard();
                if (!continueCapturing(*row, *col, piece)) {
                    break;
                }
            } else {
                drawBoard();
                printf("Invalid capture move!\n");
            }
        } else if (board[newRow][newCol] == ' ' && (abs(newRow - *row) == 1 && abs(newCol - *col) == 1 || isKingPiece)) {
            if (isKingPiece || abs(newRow - *row) == 1) {
                makeMove(*row, *col, newRow, newCol, piece);
                if (!isKingPiece) {
                    promoteToKing(newRow, newCol);
                }
                *row = newRow;
                *col = newCol;
                break;
            } else {
                drawBoard();
                printf("Invalid move for king!\n");
            }
        } else {
            drawBoard();
            printf("Invalid move!\n");
        }
    }
}

void playerMove(char piece) {
    int row, col;
    int captureAvailable = canAnyPieceCapture(piece);

    while (1) {
        drawBoard();
        printf("Move for %c!\n", piece);
        printf("Choose the piece to move:\n");

        printf("Row: ");
        while (scanf("%d", &row) != 1) {
            printf("Invalid input! Please enter a number for the row: ");
            while (getchar() != '\n'); 
        }
        printf("Column: ");
        while (scanf("%d", &col) != 1) {
            printf("Invalid input! Please enter a number for the column: ");
            while (getchar() != '\n'); 
        }
        row -= 1;
        col -= 1;

        int validPiece = (board[row][col] == piece ||
                          board[row][col] == piece - 'x' + 'X' ||
                          board[row][col] == piece - 'o' + 'O');

        if (!validPiece) {
            drawBoard();
            printf("Invalid piece coordinates!\n");
            continue;
        }

        if (!canMoveOrCapture(row, col, piece)) {
            drawBoard();
            printf("Piece cannot move!\n");
            continue;
        }

        int isKingPiece = isKing(board[row][col]);

        if (captureAvailable &&
            !canCapture(row, col, 1, 1, piece) && 
            !canCapture(row, col, 1, -1, piece) && 
            !canCapture(row, col, -1, 1, piece) && 
            !canCapture(row, col, -1, -1, piece)) {

            if (isKingPiece) {
                int canKingCapture = 0;
                for (int dRow = -1; dRow <= 1; dRow += 2) {
                    for (int dCol = -1; dCol <= 1; dCol += 2) {
                        if (canCapture(row, col, dRow, dCol, piece)) {
                            canKingCapture = 1;
                            break;
                        }
                    }
                    if (canKingCapture) break;
                }
                if (canKingCapture) {
                    drawBoard();
                    printf("You must capture! Choose another piece!\n");
                    continue;
                }
            } else {
                drawBoard();
                printf("You must capture! Choose another piece!\n");
                continue;
            }
        }

        movePiece(&row, &col, board[row][col]);
        break;
    }
}

int main() {
    int x = countPieces('x');
    int o = countPieces('o');
    drawBoard();

    while (x > 0 && o > 0) {
        playerMove('x');
        drawBoard();
        x = countPieces('x');
        o = countPieces('o');

        if (!canPlayerMove('o')) {
            printf("No moves left for player o. It's a draw!\n");
            break;
        }

        if (o == 0) break;

        playerMove('o');
        drawBoard();
        x = countPieces('x');
        o = countPieces('o');

        if (!canPlayerMove('x')) {
            printf("No moves left for player x. It's a draw!\n");
            break;
        }
    }

    if (x == 0) {
        printf("Player o wins!\n");
    } else if (o == 0) {
        printf("Player x wins!\n");
    }

    return 0;
}
