#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "teclado.h"
#include "video.h"
#include <time.h>
#include <unistd.h> 
#include <string.h>


void impMat(int m[4][4]){
    printf("\n");
    printf("\r");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (m[i][j] == 0) 
                printf("    ");
            else 
                printf("%3d ", m[i][j]);
        }
        printf("\n\r");
    }
}

void iniTab(int m[4][4]){
    int valores[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    srand(time(NULL));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = valores[i][j];
        }
    }
}

void achaZero(int m[4][4], int *i, int *j){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            if (m[x][y] == 0){
                *i = x;
                *j = y;
                return;
            }
        }
    }
}

bool move_baixo(int m[4][4]){
    int i, j;
    achaZero(m, &i, &j);
    if (i == 3){
        printf("Movimento inválido\n\r");
        return false;
    } else {
        int n = m[i+1][j];
        m[i+1][j] = m[i][j];
        m[i][j] = n;
        return true;
    }
}

bool move_esquerda(int m[4][4]){
    int i, j;
    achaZero(m, &i, &j);
    if (j == 0){
        printf("Movimento inválido\n\r");
        return false;
    } else {
        int n = m[i][j-1];
        m[i][j-1] = m[i][j];
        m[i][j] = n;
        return true;
    }
}

bool move_direita(int m[4][4]){
    int i, j;
    achaZero(m, &i, &j);
    if (j == 3){
        printf("Movimento inválido\n\r");
        return false;
    } else {
        int n = m[i][j+1];
        m[i][j+1] = m[i][j];
        m[i][j] = n;
        return true;
    }
}

bool move_cima(int m[4][4]){
    int i, j;
    achaZero(m, &i, &j);
    if (i == 0){
        printf("Movimento inválido\n\r");
        return false;
    } else {
        int n = m[i-1][j];
        m[i-1][j] = m[i][j];
        m[i][j] = n;

        return true;
    }
}



void embaralha(int m[4][4]){
    iniTab(m);
    int movimentos = 200;

    for (int i = 0; i < movimentos; i++) {
        int direcao = rand() % 4; 
        switch (direcao) {
            case 0:
                move_cima(m);
                break;
            case 1:
                move_baixo(m);
                break;
            case 2:
                move_esquerda(m);
                break;
            case 3:
                move_direita(m);
                break;
        }
    }
}


bool verificaVitoria(int m[4][4]){
    int cont = 1;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (i == 3 && j == 3){
                if (m[i][j] == 0) return true;
            } else {
                if (m[i][j] != cont) return false;
                cont++;
            }
        }
    }
    return false;
}



void escolha(int e, int m[4][4]){
    vid_atualiza();
    switch(e) {
        case T_CIMA:
            move_cima(m);
            break;
        case T_BAIXO:
            move_baixo(m);
            break;
        case T_ESQUERDA:
            move_esquerda(m);
            break;
        case T_DIREITA:
            move_direita(m);
            break;
        default:
            break; 
    }
    impMat(m);
    printf("\n\r");
}

//funções que o professor fez pa ler o teclado
static char le_1(){
  char c;
  if (read(0, &c, 1) != 1) return '\0';
  return c;
}

static tecla_t tec_esc(){
  char t2 = le_1();

  if (t2 != '[') return T_ESC;
  char t3 = le_1();
  if (t3 == 'A') return T_CIMA;
  if (t3 == 'B') return T_BAIXO;
  if (t3 == 'C') return T_DIREITA;
  if (t3 == 'D') return T_ESQUERDA;
  if (t3 == 'F') return T_END;
  if (t3 == 'H') return T_HOME;

  // pgup e pgdn enviam 4 códigos!  (<esc><[><5><~> para pgup, por exemplo)
  char t4 = le_1();
  if (t4 != '~') return T_ESC;
  if (t3 == '5') return T_PGUP;
  if (t3 == '6') return T_PGDN;

  // tem várias outras possibilidades, mas essas são suf, pelo menos por enquanto
  return T_ESC;
}

