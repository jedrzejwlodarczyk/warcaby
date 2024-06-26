#include <stdio.h>
#include <ctype.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>

#define SCREEN_W 800
#define SCREEN_H 840
#define SQUARE_SIZE 100
int continueCapture=0;
char tab[8][8][2] ={
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'X', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'o', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}}};
 /*{
    {{' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}},
    {{'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}},
    {{' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {'o', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}},
    {{' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}},
    {{'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}}
};*/

void drawBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
                al_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, (j + 1) * SQUARE_SIZE, (i + 1) * SQUARE_SIZE, al_map_rgb(255, 255, 255));
            else
                al_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, (j + 1) * SQUARE_SIZE, (i + 1) * SQUARE_SIZE, al_map_rgb(0, 0, 0));

            if (tab[i][j][0] == 'x')
                al_draw_filled_circle(j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5, al_map_rgb(255, 0, 0));
            else if (tab[i][j][0] == 'o')
                al_draw_filled_circle(j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5, al_map_rgb(0, 0, 255));
            else if (tab[i][j][0] == 'X')
                al_draw_filled_circle(j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5, al_map_rgb(137, 0, 0));
            else if (tab[i][j][0] == 'O')
                al_draw_filled_circle(j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 2 - 5, al_map_rgb(0, 0, 137));
        }
    }
}

int isValidMove(char piece, int fromRow, int fromCol, int toRow, int toCol) {
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8)
        return 0; // Destination out of bounds

    if (tab[toRow][toCol][0] != ' ')
        return 0; // Destination is not empty

    if (piece == 'x' || piece == 'X') {
        if (piece == 'x' && toRow == fromRow - 1 && (toCol == fromCol - 1 || toCol == fromCol + 1))
            return 1; // Regular move for x
        else if (piece == 'x' && toRow == fromRow - 2 && (toCol == fromCol - 2 || toCol == fromCol + 2)) {
            // Check if it's a capture move for x
            int enemyRow = (toRow + fromRow) / 2;
            int enemyCol = (toCol + fromCol) / 2;
            if (tab[enemyRow][enemyCol][0] == 'o' || tab[enemyRow][enemyCol][0] == 'O')
                return 1;
         } else if (piece == 'X') {
            // Damka move
            if (abs(toRow - fromRow) == abs(toCol - fromCol)) {
                int rowDirection = (toRow - fromRow) / abs(toRow - fromRow);
                int colDirection = (toCol - fromCol) / abs(toCol - fromCol);
                int i, j;
                bool enemyPieceFound = false;
                int enemyi=-1;
                int enemyj=-1;
                for (i = fromRow + rowDirection, j = fromCol + colDirection; i != toRow && j != toCol; i += rowDirection, j += colDirection) {
                    if (tab[i][j][0] != ' ') {
                        if (!enemyPieceFound && (tab[i][j][0] == 'o' || tab[i][j][0] == 'O')) {
                            enemyPieceFound = true;
                            enemyi=i;
                            enemyj=j;
                        } else {
                            return 0; // Path is blocked or multiple pieces found
                        }
                    }   
                }
                if(toRow == enemyi+rowDirection && toCol == enemyj+colDirection){
                    return 1;
                }
                else{return 0;}    
                }
                return 1; // Valid move
            }
        }
     else if (piece == 'o' || piece == 'O') {
        if (piece == 'o' && toRow == fromRow + 1 && (toCol == fromCol - 1 || toCol == fromCol + 1))
            return 1; // Regular move for o
        else if (piece == 'o' && toRow == fromRow + 2 && (toCol == fromCol - 2 || toCol == fromCol + 2)) {
            // Check if it's a capture move for o
            int enemyRow = (toRow + fromRow) / 2;
            int enemyCol = (toCol + fromCol) / 2;
            if (tab[enemyRow][enemyCol][0] == 'x' || tab[enemyRow][enemyCol][0] == 'X')
                return 1;
             } else if (piece == 'O') {
            // Damka move
            if (abs(toRow - fromRow) == abs(toCol - fromCol)) {
                int rowDirection = (toRow - fromRow) / abs(toRow - fromRow);
                int colDirection = (toCol - fromCol) / abs(toCol - fromCol);
                int i, j;
                bool enemyPieceFound = false;
                int enemyi=-1;
                int enemyj=-1;
                for (i = fromRow + rowDirection, j = fromCol + colDirection; i != toRow && j != toCol; i += rowDirection, j += colDirection) {
                    if (tab[i][j][0] != ' ') {
                        if (!enemyPieceFound && (tab[i][j][0] == 'x' || tab[i][j][0] == 'X')) {
                            enemyPieceFound = true;
                            enemyi=i;
                            enemyj=j;
                        } else {
                            return 0; // Path is blocked or multiple pieces found
                        }
                    }   
                }
                if(toRow == enemyi+rowDirection && toCol == enemyj+colDirection){
                    return 1;
                }
                else{return 0;}    
                }
                return 1; // Valid move
            }
        }
    return 0; // Invalid move
}
int hasCaptureMovesonepiece(char currentPlayer, int row, int col) {
    int directions[4][2] = {
        {-2, -2}, {-2, 2}, {2, -2}, {2, 2}
    };

    for (int i = 0; i < 4; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (isValidMove(currentPlayer, row, col, newRow, newCol)) {
            return 1; // There's at least one capture move
        }
    }

    return 0; // No capture moves
}

