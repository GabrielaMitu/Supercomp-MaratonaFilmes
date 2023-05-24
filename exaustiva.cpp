#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>

struct filme {
    int inicio;
    int fim;
    int categoria;
};

bool my_compare(filme a, filme b) {
    if (a.fim != b.fim) {
        return a.fim < b.fim;
    } else {
        return a.inicio < b.inicio;
    }
}

bool hasOverlap(filme a, filme b) {
    return !(a.fim <= b.inicio || b.fim <= a.inicio);
}

void buscaExaustiva(const std::vector<filme>& filmes, const std::vector<int>& max_filmes,
                    std::vector<filme>& selecaoAtual, std::vector<int>& cntCat,
                    int bitmask, int pos, int& maxSelecionados, int& duracaoMaxTotal,
                    std::vector<filme>& melhorSelecao) {
    if (pos >= filmes.size()) {
        int duracaoAtual = 0;
        for (const auto& filme : selecaoAtual) {
            duracaoAtual += (filme.fim - filme.inicio);
        }

        bool valido = true;
        for (int i = 0; i < cntCat.size(); i++) {
            if (cntCat[i] > max_filmes[i]) {
                valido = false;
                break;
            }
        }

        if (valido && duracaoAtual > duracaoMaxTotal) {
            duracaoMaxTotal = duracaoAtual;
            maxSelecionados = selecaoAtual.size();
            melhorSelecao = selecaoAtual;
        }

        return;
    }

    buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, bitmask, pos + 1,
                    maxSelecionados, duracaoMaxTotal, melhorSelecao);

    if (!(bitmask & (1 << pos))) {
        bool overlap = false;
        for (const auto& filme : selecaoAtual) {
            if (hasOverlap(filmes[pos], filme)) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            selecaoAtual.push_back(filmes[pos]);
            cntCat[filmes[pos].categoria - 1]++;
            buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, bitmask | (1 << pos),
                            pos + 1, maxSelecionados, duracaoMaxTotal, melhorSelecao);
            selecaoAtual.pop_back();
            cntCat[filmes[pos].categoria - 1]--;
        }
    }
}

int main() {
    int n; // Número de filmes
    int m; // Número de categorias totais
    std::cin >> n >> m;

    std::vector<filme> filmes(n);
    std::vector<int> max_filmes(m);
    std::vector<filme> melhorSelecao;
    std::vector<int> duracaoTotal(m, 0);

    for (int i = 0; i < m; i++) {
        std::cin >> max_filmes[i];
    }

    for (int i = 0; i < n; i++) {
        std::cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }

    std::sort(filmes.begin(), filmes.end(), my_compare);

    int maxSelecionados = 0;
    int duracaoMaxTotal = 0;

    std::vector<filme> selecaoAtual;
    std::vector<int> cntCat(m, 0);

    buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, 0, 0,
                    maxSelecionados, duracaoMaxTotal, melhorSelecao);

    std::cout << "Quantidade de filmes da maratona: " << maxSelecionados << std::endl;
    std::cout << "Duração total dos filmes selecionados: " << duracaoMaxTotal << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << "Filmes da maratona: " << std::endl;
    for (const auto& filme : melhorSelecao) {
        std::cout << filme.inicio << " " << filme.fim << " " << filme.categoria << std::endl;
    }

    return 0;
}



// PARA RODAR NO CMD:
// g++ -o exaustiva exaustiva.cpp
// ./exaustiva < inputs/input1.txt