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
    vector<filme> tempF;    // Vetor temporario para armazenar os filmes que começam no mesmo horario

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

        int inicioF = filmes[i].inicio; // Inicio do filme
        vector<int> duracao2;
        int best; // Melhor filme para assistir
        bool found = false;
        bool vaiprox = false;
        int w = 0;
        //std::cout << "oi :" << filmes[i].inicio << " " << filmes[i].fim << " " <<filmes[i].categoria << endl;
        int j = 0;

        // Coloca em um vetor todos os filmes que tem inicio no mesmo horario 
        while (!found && !vaiprox) {
            while (w < n) {
                if (filmes[i + j].inicio == maratona + w) {
                    tempF.push_back(filmes[i + j]);
                    duracao2.push_back(filmes[i + j].fim - filmes[i + j].inicio);
                    found = true;
                    break;
                }
                if (filmes[i + j].inicio < maratona) {
                    vaiprox = true;
                    break;
                }
            w++;
            }
        j++;

        }

/* 
        for (int s = 0; s < static_cast<int>(tempF.size()); s++) {
            std::cout << tempF[s].inicio << " " << tempF[s].fim << " " << tempF[s].categoria << endl;
        } */

        if (found) {
            
            // Encontra o iterador para o menor valor usando min_element
            auto iteradorMenor = std::min_element(duracao2.begin(), duracao2.end());

            // Calcula a posição (índice) do menor valor usando distance
            int bestF = std::distance(duracao2.begin(), iteradorMenor);
            //std::cout << "bestF: " << bestF << endl;

            cntCat[tempF[bestF].categoria-1]++; // Aumenta a quantidade de filmes em cada categoria
            maratona = tempF[bestF].fim; // Atualiza a variavel de maratona para a próxima seleção
            //std::cout << "aloo: " << tempF[bestF].fim << endl;
            selected.push_back(tempF[bestF]); // Adiona o filme dos selecionados para a maratona
            
            tempF.clear(); // Limpa o vetor tempF
            duracao2.clear(); // Limpa o vetor duracao2

            //std::cout << "fim :" << filmes[i].inicio << " " << filmes[i].fim << " " << filmes[i].categoria << endl;
        }

    }

    // -------- OUTPUT (TIRAR PARA A ANALISE NO VALGRIND) --------    
    // FILMES DA MARATONA
    std::cout << "Quantidade de filmes da maratona: " << selected.size() << endl;
    int duracaoT = 0;
    std::cout << "Duração total dos filmes selecionados: ";
    for (int s = 0; s < static_cast<int>(selected.size()); s++) {
        duracaoT += (selected[s].fim - selected[s].inicio);
    }
    std::cout << duracaoT << endl;

    std::cout << "----------" << endl;

    // FILMES SELECIONADOS PARA ASSISTIR
    std::cout << "Filmes da maratona: " << endl;
    for (int s = 0; s < static_cast<int>(selected.size()); s++) {
      std::cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
    }


    return 0;
}
// PARA RODAR NO CMD:
// g++ -o exaustiva exaustiva.cpp
// ./exaustiva < inputs/input1.txt