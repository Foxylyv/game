#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define LARGURA 40
#define ALTURA 20
#define TAMANHO_MAX 100
#define ESPERA_PADRAO 100

typedef struct 
{
    int x[TAMANHO_MAX];
    int y[TAMANHO_MAX];
    int comprimento;
    int direcaox;
    int direcaoy;
} Cobra;

typedef struct {
    int x;
    int y;
} Comida; 

Cobra cobra;
Comida comida;
bool gameOver = FALSE;
int score = 0; //pontuação do jogo

char telaBuffer[ALTURA][LARGURA];

void gerarComida();
void inicializar ();
void desenhar();
void atualizar();//faz a cobra se mover
void processarEntrada ();

//jogo começa aqui
int main () {
    inicializar();
    
    while (!gameOver){
        desenhar();//aguardar uma entrada do jogador
        processarEntrada();
        atualizar(); //atualizar posição
        napms(ESPERA_PADRAO); //esperar entre um loop E outro


    }

        endwin();

    return 0;
}

void inicializar () {
    initscr();//pegar eventos do teclado
    cbreak();//não aguardar o enter
    nodelay(stdscr, TRUE);//serve para nao esperar apertar enter
    noecho();//não ecoar o que escrevo na tela
    keypad(stdscr, TRUE);
    curs_set(0);//não mostrar curso na tela
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    cobra.comprimento = 1;
    cobra.x[0] = LARGURA / 2;
    cobra.y[0] = ALTURA / 2;
    cobra.direcaox = 1;
    cobra.direcaoy = 0;

    gerarComida();

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            if (i == 0 || i == ALTURA - 1 || j ==  0 || j == LARGURA- 1){
                //lógica daposição da cobra
                telaBuffer[i][j] = '#';
            } else {
                telaBuffer[i][j]= ' ';
            }
        }
    }
}

void processarEntrada () {
    int tecla = getch(); //captura qual tecla foi utilizada para se movimentar

    switch (tecla) {     //Cada valor possivel ele lê e quebra como se fosse um if
        case KEY_UP:
        if (cobra.direcaoy == 0) {
            cobra.direcaoy = -1;
            cobra.direcaox = 0;
        }
        break;

        case KEY_DOWN:
        if (cobra.direcaoy == 0){
            cobra.direcaoy = 1;
            cobra.direcaox = 0;
        }
        break;

        case KEY_LEFT:
        if (cobra.direcaox == 0){
            cobra.direcaox = -1;
            cobra.direcaoy = 0;
        }
        break;

        case KEY_RIGHT:
        if (cobra.direcaox == 0){
            cobra.direcaox = 1;
            cobra.direcaoy = 0;
        }
        break;

        case 'x':
        case 'q':
        gameOver = TRUE;
        break;
    }
}

void gerarComida() {
    comida.x = rand() % (LARGURA - 2) + 1;
    comida.y = rand() % (ALTURA - 2) + 1;

}

void desenhar() { //loop: definição; condição de termino; incremento da variavel ate chegar na condição de termino

    for (int i = 1; i < ALTURA - 1; i++) {
        for (int j = 1; j < LARGURA - 1; j++) {
                telaBuffer[i][j]= ' ';
            }
        }

        telaBuffer[comida.y][comida.x] = '$';
    
    for (int i = 0; i < cobra.comprimento; i++) {
        int x = cobra.x[i];
        int y = cobra.y[i];
        telaBuffer[y][x] = '@';
    }

    for (int i = 0; i < ALTURA; i++){
        for (int j = 0; j < LARGURA; j++){
            mvaddch(i, j, telaBuffer[i][j]);
        }
    }

    mvprintw(ALTURA, 0, "Score: %d", score);
    refresh();

}
void atualizar(){
    int novoX = cobra.x[0] + cobra.direcaox;
    int novoY = cobra.y[0] + cobra.direcaoy;

    //detectar colisão com parede
    if (novoX <= 0 || novoX >= LARGURA - 1 || novoY <= 0 || novoY >= ALTURA - 1) {
        gameOver = TRUE;
    }
    //detectar colisão com comida
    if (novoX == comida.x && novoY == comida.y) {
        cobra.comprimento++;
        score += 10;//pontos
        gerarComida();
    }

    for (int i = cobra.comprimento - 1; i > 0; i--){
        cobra.x[i] = cobra.x[i - 1];
        cobra.y[i] = cobra.y[i - 1];
    }

    cobra.x[0] = novoX;
    cobra.y[0] = novoY;
}