tecla_t tec_tecla(){
  tecla_t tec = le_1();
  if (tec == 127) tec = T_BS;
  else if (tec == '\r') tec = T_ENTER;
  else if (tec == T_ESC) {
    tec = tec_esc();
  }
  return tec;
}

// funções de video feitas pelo professor
#define NS_POR_TELA 50000000
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

void vid_inicia(){
  printf("\033[?1049h");   // seleciona a tela alternativa
  printf("\033[?25l");     // esconde o cursor
  printf("\033[2J");       // limpa a tela
  printf("\033[H");        // põe o cursor no topo
  printf("\n");            // avança a linha (e força a saída)
}

void vid_fim(){
  printf("\033[?25h");     // mostra o cursor
  printf("\033[?1049l");   // seleciona a tela normal
  printf("\n");            // avança a linha (e força a saída)
}

static void erro_brabo(char *msg){
  tec_fim();
  vid_fim();
  printf("Erro brabo: %s\n", msg);
  exit(5);
}

// local onde guardar os bytes antes de enviar para o SO em uma tacada.
// declarados static para não serem visíveis fora deste arquivo.
// variáveis globais, mas confinadas às poucas funções abaixo.
#define NCHARS 10000
static char chars[NCHARS];
static int nchars = 0;

static void vid_flush(){
  if (nchars == 0) return;
  if (nchars > 10000) erro_brabo("excesso de caracteres no buffer");
  write(1, chars, nchars);
  nchars = 0;
}

// envia todos os caracteres guardados para o SO, na frequencia esperada
void vid_atualiza(){
  // timespec é um registro para o tempo em alta resolução.
  //   tem dois campos, com os segundos e os nanosegundos do horário
  // pega o horário atual e calcula se já passou tempo suficiente
  //   desde a última vez que atualizou a tela
  static struct timespec ultimo = { 0, 0 };
  struct timespec agora;
  clock_gettime(CLOCK_REALTIME, &agora);
  long diff_s = agora.tv_sec - ultimo.tv_sec;
  if (diff_s >= 0 && diff_s < 2) {
    long diff_ns = diff_s * 1000000000
                 + (agora.tv_nsec - ultimo.tv_nsec);
    if (diff_ns < NS_POR_TELA) {
      // faz muito pouco tempo que atualizou a tela, espera um pouco
      struct timespec espera;
      espera.tv_sec = 0;
      espera.tv_nsec = NS_POR_TELA - diff_ns;
      nanosleep(&espera, NULL);
    }
  }
  ultimo = agora;

  vid_flush();
}

void vid_impcs(char *s, int tam)
{
  while (tam > 0) {
    int espaco = NCHARS - nchars;
    if (espaco == 0) {
      vid_flush();
      continue;
    }
    int n = tam;
    if (n > espaco) n = espaco;
    memcpy(chars + nchars, s, n);
    nchars += n;
    s += n;
    tam -= n;
  }
}

void vid_limpa(){
  vid_impcs("\033[2J\033[H", 7);
}

void jogar(){
    int matriz[4][4];

    tec_inicia();
    embaralha(matriz);
    impMat(matriz);

    while (1){
        printf("Escolha para onde ir: ");
        tecla_t esc = tec_tecla();
        escolha(esc, matriz);

        if (verificaVitoria(matriz)) {
            printf("\n\rParabéns! Você venceu!\n\r");
            impMat(matriz);
            break;
        }

        if (esc == T_END) {
            printf("Você saiu do jogo!\n\r");
            break;
        }
        if (esc == T_HOME) {
            printf("Você reiniciou o jogo!\n\r");
            embaralha(matriz);
            impMat(matriz);
        }
    }
    tec_fim();

}

int main(){
    printf("Bem-vindo ao jogo!\n\r");
    sleep(1);
    jogar();
    return 0;
}