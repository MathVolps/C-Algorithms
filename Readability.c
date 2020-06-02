#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string str, int n);
int count_words(string str, int n);
int count_sentences(string str, int n);
void readability(int index);

int main() {
    string str = get_string("Text: ");
    int n = strlen(str); //conta a quantidade de caracteres (em geral) da string

    float num_letters = count_letters(str, n);
    float num_words = count_words(str, n);
    float num_sentences = count_sentences(str, n);

    float L = (num_letters/num_words)* 100;
    float S = (num_sentences/num_words) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8; //calculando o index, conforma fo´rmula disponibilizada

    index = round(index); //arredonda o index, calculado anteriormemnte
    readability(index);
    
}

// Conta as letras da string
int count_letters(string str, int n) {
    int letters = 0;
    for (int i = 0; i <= n; i++) {
        if (((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) && (str[i] != ' ' && str[i] != '.' && str[i] != ',')) {
            letters++;
        }
    }
    return letters;
}

// Conta as palvras da string
int count_words(string str, int n) {
    int words = 0;
    for (int j = 0; j <= n; j++) {
        if (str[j] == ' ') {
            words++;
        }
    }
    words++;

    return words;
}

// Conta as sentenças da string
int count_sentences(string str, int n) {
    int sentences = 0;
    for (int k = 0; k <= n; k++) {
        if (str[k] == '.' || str[k] == '!' || str[k] == '?') {
            sentences++;
        }
    }

    return sentences;
}

// Imprime os resultados finais
void readability(int index) {
    if (index >= 16) {
        printf("Grade 16+\n");
    } else if (index < 1) {
        printf("Before Grade 1\n");
    } else {
        printf("Grade %.f\n", index);
    }
}