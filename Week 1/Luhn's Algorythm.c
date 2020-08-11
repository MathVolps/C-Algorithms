#include <stdio.h>

void verification (long long int cartao);

//PROGRAMA QUE VALIDA UM NÚMERO DE CARTÃO
int main () {
    long long int cartao, aux;
    cartao = get_long_long("Number: ");

    aux = cartao;
    while (aux != 0) { //conta o n° de dígitos do catão (AMEX = 15, VISA = 13 ou 16 e MASTERCARD = 16)
        contador++;
        aux = aux / 10;
    }

    verification(cartao);
    return 0;
}

void verification (long long int cartao) {
    int num, soma = 0, digito = 0, eh_valido = 0, contador = 0, eh_visa = 0;
    int metade1_amex = 0, metade2_amex = 0, eh_amex = 0, metade1_master = 0, metade2_master = 0, eh_master = 0;

    for (int i = contador - 1; i >= 0; i--) { //loop que valida o cartão, roda de acordo com o n° de dígitos
        num = cartao % 10;
        if (i == 0 && (contador == 13 || contador == 16) && num == 4) //cartões VISA começam com 4
            eh_visa = 1;

        if (i == 0 && contador == 15 && num == 3) //cartões AMEX começam com 3
            metade1_amex = 1;

        if (i == 1 && contador == 15 && (num == 4 || num ==7)) //cartões AMEX começam com 34 ou 37
            metade2_amex = 1;

        if (i == 0 && contador == 16 && num == 5) //cartões MASTERCARD começam com 5
            metade1_master = 1;

        if (i == 1 && contador == 16 && (num == 1 || num == 2 || num == 3 || num == 4 || num == 5)) //cartões MASTERCARD começam com 51, 52, 53, 54 ou 55
            metade2_master = 1;

        if (num != 0) { //se o num não for zero, faz-se as operações necessárias
            if (contador == 13 || contador == 16) { //números a serem multiplicados são os pares
                if (i % 2 == 0 || i == 0) {
                    num = num * 2;
                    if (num >= 10) {
                        while (num != 0) {
                            digito = num % 10;
                            soma += digito;
                            num = num / 10;
                        }
                    } else {
                        soma += num;
                    }
                } else {
                    soma += num;
                }
            } else { //números a serem multiplicados são os ímpares
                if (i % 2 != 0) {
                        num = num * 2;
                        if (num >= 10) {
                            while (num != 0) {
                                digito = num % 10;
                                soma += digito;
                                num = num / 10;
                            }
                        } else {
                            soma += num;
                        }
                    } else {
                        soma += num;
                    }
            }
        } //se o num for zero, somar zero não faz diferença, entao apenas continua-se o loop
        cartao = cartao / 10;
    }

    if (soma % 10 == 0) { //verifica se a soma é de um cartão válido
        eh_valido = 1;
    }

    if (metade1_amex == 1 && metade2_amex == 1) {
        eh_amex = 1;
    }

    if (metade1_master == 1 && metade2_master == 1) {
        eh_master = 1;
    }

    //verificações finais e impressão da resposta
    if (eh_valido == 1 && eh_amex == 1) {
        printf("AMEX\n");
    } else if (eh_valido == 1 && eh_visa == 1) {
        printf("VISA\n");
    } else if (eh_valido == 1 && eh_master == 1) {
        printf("MASTERCARD\n");
    } else {
        printf("INVALID\n");
    }
}