int hasCaptureMoves(char currentPlayer) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((currentPlayer == 'x' && (tab[i][j][0] == 'x' || tab[i][j][0] == 'X')) ||
                (currentPlayer == 'o' && (tab[i][j][0] == 'o' || tab[i][j][0] == 'O'))) {
                if (hasCaptureMovesonepiece(currentPlayer, i, j)) {
                    return 1; // At least one piece can capture
                }
            }
        }
    }
    return 0; // No piece can capture
}

int hasValidMoves(char player) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((player == 'x' && (tab[i][j][0] == 'x' || tab[i][j][0] == 'X')) ||
                (player == 'o' && (tab[i][j][0] == 'o' || tab[i][j][0] == 'O'))) {
                if (hasCaptureMovesonepiece(player, i, j) || isValidMove(tab[i][j][0], i, j, i + 1, j - 1) ||
                    isValidMove(tab[i][j][0], i, j, i + 1, j + 1) || isValidMove(tab[i][j][0], i, j, i - 1, j - 1) ||
                    isValidMove(tab[i][j][0], i, j, i - 1, j + 1)) {
                    return 1; // Gracz ma dostępne legalne ruchy
                }
            }
        }
    }
    return 0; // Gracz nie ma dostępnych legalnych ruchów
}

int gameOver(char currentPlayer, char board[8][8][2]) {
    int xCount = 0, oCount = 0;
    char winner;
    // Licz pionki każdego gracza
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j][0] == 'x'|| board[i][j][0]=='X')
                xCount++;
            else if (board[i][j][0] == 'o'||board[i][j][0]=='O')
                oCount++;
        }
    }
    // Gra kończy się, gdy któryś z graczy straci wszystkie pionki lub gdy żaden z graczy nie ma możliwych ruchów
    if (xCount == 0 ){
        winner='o';
        return winner; // Gra się zakończyła
    }
    else if(oCount == 0){
        winner='x';
        return winner;
    }
    else if (!hasValidMoves(currentPlayer)) {
        return 'd'; // Remis
    }
    else{
        winner=' ';
        return winner;}
}

void displayWinner(char winner) {
    ALLEGRO_FONT *font = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (winner == 'x') {
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Red Wins!");
    } else if (winner == 'o') {
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Blue Wins!");
    }
    else if (winner == 'd') {
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Draw!");
    }
    else if(winner ==' '){return;}
    al_flip_display();
    al_rest(3); // Poczekaj 3 sekundy przed zamknięciem
    exit(5);
}

void makemove(char currentPlayer,int selectedRow, int selectedCol, int row, int col){
                    int isCapture = abs(row - selectedRow) > 1;
                    if (isCapture || !hasCaptureMoves(currentPlayer)) {
                        tab[row][col][0] = tab[selectedRow][selectedCol][0];
                        tab[selectedRow][selectedCol][0] = ' ';

                        // Check if it's a capture move and remove the captured piece
                        if (isCapture) {
                            if (abs(selectedRow - row) == 2) {
                                int capturedRow = (row + selectedRow) / 2;
                                int capturedCol = (col + selectedCol) / 2;
                                if (tab[capturedRow][capturedCol][0] != ' ')
                                    tab[capturedRow][capturedCol][0] = ' ';
                            } else {
                                int rowDirection = (selectedRow - row) / abs(selectedRow - row);
                                int colDirection = (selectedCol - col) / abs(selectedCol - col);
                                int i = row + rowDirection;
                                int j = col + colDirection;
                                while (i != selectedRow && j != selectedCol) {
                                    if (tab[i][j][0] != ' ') {
                                        tab[i][j][0] = ' '; // Usunięcie zbitego pionka
                                        break;
                                    }
                                    i += rowDirection;
                                    j += colDirection;
                                }
                            }
                            displayWinner(gameOver(currentPlayer,tab));
                        }

                        // Promote to damka if reached the end of the board
                        if (tab[row][col][0] == 'x' && row == 0)
                            tab[row][col][0] = 'X';
                        else if (tab[row][col][0] == 'o' && row == 7)
                            tab[row][col][0] = 'O';
    }
    return;
}


int main() {
    al_init();
    al_init_primitives_addon();
    al_install_mouse();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    char currentPlayer = 'x';
    int selectedRow = -1;
    int selectedCol = -1;

    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mouseX = ev.mouse.x;
            int mouseY = ev.mouse.y;

            int row = mouseY / SQUARE_SIZE;
            int col = mouseX / SQUARE_SIZE;

            if (selectedRow == -1 && selectedCol == -1) {
                // Select a piece
                if (tab[row][col][0] == currentPlayer || tab[row][col][0] == toupper(currentPlayer)) {
                    selectedRow = row;
                    selectedCol = col;
                }
            } else {
                // Move the selected piece
                if (isValidMove(tab[selectedRow][selectedCol][0], selectedRow, selectedCol, row, col)) {
                    makemove(currentPlayer, selectedRow, selectedCol, row, col);

                    if (abs(row - selectedRow) == 2 || abs(row - selectedRow) > 2) {
                        // Check if there are further capture moves
                        if (hasCaptureMovesonepiece(currentPlayer, row, col)) {
                            continueCapture = 1;
                            selectedRow = row;
                            selectedCol = col;
                        } else {
                            continueCapture = 0;
                            currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
                            selectedRow = -1;
                            selectedCol = -1;
                        }
                    } else {
                        continueCapture = 0;
                        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
                        selectedRow = -1;
                        selectedCol = -1;
                    }
                } else {
                    selectedRow = -1;
                    selectedCol = -1;
                }
            }
        }

        // Rysowanie planszy na ekranie
        drawBoard();
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}