#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int comand_verification(int argc, char **argv);
int duplicated_caracters(char **argv);
void encrypting(string plaintext, char **argv);

int main(int argc, char *argv[]) {
    if (comand_verification(argc, argv) == 1) { 
        return 1;
    }

    if (duplicated_caracters(argv) == 1) {
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    encrypting(plaintext, argv);
    printf("ciphertext: %s\n", plaintext);
    return 0;
}

// Verifica se a linha de comando está formatada corretamente
int comand_verification(int argc, char **argv) {
    if (argc == 1) { // o argc não deve ser 1, pois, se assim for, não haverá alfabeto para substituição
        printf("./substitution key\n");
        return 1;
    } else if (argc == 2) { // agora que o n° de argumentos do argc foi verificado, vamos checá-lo mais detalhadamente
        int count = 0; // contador de letras 
        for (int i = 0; argv[1][i] != '\0'; i++) { // neste loop, verifica-se se algum elementos do argv[1] é alfanumérico ou dígitos (o que não deve ocorrer)
            if (isalpha(argv[1][i]) == 0) {
                printf("./substitution key\n");
                return 1;

            } else if (isdigit(argv[1][i] != 0)) {
                printf("./substitution key\n");
                return 1;

            } else { //se estiver adequado, o contador é acrescentado
                count++;
            }
        }

        if (count < 26) { // para ser um alfabeto de substituição, deve conter 26 elementos
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    } else { // se houver mais do que 2 argumentos na linha de comando, esta também não está adequada
        printf("./substitution key\n");
        return 1;
    }

    return 0;
}

// Verifica se há elementos duplicados no alfabeto de substituição inserido na linha de comando
int duplicated_caracters(char **argv) {
    for (int k = 0; k < 26; k++) { // for´s aninhados que verificam, dois a dois, se há elementos repetidos
        for (int l = k + 1; l < 26; l++) {
            if (argv[1][k] == argv[1][l]) {
                return 1;
            }
        }
    }
    return 0;
}

// Codifica (ou encripta) o texto original, substituindo os carateres pelos correspondentes, inseridos na linha de comando
void encrypting(string plaintext, char **argv) {
    for (int j = 0; plaintext[j] != '\0'; j++) { // percorre toda a string
        if (plaintext[j] != ' ' || plaintext[j] != ',' || plaintext[j] != '!' || plaintext[j] != '?') { // não codifica símbolos gramaticais
            int n = plaintext[j];
            if (islower(plaintext[j]) != 0) { // faz a substituição, para letras minúsculas
                plaintext[j] = tolower(argv[1][n - 97]);
            } else if (isupper(plaintext[j]) != 0) { // faz a substituição, para letras maiúsculas
                plaintext[j] = toupper(argv[1][n - 65]);
            }
        }
    }

}
