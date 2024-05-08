#include <stdio.h>
#include <stdlib.h>

char tab[8][8] = {
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
    printf("   1 2 3 4 5 6 7 8  \n");
    printf("  -----------------");
    for (int a = 0; a < 8; a++) {
        printf("\n");
        printf("%d |",a+1);
        for (int b = 0; b < 8; b++) {
            printf("%c|",tab[a][b]);
        }
    }
    printf("\n");
    printf("  -----------------\n");
}

int isValidMove(char piece, int fromRow, int fromCol, int toRow, int toCol) {
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8)
        return 0; // Destination out of bounds

    if (tab[toRow][toCol] != ' ')
        return 0; // Destination is not empty

    if (piece == 'x') {
        if (toRow == fromRow - 1 && (toCol == fromCol - 1 || toCol == fromCol + 1))
            return 1; // Regular move for x
        else if (toRow == fromRow - 2 && (toCol == fromCol - 2 || toCol == fromCol + 2)) {
            // Check if it's a capture move for x
            int enemyRow = (toRow + fromRow) / 2;
            int enemyCol = (toCol + fromCol) / 2;
            if (tab[enemyRow][enemyCol] == 'o')
                return 1;
        }
    } else if (piece == 'o') {
        if (toRow == fromRow + 1 && (toCol == fromCol - 1 || toCol == fromCol + 1))
            return 1; // Regular move for o
        else if (toRow == fromRow + 2 && (toCol == fromCol - 2 || toCol == fromCol + 2)) {
            // Check if it's a capture move for o
            int enemyRow = (toRow + fromRow) / 2;
            int enemyCol = (toCol + fromCol) / 2;
            if (tab[enemyRow][enemyCol] == 'x')
                return 1;
        }
    }
    return 0; // Invalid move
}


void movePiece(char piece) {
    int fromRow, fromCol, toRow, toCol;

    while (1) {
        clearTerminal();
        drawBoard(); // Rysuje planszę przed każdym nowym ruchem
        printf("Move %c!\n", piece);
        printf("Select the piece you want to move:\n");
        printf("Row: ");
        scanf("%d", &fromRow);
        printf("Column: ");
        scanf("%d", &fromCol);
        fromRow--;
        fromCol--;

        if (tab[fromRow][fromCol] != piece) {
            printf("Invalid piece selection!\n");
            getchar(); // Czyści bufor wejścia
            getchar(); // Czeka na wciśnięcie klawisza Enter
            continue;
        }

        printf("Where do you want to move it?\n");
        printf("Row: ");
        scanf("%d", &toRow);
        printf("Column: ");
        scanf("%d", &toCol);
        toRow--;
        toCol--;

        if (!isValidMove(piece, fromRow, fromCol, toRow, toCol)) {
            printf("Invalid move!\n");
            getchar(); // Czyści bufor wejścia
            getchar(); // Czeka na wciśnięcie klawisza Enter
            continue;
        }

        tab[toRow][toCol] = piece;
        tab[fromRow][fromCol] = ' ';

        // Check if it's a capture move and remove the captured piece
        if (abs(toRow - fromRow) == 2) {
            int enemyRow = (toRow + fromRow) / 2;
            int enemyCol = (toCol + fromCol) / 2;
            tab[enemyRow][enemyCol] = ' ';

            // Check if there are additional capture moves for the current piece
            int additionalMovePossible = 0;
            int possibleMoves[4][2] = {{toRow + 2, toCol + 2}, {toRow + 2, toCol - 2}, {toRow - 2, toCol + 2}, {toRow - 2, toCol - 2}};
            for (int i = 0; i < 4; i++) {
                int newRow = possibleMoves[i][0];
                int newCol = possibleMoves[i][1];
                if (isValidMove(piece, toRow, toCol, newRow, newCol)) {
                    additionalMovePossible = 1;
                    break;
                }
            }

            if (!additionalMovePossible)
                break; // If no additional move possible, exit the loop
        } else {
            break; // If it's not a capture move, exit the loop
        }
    }
}



int countPieces(char piece) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tab[i][j] == piece) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    int x = 12;
    int o = 12;
    drawBoard();

    while (x > 0 && o > 0) {
        movePiece('x');
        drawBoard();
        x = countPieces('x');
        o = countPieces('o');
        if (o == 0) {
            printf("Player x wins!\n");
            break;
        }
        movePiece('o');
        drawBoard();
        x = countPieces('x');
        o = countPieces('o');
        if (x == 0) {
            printf("Player o wins!\n");
            break;
        }
    }

    return 0;
}
