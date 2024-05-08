#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BOARD_SIZE 8
#define SQUARE_SIZE (SCREEN_WIDTH / BOARD_SIZE)

enum PieceType {
    EMPTY,
    RED,
    BLACK
};

typedef struct {
    enum PieceType type;
    int king;
} Piece;

Piece board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer = RED;
int redPieces = 12;
int blackPieces = 12;

void initialize_board() {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 0 && i < 3)
                board[i][j] = (Piece) {BLACK, 0};
            else if ((i + j) % 2 == 0 && i > 4)
                board[i][j] = (Piece) {RED, 0};
            else
                board[i][j] = (Piece) {EMPTY, 0};
        }
    }
}

void draw_board() {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 0)
                al_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE,
                                         (j + 1) * SQUARE_SIZE, (i + 1) * SQUARE_SIZE,
                                         al_map_rgb(255, 255, 255));
            else
                al_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE,
                                         (j + 1) * SQUARE_SIZE, (i + 1) * SQUARE_SIZE,
                                         al_map_rgb(0, 0, 0));
            if (board[i][j].type != EMPTY) {
                ALLEGRO_COLOR color = (board[i][j].type == RED) ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 0);
                al_draw_filled_circle((j + 0.5) * SQUARE_SIZE, (i + 0.5) * SQUARE_SIZE,
                                      SQUARE_SIZE / 3, color);
                if (board[i][j].king)
                    al_draw_textf(al_load_font("arial.ttf", 20, 0), color,
                                  (j + 0.5) * SQUARE_SIZE, (i + 0.5) * SQUARE_SIZE,
                                  ALLEGRO_ALIGN_CENTER, "K");
            }
        }
    }
}

int is_valid_move(int x1, int y1, int x2, int y2) {
    if (x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE)
        return 0; // Destination out of bounds
    if (board[y2][x2].type != EMPTY)
        return 0; // Destination not empty

    int dx = x2 - x1;
    int dy = y2 - y1;

    if (board[y1][x1].type == RED) {
        if (dy == 1 && (dx == 1 || dx == -1))
            return 1; // Regular move
        if (dy == 2 && (dx == 2 || dx == -2) && board[y1 + dy / 2][x1 + dx / 2].type == BLACK)
            return 2; // Capture move
    } else if (board[y1][x1].type == BLACK) {
        if (dy == -1 && (dx == 1 || dx == -1))
            return 1; // Regular move
        if (dy == -2 && (dx == 2 || dx == -2) && board[y1 + dy / 2][x1 + dx / 2].type == RED)
            return 2; // Capture move
    }

    return 0; // Invalid move
}

void make_move(int x1, int y1, int x2, int y2) {
    if (is_valid_move(x1, y1, x2, y2) == 1) {
        // Regular move
        board[y2][x2] = board[y1][x1];
        board[y1][x1] = (Piece) {EMPTY, 0};
        if ((board[y2][x2].type == RED && y2 == BOARD_SIZE - 1) ||
            (board[y2][x2].type == BLACK && y2 == 0))
            board[y2][x2].king = 1; // King me if reached the opposite side
        currentPlayer = (currentPlayer == RED) ? BLACK : RED;
    } else if (is_valid_move(x1, y1, x2, y2) == 2) {
        // Capture move
        board[y2][x2] = board[y1][x1];
        board[y1][x1] = (Piece) {EMPTY, 0};
        board[y1 + (y2 - y1) / 2][x1 + (x2 - x1) / 2] = (Piece) {EMPTY, 0}; // Remove captured piece
        if ((board[y2][x2].type == RED && y2 == BOARD_SIZE - 1) ||
            (board[y2][x2].type == BLACK && y2 == 0))
            board[y2][x2].king = 1; // King me if reached the opposite side
        if (board[y2][x2].type == RED)
            blackPieces--;
        else
            redPieces--;
        if (blackPieces == 0 || redPieces == 0)
            currentPlayer = EMPTY; // Game over
    }
}

int main() {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;

    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro.\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Failed to create display.\n");
        return -1;
    }

    al_init_primitives_addon();
    al_install_keyboard();

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    initialize_board();

    int selectedPieceX = -1, selectedPieceY = -1;

    while (currentPlayer != EMPTY) {
        while (!al_is_event_queue_empty(event_queue)) {
            al_wait_for_event(event_queue, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    al_destroy_display(display);
                    al_destroy_event_queue(event_queue);
                    return 0;
                }
            } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                int mouseX = ev.mouse.x / SQUARE_SIZE;
                int mouseY = ev.mouse.y / SQUARE_SIZE;
                if (selectedPieceX == -1 && selectedPieceY == -1) {
                    if (board[mouseY][mouseX].type == currentPlayer) {
                        selectedPieceX = mouseX;
                        selectedPieceY = mouseY;
                    }
                } else {
                    make_move(selectedPieceX, selectedPieceY, mouseX, mouseY);
                    selectedPieceX = -1;
                    selectedPieceY = -1;
                }
            }
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));

        draw_board();

        al_flip_display();
        al_rest(0.01);
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
