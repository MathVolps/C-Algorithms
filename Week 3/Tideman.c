#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
int strength_of_victory(const void *candidate1, const void *candidate2);
void sort_pairs(void);
bool has_cycle_helper(int index, bool visited[]);
bool has_cycle(int starting_index);
void lock_pairs(void);
bool is_source(int index);
int get_source();
int get_source();
void print_winner(void);

int main(int argc, string argv[]) {
    // Check for invalid usage
    if (argc < 2) {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX) {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++) {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Recebe os votos, verifica se são válidas e os contabiliza
bool vote(int rank, string name, int ranks[]) {
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(name, candidates[i]) == 0) { // compara o nome do voto ao vetor de candidatos
            ranks[rank] =  i; // se estiver correto, a posição "rank" do vetor ranks recebe o index (i) do candidato que recebeu o voto
            return true;
        }
    }
    return false; // se o voto estiver indorreto, retorna falso
}

// Atualiza a matriz de preferências
void record_preferences(int ranks[]) {
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Analisa a matriz de preferências e atribui determina o vencedor e o perdedor (de cada par)
void add_pairs(void) {
    for (int i = 0; i < candidate_count; i++) { // percorre as linhas da matriz
        for (int j = 0; j < candidate_count; j++) { // percorre as colunas da matriz
            if (preferences[i][j] > preferences[j][i]) { //compara os elementos simétricos (ex: a12 e a21)
                                                         //se houver vencedor, insere as informações num struct "pair" e o struct no vetor de pares
                pair p;
                p.winner = i;
                p.loser = j;
                pairs[pair_count++] = p;

            }
        }
    }
}

// Calcula a "força de vitória" de cada par de candidatos (exemplo, para os candidatos A e B, faz-se A-B e B-A, e depois retorna-se a diferença)
int strength_of_victory(const void *candidate1, const void *candidate2) {
    pair a = *((pair*)candidate1);
    pair b = *((pair*)candidate2);
    int comparacao_1 = preferences[a.winner][a.loser] - preferences[a.loser][a.winner];
    int comparacao_2 = preferences[b.winner][b.loser] - preferences[b.loser][b.winner];
    return comparacao_2 - comparacao_1;
}

// Ordena o vetor de pares em ordem decrescente de "força de vitória"
void sort_pairs(void) {
    qsort(pairs, pair_count, sizeof(pair), strength_of_victory); // para isso, foi usada a função qsort, da stdlib
    //qsort(vetor, tamanho do vetor, tipo de elementos (no caso, structs), critério de ordenação)
}

// Verifica se há um ciclo no grafo, parte 2 (recursiva)
bool has_cycle_helper(int index, bool visited[]) { // recebe o índice a partir do qual deseja-se verifica a existência ou não de ciclos 
    if (visited[index] == true) { // se o nó do índice passado já voi visitado, retorna true (fim da pilha de recursão = condição de parada)
        return true;
    }
    visited[index] = true; // se o nó ainda não havia sido visitado, agora foi
    for (int i = 0; i < candidate_count; i++) { // percorre novamente todos os nós
        if (locked[index][i] && has_cycle_helper(i, visited)) { // verifica, recursivamente, se há ciclos
            return true;
        }

    }
    return false;
}

// Verifica se há um ciclo no grafo, parte 1
bool has_cycle(int starting_index) {
    bool visited[candidate_count]; // cria um vetor booleano, para controlarmos quais nós (candidatos) já foram visitados, ou seja, têm aresta incidente (perdem disputas)
    for (int i = 0; i < candidate_count; i++) { // percorre os nós (candidatos)
        visited[i] = false; // no início, nenhum nó foi "visitado" ainda
    }
    return has_cycle_helper(starting_index, visited); // chama outra função, auxiliar na verificação de ciclos
}

// Forma as arestas etre os pares de candidatos do grafo, sem formar ciclos
void lock_pairs(void) {
    for (int i = 0; i < pair_count; i++) {
        locked[pairs[i].winner][pairs[i].loser] = true; // cria a aresta saindo do candidato vencedor até o perdedor
        if (has_cycle(i)) { // se a criação da aresta acima formou um ciclo no grafo, ela deve ser retirada (false)
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// Verifica se determinado nó (candidato) do grafo é um "source"
bool is_source(int index) {
    for (int i = 0; i < candidate_count; i++) { //percorre todos os nós, novamente
        if (locked[i][index] == true) { //verifica se o nó passado como parâmetro (index) perde (tem arestas incidentes) de qualquer um dos outros nós
            return false; //se a verificação acima for verdadeira, significa que aquele nó (index) não pode ser um "source"
        }
    }
    return true; // caso contrário, retorna true. Aquele nó (index) é um "source"
}

// Identifica um "source" (nó sem arestas incidentes) no grafo
int get_source() {
    for (int i = 0; i < candidate_count; i++) { // percorre todos os nós (candidatos)
            if (is_source(i) == true) { //se aquele nó (candidato) dor um "source", seu índice é retornado
                return i;
            }
    }
    return -1; // se não, retorna -1
}

// Imprime na tela o vencedor das eleições
void print_winner(void) {
    int a = get_source(); // verifica se há vencedor 
    if (a != -1) {
        printf("%s\n", candidates[a]); // se sim, o nome do vencedor é impresso
    }
}
