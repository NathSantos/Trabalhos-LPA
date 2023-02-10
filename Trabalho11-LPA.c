// ======================================== DÉCIMO PRIMEIRO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <GL/glut.h>

int largura;
int altura;
int quant_seg;

typedef struct coordenada_seg {
    int x;
    int y;
}coordenada_seg;

typedef struct segmento {
    coordenada_seg seg_A;
    coordenada_seg seg_B;
}segmento;

segmento *segm;

int ccw(coordenada_seg a, coordenada_seg b, coordenada_seg c) {
    return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
}

int intersect(coordenada_seg a, coordenada_seg b, coordenada_seg c, coordenada_seg d) {
    return ccw(a,c,d) != ccw(b,c,d) && ccw(a,b,c) != ccw(a,b,d);
}

int verifica_interseccao(segmento segmento, int quantidade){
    int i;

    for(i = 0; i < quantidade; i++){
        if(intersect(segmento.seg_A, segmento.seg_B, segm[i].seg_A, segm[i].seg_B) == 1){
            return 0;
        }
    }
    return 1;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0); // Seta a cor do seg. (Red, Green, Blue, entre 0.0 e 1.0)
    glBegin(GL_LINES); // Indica que um segmento será iniciado

    segmento prox_segm;
    int i = 0;

    while(i < quant_seg) {

        prox_segm.seg_A.x = rand() % largura;
        prox_segm.seg_A.y = rand() % altura;
        prox_segm.seg_B.x = rand() % largura;
        prox_segm.seg_B.y = rand() % altura;

        bool flag = true;

        for(int j = 0; j < i; j++) {
            if(intersect(prox_segm.seg_A, prox_segm.seg_B, segm[j].seg_A, segm[j].seg_B) == 1) {
                flag = false;
            }
        }

        if(flag) {
            glVertex2f(prox_segm.seg_A.x, prox_segm.seg_A.y);
            glVertex2f(prox_segm.seg_B.x, prox_segm.seg_B.y);
            segm[i] = prox_segm;
            i++;
        }
    }

    glEnd(); // Finaliza a criação do segmento
    glFlush();

}

// a largura da janela (argv[1]), a altura da janela (argv[2]) e uma
// quantidade de segmentos (argv[3]). 
void main(int argc, char *argv[]) {
    srand(time(NULL));

    largura = atoi(argv[1]);
    altura = atoi(argv[2]);
    quant_seg = atoi(argv[3]);
    segm = (segmento*)malloc(sizeof(segmento)*quant_seg);

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
    // Cria uma janela de tamanho “largura” x “altura”
    glutInitWindowSize(largura,altura);
    glutCreateWindow ("Segmentos Aleatorios");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, largura, 0.0, altura, -1.0, 1.0);
    // Seta a cor do fundo da janela
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Seta a função “display” como sendo a função que irá pintar a janela (infinitamente)
    glutDisplayFunc(display);
    glutMainLoop();
}