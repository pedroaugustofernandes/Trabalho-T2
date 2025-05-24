#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "teclado.h"
#include "video.h"
#include <time.h>
#include <unistd.h> 
#include <string.h>

void impMat(int m[4][4]) {
    vid_limpa();
    cor_t texto = {255, 255, 255};
    cor_t fundo = {0, 0, 0};
    cor_t paredes = {200, 120, 255};
    vid_cor_fundo(fundo);
    vid_cor_texto(paredes);
    
    vid_imps("       ");
    vid_imps("\u2554\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2566\u2550\u2550\u2550\u2557\n\r");  
    for (int i = 0; i < 4; i++) {
        if(i > 0) {
            vid_imps("       ");
            vid_imps("\u2560\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u256C\u2550\u2550\u2550\u2563\n\r");
        }
        vid_imps("       ");
        vid_imps("\u2551");
        for (int j = 0; j < 4; j++) {
            if (m[i][j] == 0) {
                vid_cor_texto(texto);
                vid_imps("   ");
            } 
            else if (m[i][j] < 10) {
                vid_cor_texto(texto);
                vid_imps(" ");
                vid_impc('0' + m[i][j]);
                vid_imps(" ");
            } 
            else {
                vid_cor_texto(texto);
                vid_impc('0' + (m[i][j] / 10));
                vid_impc('0' + (m[i][j] % 10));
                vid_imps(" ");
            }
            vid_cor_texto(paredes);
            vid_imps("\u2551");
        }
        vid_imps("\n\r");
    }
    
    vid_imps("       ");
    vid_imps("\u255A\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u2569\u2550\u2550\u2550\u255D\n\r");
    
    vid_atualiza();
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
    vid_imps("\n\r");
}

void jogar(){
    int matriz[4][4];

    tec_inicia();
    vid_inicia();
    
    embaralha(matriz);
    impMat(matriz);
    vid_atualiza();

    while (1){
        vid_imps("       ");
        vid_imps("Escolha para onde ir:");
        vid_imps(" \u2191 CIMA, \u2193 BAIXO, \u2190 ESQUERDA, \u2192 DIREITA\n\r");
        vid_imps("       ");
        vid_imps("END = sair, HOME = reiniciar\n\r");
        vid_atualiza();
        
        tecla_t esc = tec_tecla();
        escolha(esc, matriz);

        if (verificaVitoria(matriz)) {
            vid_imps("\n\rParabéns! Você venceu!\n\r");
            vid_atualiza();
            impMat(matriz);
            break;
        }

        if (esc == T_END) {
            vid_imps("Você saiu do jogo!\n\r");
            vid_atualiza();
            break;
        }
        if (esc == T_HOME) {
            vid_imps("Você reiniciou o jogo!\n\r");
            vid_atualiza();
            sleep(1);
            embaralha(matriz);
            impMat(matriz);
        }
    }
    tec_fim();
    vid_fim();
}

int main(){
    vid_imps("Bem-vindo ao jogo!\n\r");
    sleep(1);
    jogar();
    return 0;
}