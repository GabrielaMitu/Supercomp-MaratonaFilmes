#include<iostream>
#include<vector>
#include<algorithm>
#include <list>
using namespace std;

// STRUCT DO FILME
struct filme {
    int inicio;
    int fim;
    int categoria;
};

// FUNCAO DE ORDENACAO CRESCENTE DO FIM DOS FILMES
bool my_compare(filme a, filme b) {
    if (a.fim != b.fim) {
        return a.fim < b.fim;
    } else {
        return a.inicio < b.inicio;
    }
}

int main() {

    // DECLARAÇÃO DE VARIAVEIS E VETORES
    int n; // Numero de filmes
    int m; // Numero de categorias totais
    cin >> n >> m; // Input das variáveis acima

    int maratona = 0; // Posição em que a maratona se encontra
    vector<filme> filmes(n); // Vetor dos filmes dados pelo input
    vector<int> max_filmes(m); // Máximo de filmes por categoria
    vector<int> cntCat(m, 0); // Armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    vector<filme> selected; // Lista dos filmes selecionados para a maratona
    vector<int> duracao(n); // Duracão de cada filme

    // INPUT DO MÁXIMO DE FILMES POR CATEGORIA
    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }

    // INPUTS DOS FILMES 
    for (int i = 0; i < n; i++) {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }
    
    // ORDENAÇÃO CRESCENTE DO HORARIO DE TERMINO DOS FILMES 
    sort(filmes.begin(), filmes.end(), my_compare);

    // PERCORRENDO TODOS OS FILMES
    for (int i = 0; i < n; i++) {
        duracao[i] = filmes[i].fim - filmes[i].inicio; // Duracao de cada filme

        // FILTRO DE FILMES SE COMEÇAREM A NOITE E TERMINAR DE MADRUGADA NO PROX DIA
        if (filmes[i].fim - filmes[i].inicio < 0) {
            continue;
        }

        // FILTRO SE FILME SELECIONADO NÃO ULTRAPASSA O MAX POR CATEGORIA
        if (cntCat[filmes[i].categoria-1] >= max_filmes[filmes[i].categoria-1]) {
            continue;
        }

        // FILTRO SE FILME NÃO COMEÇA E TERMINA NO MESMO HORÁRIO
        if (duracao[i] == 0) {
            continue;
        }

        // SE O FILME PODE ENTRAR NA MARATONA
        if (filmes[i].inicio >= maratona) {
            cntCat[filmes[i].categoria-1]++; // Aumenta a quantidade de filmes em cada categoria
            maratona = filmes[i].fim; // Atualiza a variavel de maratona para a próxima seleção
            selected.push_back(filmes[i]); // Adiona o filme dos selecionados para a maratona
        }
    

    }
    // -------- OUTPUT (TIRAR PARA A ANALISE NO VALGRIND) --------    
    // FILMES DA MARATONA
/*     cout << "Quantidade de filmes da maratona: " << selected.size() << endl;
    int duracaoT = 0;
    cout << "Duração total dos filmes selecionados: ";
    for (int s = 0; s < static_cast<int>(selected.size()); s++) {
        duracaoT += (selected[s].fim - selected[s].inicio);
    }
    cout << duracaoT << endl;

    cout << "----------" << endl;

    // FILMES SELECIONADOS PARA ASSISTIR
    cout << "Filmes da maratona: " << endl;
    for (int s = 0; s < selected.size(); s++) {
      cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
    }  */

  return 0;
}


// PARA RODAR NO CMD:
// g++ -o gulosa heuristicaGulosa.cpp
// ./gulosa < inputs/input1.txt

// PARA RODAR PARA ANALISAR COM VALGRIND:
// g++ -Wall -O3 -g heuristicaGulosa.cpp -o gulosa
// valgrind --tool=callgrind ./gulosa < input.txt
// callgrind_annotate callgrind.out.6978 heuristicaGulosa.cpp