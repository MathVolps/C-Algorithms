#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int comand_verification(int argc, char **argv);
void encrypting(string plaintext, int n);

int main(int argc, char *argv[]) {

    if (comand_verification(argc, argv) == 1) {
        return 1;
    }

    int n = atoi(argv[1]); //n = key
    string plaintext = get_string("plaintext: ");
    encrypting(plaintext, n);

    printf("ciphertext: %s\n", plaintext);
    return 0;
}

// Verifica se a linha de comando está adequada
int comand_verification(int argc, char **argv) {
    if (argc == 2) { // argv deve ser igual a 2 
        for (int i = 0; argv[1][i] != '\0'; i++) { // percorre argv
            int aux = argv[1][i];
            if (isdigit(aux) == 0) { // verifica se todos os elmentos de argv são dígitos 
                printf("Usage: ./caesar key\n");
                return 1 ;
            }
        }

    } else { // caso contrário, está errado
        printf("Usage: ./caesar key\n");
        return 1;
    }

    return 0;
}

// Codifica (encripta) a mensagem original
void encrypting(string plaintext, int n) {
    for (int j = 0; plaintext[j] != '\0'; j++) {
        for (int l = 1; l <= n; l++) {
            if (plaintext[j] >= 'a' && plaintext[j] <= 'z') { // para caracteres for minúsculos
                plaintext[j] = (plaintext[j] + 1); 
                if (plaintext[j] > 'z') { // garante que o caractere se mantenha entre "a" e "z"
                    plaintext[j] = 'a';
                }
            }

            if (plaintext[j] >= 'A' && plaintext[j] <= 'Z') { // para caracteres maiúsculos
                plaintext[j] = (plaintext[j] + 1);
                if (plaintext[j] > 'Z') { // garante que o caractere se mantenha entre "A" e "Z"
                    plaintext[j] = 'A';
                }
            }
        }
    }
}

