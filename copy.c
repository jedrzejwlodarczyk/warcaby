void rysujPlansze() {
    clearTerminal();
    printf("\n");
    printf("    1   2   3   4   5   6   7   8\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int a = 0; a < SIZE; a++) {
        printf("%d |", a + 1);
        for (int b = 0; b < SIZE; b++) {
            printf(" %c |", tab[a][b]);
        }
        printf("\n");
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    1   2   3   4   5   6   7   8\n");
}