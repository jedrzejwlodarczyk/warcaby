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

void rysujPlansze() {
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


void ruchX(){
    int wiersz,kolumna;
    wyborPionka:
        printf("Ruch x!\n");
        printf("Wybierz pionek, ktorym chcesz wykonac ruch: \n");
        printf("Wiersz: ");
        scanf("%d",&wiersz);
        printf("Kolumna: ");
        scanf("%d",&kolumna);
        wiersz=wiersz-1;
        kolumna=kolumna-1;

    if(tab[wiersz][kolumna] != 'x'){
        printf("Podano bledne wspolrzedne pionka!\n");
        clearTerminal();
        rysujPlansze();
        goto wyborPionka;
    }
    else{
        int w1,k1;
        int bicie = 0;

        printf("Gdzie chcesz go przesunac? \n");
        printf("Wiersz: ");
        scanf("%d",&w1);
        printf("Kolumna: ");
        scanf("%d",&k1);
        w1=w1-1;
        k1=k1-1;
        if((tab[wiersz-1][kolumna+1]=='o' || tab[wiersz-1][kolumna-1]=='o') && (tab[wiersz-2][kolumna+2]==' ' || tab[wiersz-2][kolumna-2] == ' ')){
            bicie = 1;
            printf("bicie \n");
        }
        else{
            bicie = 0;
        }

        if(tab[w1][k1] == ' ' && (k1==kolumna+1 || k1==kolumna-1) && w1==wiersz-1 && w1>=0 && w1<8 && k1>=0 && k1<8 && bicie == 0){
            tab[w1][k1]='x';
            tab[wiersz][kolumna]=' ';
        }
        else{
            clearTerminal();
            rysujPlansze();
            printf("Nieprawidlowy ruch!\n");
            goto wyborPionka;
        }
    }
}

void ruchY(){
    int wiersz,kolumna;
    wyborPionka:
        printf("Ruch o!\n");
        printf("Wybierz pionek, ktorym chcesz wykonac ruch: \n");
        printf("Wiersz: ");
        scanf("%d",&wiersz);
        printf("Kolumna: ");
        scanf("%d",&kolumna);
        wiersz=wiersz-1;
        kolumna=kolumna-1;

    if(tab[wiersz][kolumna] != 'o'){
        printf("Podano bledne wspolrzedne pionka!\n");
        goto wyborPionka;
    }
    else{
        int w1,k1;

        printf("Gdzie chcesz go przesunac? \n");
        printf("Wiersz: ");
        scanf("%d",&w1);
        printf("Kolumna: ");
        scanf("%d",&k1);
        w1=w1-1;
        k1=k1-1;
        if(tab[w1][k1] == ' ' && (k1==kolumna+1 || k1==kolumna-1) && w1==wiersz+1 && w1>=0 && w1<8 && k1>=0 && k1<8){
            tab[w1][k1]='o';
            tab[wiersz][kolumna]=' ';
        }
        else{
            clearTerminal();
            rysujPlansze();
            printf("Nieprawidlowy ruch!\n");
            goto wyborPionka;
        }
    }

}
int countcheckers(char piece){
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





int main()
{
    int x = 12;
    int o = 12;
    rysujPlansze();

    while(x>0 || o>0){
        ruchX();
        rysujPlansze();
        ruchY();
        rysujPlansze();
    }

}
