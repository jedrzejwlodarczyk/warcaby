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

char tab[8][8][2] ={
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {'O', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {'o', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
    {{'x', ' '}, {' ', ' '}, {'x', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}, {' ', ' '}},
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
                for (i = fromRow + rowDirection, j = fromCol + colDirection; i != toRow && j != toCol; i += rowDirection, j += colDirection) {
                    if (tab[i][j][0] != ' ') {
                        if (!enemyPieceFound && (tab[i][j][0] == 'o' || tab[i][j][0] == 'O')) {
                            enemyPieceFound = true;
                        } else {
                            return 0; // Path is blocked or multiple pieces found
                        }
                    }
                }
                return 1; // Valid move
            }
        }
    } else if (piece == 'o' || piece == 'O') {
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
                for (i = fromRow + rowDirection, j = fromCol + colDirection; i != toRow && j != toCol; i += rowDirection, j += colDirection) {
                    if (tab[i][j][0] != ' ') {
                        if (!enemyPieceFound && (tab[i][j][0] == 'x' || tab[i][j][0] == 'X')) {
                            enemyPieceFound = true;
                        } else {
                            return 0; // Path is blocked or multiple pieces found
                        }
                    }
                }
                return 1; // Valid move
            }
        }
    }
    return 0; // Invalid move
}

int hasCaptureMoves(char player) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tab[i][j][0] == player || tab[i][j][0] == toupper(player)) {

                if (player == 'x') {
                    if (i - 2 >= 0 && j - 2 >= 0 && (tab[i - 1][j - 1][0] == 'o' || tab[i - 1][j - 1][0] == 'O') && tab[i - 2][j - 2][0] == ' ')
                        return 1;
                    if (i - 2 >= 0 && j + 2 < 8 && (tab[i - 1][j + 1][0] == 'o' || tab[i - 1][j + 1][0] == 'O') && tab[i - 2][j + 2][0] == ' ')
                        return 1;
                } else if (player == 'o') {
                    if (i + 2 < 8 && j - 2 >= 0 && (tab[i + 1][j - 1][0] == 'x' || tab[i + 1][j - 1][0] == 'X') && tab[i + 2][j - 2][0] == ' ')
                        return 1;
                    if (i + 2 < 8 && j + 2 < 8 && (tab[i + 1][j + 1][0] == 'x' || tab[i + 1][j + 1][0] == 'X') && tab[i + 2][j + 2][0] == ' ')
                        return 1;
                }
                if (player == 'X' || player == 'O') {
                    // Damka captures
                    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
                    for (int d = 0; d < 4; d++) {
                        int rowDirection = directions[d][0];
                        int colDirection = directions[d][1];
                        int r = i + rowDirection, c = j + colDirection;
                        while (r + rowDirection >= 0 && r + rowDirection < 8 && c + colDirection >= 0 && c + colDirection < 8) {
                            if ((tab[r][c][0] == 'x' || tab[r][c][0] == 'X' || tab[r][c][0] == 'o' || tab[r][c][0] == 'O') && tab[r + rowDirection][c + colDirection][0] == ' ') {
                                return 1;
                            }
                            if (tab[r][c][0] != ' ')
                                break;
                            r += rowDirection;
                            c += colDirection;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


void moveAndCapture(char piece, int fromRow, int fromCol, int toRow, int toCol) {
    int isCapture = abs(toRow - fromRow) > 1;

    tab[toRow][toCol][0] = tab[fromRow][fromCol][0];
    tab[fromRow][fromCol][0] = ' ';

    // Check if it's a capture move and remove the captured piece
    if (isCapture) {
        if (abs(fromRow - toRow) == 2) {
            int capturedRow = (toRow + fromRow) / 2;
            int capturedCol = (toCol + fromCol) / 2;
            if (tab[capturedRow][capturedCol][0] != ' ')
                tab[capturedRow][capturedCol][0] = ' ';
        } else {
            int rowDirection = (fromRow - toRow) / abs(fromRow - toRow);
            int colDirection = (fromCol - toCol) / abs(fromCol - toCol);
            int i = toRow + rowDirection;
            int j = toCol + colDirection;
            while (i != fromRow && j != fromCol) {
                if (tab[i][j][0] != ' ') {
                    tab[i][j][0] = ' '; // Usunięcie zbitego pionka
                    break;
                }
                i += rowDirection;
                j += colDirection;
            }
        }
    }

    // Promote to damka if reached the end of the board
    if (tab[toRow][toCol][0] == 'x' && toRow == 0)
        tab[toRow][toCol][0] = 'X';
    else if (tab[toRow][toCol][0] == 'o' && toRow == 7)
        tab[toRow][toCol][0] = 'O';
}

int gameOver(char currentPlayer, char board[8][8][2]) {
    int xCount = 0, oCount = 0;

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
        return 1; // Gra się zakończyła
    }
    else if(oCount == 0){
        return 2;
    }
    //else if(!hasValidMoves(currentPlayer, board)){
    //    return 3;
    //}
    else{return 0;}
}

void displayWinner(char winner) {
    ALLEGRO_FONT *font = al_create_builtin_font();
    if (winner == 'x') {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Red Wins!");
    } else if (winner == 'o') {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Blue Wins!");
    }
    else if (winner == 'd') {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2,ALLEGRO_ALIGN_CENTER, "Draw!");
    }
    else if(winner=' '){
        return;
    }
    al_flip_display();
    al_rest(3); // Poczekaj 3 sekundy przed zamknięciem
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
                    int isCapture = abs(row - selectedRow) > 1;

                    if (isCapture || !hasCaptureMoves(currentPlayer)) {
                        moveAndCapture(tab[selectedRow][selectedCol][0], selectedRow, selectedCol, row, col);
                        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
                    }

                    selectedRow = -1;
                    selectedCol = -1;
                } else {
                    selectedRow = -1;
                    selectedCol = -1;
                }
            }
        }
        char winner=' ';
        if(gameOver(currentPlayer,tab)==1){
            winner='x';
        }
        else if(gameOver(currentPlayer,tab)==2){
            winner='o';
            }
        
        // Rysowanie planszy na ekranie
        drawBoard();
        displayWinner(winner);
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
