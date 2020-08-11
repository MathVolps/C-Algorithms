#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    int mean; //variável onde será armazenada a média dos parâmetros
    for (int i = 0; i < width; i++) { //percorrendo a imagem
        for (int j = 0; j < height; j++) {
            mean = round((image[j][i].rgbtRed + image[j][i].rgbtGreen + image[j][i].rgbtBlue)/3.0); //calculando a média de vermelho, azul e verde; e arredondando-a

            //atribuindo o valor da média para todos os parâmetros
            image[j][i].rgbtBlue = mean;
            image[j][i].rgbtGreen = mean;
            image[j][i].rgbtRed = mean;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaBlue;
    int sepiaRed;
    int sepiaGreen;
    for (int i = 0; i < width; i++) { //percorrendo a imagem
        for (int j = 0; j < height; j++) {
            //calculando os novos valores dos parâmetros e limitando-os a 255:
            sepiaBlue = limit(round(0.272 * image[j][i].rgbtRed + 0.534 * image[j][i].rgbtGreen + 0.131 * image[j][i].rgbtBlue));
            sepiaGreen = limit(round(0.349 * image[j][i].rgbtRed + 0.686 * image[j][i].rgbtGreen + 0.168 * image[j][i].rgbtBlue));
            sepiaRed = limit(round(0.393 * image[j][i].rgbtRed + 0.769 * image[j][i].rgbtGreen + 0.189 * image[j][i].rgbtBlue));

            //atribuindo os novos valores aos parâmetros originais:
            image[j][i].rgbtBlue = sepiaBlue;
            image[j][i].rgbtGreen = sepiaGreen;
            image[j][i].rgbtRed = sepiaRed;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int aux[3];
    for (int j = 0; j < height; j++) { //percorrendo a imagem
        for (int i = 0; i < width/2; i++) {
            //armazenando os parâmetros da 1° metade da imagem:
            aux[0] = image[j][i].rgbtBlue;
            aux[1] = image[j][i].rgbtGreen;
            aux[2] = image[j][i].rgbtRed;

            //atribuindo os parâmetros da 2° metade à 1° metade:
            image[j][i].rgbtBlue = image[j][width - i - 1].rgbtBlue;
            image[j][i].rgbtGreen = image[j][width - i - 1].rgbtGreen;
            image[j][i].rgbtRed = image[j][width - i - 1].rgbtRed;

            //atribuindo os parâmetros originais da 1° metade, que foram armazenados no vetor, à 2° metade:
            image[j][width - i - 1].rgbtBlue = aux[0];
            image[j][width - i - 1].rgbtGreen = aux[1];
            image[j][width - i - 1].rgbtRed = aux[2];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sumBlue;
    int sumGreen;
    int sumRed;
    float counter;
    RGBTRIPLE temp[height][width]; //imagem auxiliar 

    for (int i = 0; i < width; i++) { //percorrendo a imagem
        for (int j = 0; j < height; j++) {
            //resetando os valores:
            sumBlue = 0;
            sumGreen = 0;
            sumRed = 0;
            counter = 0.00;

            //realizando a soma dos 8 pixels vizinhos de cada pixel percorrido:
            for (int k = -1; k < 2; k++) {
                for (int h = -1; h < 2; h++) {
                    if (j + k >= 0 && j + k < height && i + h >= 0 && i + h < width) { //o somatório só deve ser feito em pixels válidos, que pertencem à imagem
                        //somatório de parâmetros:
                        sumBlue += image[j + k][i + h].rgbtBlue;
                        sumGreen += image[j + k][i + h].rgbtGreen;
                        sumRed += image[j + k][i + h].rgbtRed;
                        counter++; //contagem da quantidade de pixels 
                    }
                }
            }

            //calculando a média, para cada parâmetro, dos pixels vizinhos:
            temp[j][i].rgbtBlue = round(sumBlue / counter);
            temp[j][i].rgbtGreen = round(sumGreen / counter);
            temp[j][i].rgbtRed = round(sumRed / counter);
        }
    }

    //atribuir os valores da imagem auxiliar à imagem original:
    for (int i = 0; i < width; i++) { //percorrendo a imagem
        for (int j = 0; j < height; j++) {
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
}

//função para limitar o valor de cada parâmetro, sendo, no máximo, 255
int limit(int a) {
    if (a > 255) {
        return 255;
    } else {
        return a;
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //variáveis para se armazenar as somas dos parâmetros de cada pixel em Gx e Gy
    float sumBlueY;
    float sumGreenY;
    float sumRedY;
    float sumBlueX;
    float sumGreenX;
    float sumRedX;
    float modifierX;
    float modifierY;
    RGBTRIPLE temp[height][width]; //imagem auxiliar

    for (int i = 0; i < width; i++) { //percorrendo a imagem
        for (int j = 0; j < height; j++) { 
            //resetando os valores
            sumBlueY = 0.0;
            sumGreenY = 0.0;
            sumRedY = 0.0;
            sumBlueX = 0.0;
            sumGreenX = 0.0;
            sumRedX = 0.0;

            //percorrendo os 8 vizinhos de cada pixel
            for (int k = -1; k < 2; k++) {
                for (int h = -1; h < 2; h++) {
                    if (j + k >= 0 && j + k < height && i + h >= 0 && i + h < width) {
                        //calculand os "pesos" daquela posição de pixel
                        modifierX = (k + 1 * k - k * abs(h));
                        modifierY = (h + 1 * h - h * abs(k));

                        //somatório da multiplicação de cada parâmetro pelo "peso" daquela posção de pixel (Gx)
                        sumBlueX += image[j + k][i + h].rgbtBlue * modifierX;
                        sumGreenX += image[j + k][i + h].rgbtGreen * modifierX;
                        sumRedX += image[j + k][i + h].rgbtRed * modifierX;

                        //somatório da multiplicação de cada parâmetro pelo "peso" daquela posção de pixel (Gy)
                        sumBlueY += image[j + k][i + h].rgbtBlue * modifierY;
                        sumGreenY += image[j + k][i + h].rgbtGreen * modifierY;
                        sumRedY += image[j + k][i + h].rgbtRed * modifierY;
                    }
                }
            }

            //obtendo o valor final de cada parâmetro, por meio da junção de Gx e Gy
            temp[j][i].rgbtBlue = limit(round(sqrt(sumBlueX * sumBlueX + sumBlueY * sumBlueY)));
            temp[j][i].rgbtGreen = limit(round(sqrt(sumGreenX * sumGreenX + sumGreenY * sumGreenY)));
            temp[j][i].rgbtRed = limit(round(sqrt(sumRedX * sumRedX + sumRedY * sumRedY)));
        }
    }

    //atribuindo os valores da imagem auxiliar à imagem original:
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
}