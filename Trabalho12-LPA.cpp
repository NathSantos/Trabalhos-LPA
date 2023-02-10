#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

typedef struct {
    Point p1, p2;
} bounding_box;

// img  - Imagem
// x, y - Posição da imagem onde a pintura irá começar
// whiteThreshold - Escala de cinza em que qualquer valor maior é considerado "branco"
// bbox - Retângulo que contém os pixels pintados
void floodFill(Mat img, int x, int y, int whiteThreshold, bounding_box *bbox) {

    // Se estiver fora dos limites da imagem, ignora
    if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) return;

    // Acessa o valor do pixel
    uchar pixel = img.at<uchar>(y,x);

    // Se o pixel atual não é considerado "branco", ignora
    if (pixel < whiteThreshold) return;

    // Pinta o pixel para preto
    img.at<uchar>(y,x) = 0;

    // Atualiza o retângulo
    if (x < bbox->p1.x) bbox->p1.x = x;
    if (x > bbox->p2.x) bbox->p2.x = x;
    if (y < bbox->p1.y) bbox->p1.y = y;
    if (y > bbox->p2.y) bbox->p2.y = y;

    // Executa recursivamente para cima, baixo, esquerda e direita
    floodFill(img, x, y+1, whiteThreshold, bbox);
    floodFill(img, x, y-1, whiteThreshold, bbox);
    floodFill(img, x-1, y, whiteThreshold, bbox);
    floodFill(img, x+1, y, whiteThreshold, bbox);
}


int main(int argc, char **argv) {

    // Criando um vetor para salvar as estrelas encontradas. 
    // Substitua max_value pelo valor máximo de estrelas que podem ser encontradas.
    int max_value = 20000
    bounding_box vet[max_value];

    // Abre a imagem, passada pela linha de comando, em modo escala-de-cinza
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);

    printf("Arquivo: %s\n", argv[1]);
    printf("  --> Colunas: %d\n", img.cols);
    printf("  --> Linhas:  %d\n", img.rows);
    printf("  --> Canais:  %d\n", img.channels());

    // Contador que guarda a quantidade de estrelas encontradas
    int cont = 0;

    // Valor do whiteThreshold que deve ser modificado conforme sua preferência
    int whiteThreshold = 80;

    // Para cada pixel em uma linha/coluna da imagem
    for(int j=0; j<img.rows; j++) {
        for (int i=0; i<img.cols; i++) {

            uchar pixel = img.at<uchar>(j,i);

            // Se o pixel for branco (maior que o whiteThreshold)
            if(pixel > whiteThreshold) {

                // Crie um bounding_box 
                bounding_box bbox = {
                    .p1 = Point(img.cols, img.rows),
                    .p2 = Point(0, 0)
                };

                // Execute o flood fill no pixel. 
                // Note que ele irá "apagar" (pintar de preto) a estrela inteira
                floodFill(img, i, j, whiteThreshold, &bbox);

                // Salve o bouding box no vetor de estrelas encontradas
                vet[cont] = bbox;
                cont++;
            }
        }
    }

    printf("  --> Número de estrelas encontradas: %d\n", cont);

    // Abre a imagem, passada pela linha de comando, em modo colorido
    Mat imgOriginal = imread(argv[1], IMREAD_COLOR);

    // Para cada estrela
    for(int i=0; i<cont; i++) {
        // Pinte o retângulo na imagem original
        rectangle(imgOriginal, vet[i].p1, vet[i].p2, Scalar(0, 255, 0), 1);
    }

    // Salve a imagem com os retângulos
    imwrite("output-stars.png", imgOriginal);
}