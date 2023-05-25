#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

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

// VERIFICAÇÃO SE DOIS FILMES TEM SOBREPOSIÇÃO DE HORÁRIOS
bool hasOverlap(filme a, filme b) {
    return !(a.fim <= b.inicio || b.fim <= a.inicio);
}

// FUNÇÃO RECURSIVA QUE FAZ A BUSCA EXAUSTIVA
void buscaExaustiva(const vector<filme>& filmes, const vector<int>& max_filmes,
                    vector<filme>& selecaoAtual, vector<int>& cntCat,
                    int bitmask, int pos, int& maxSelecionados, int& duracaoMaxTotal,
                    vector<filme>& melhorSelecao) {

    // VERIFICA SE A BUSCA CHEGOU AO FIM
    if (pos >= filmes.size()) {
        int duracaoAtual = 0;
        for (const auto& filme : selecaoAtual) {
            duracaoAtual += (filme.fim - filme.inicio);
        }
        // VERIFICA SE O NÚMERO DE FILMES SELECIONADOS PARA CADA CATEGORIA NAO EXCEDE O LIMITE ESTABELECIDO POR MAX_FILMES
        bool valido = true;
        for (int i = 0; i < cntCat.size(); i++) {
            if (cntCat[i] > max_filmes[i]) {
                valido = false;
                break;
            }
        }
        // SE A SELEÇÃO ATUAL FOR VÁLIDA E A DURAÇÃO TOTAL FOR MAIOR QUE A DURAÇÃO MÁXIMA ATUAL, ATUALIZA A DURAÇÃO MÁXIMA E O NÚMERO DE FILMES SELECIONADOS
        if (valido && duracaoAtual > duracaoMaxTotal) {
            duracaoMaxTotal = duracaoAtual;
            maxSelecionados = selecaoAtual.size();
            melhorSelecao = selecaoAtual;
        }

        return;
    }

    // CONTINUA A BUSCA SEM SELECIONAR O FILME ATUAL
    buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, bitmask, pos + 1,
                    maxSelecionados, duracaoMaxTotal, melhorSelecao);

    // SE O FILME ATUAL NÃO TIVER SIDO SELECIONADO AINDA
    if (!(bitmask & (1 << pos))) {
        bool overlap = false;
        for (const auto& filme : selecaoAtual) {
            if (hasOverlap(filmes[pos], filme)) {
                overlap = true;
                break;
            }
        }
        // E TAMBEM SE NAO TIVER SOBREPOSIÇÃO DE HORÁRIOS COM NENHUM DOS FILMES JÁ SELECIONADOS
        if (!overlap) {
            selecaoAtual.push_back(filmes[pos]);
            cntCat[filmes[pos].categoria - 1]++;
            buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, bitmask | (1 << pos),
                            pos + 1, maxSelecionados, duracaoMaxTotal, melhorSelecao);
            selecaoAtual.pop_back(); // filme é removido da seleção atual
            cntCat[filmes[pos].categoria - 1]--; // contador de filmes da categoria é decrementado
        }
    }
}

int main() {
    int n; // Número de filmes
    int m; // Número de categorias totais
    cin >> n >> m;

    vector<filme> filmes(n);
    vector<int> max_filmes(m);
    vector<filme> melhorSelecao;
    vector<int> duracaoTotal(m, 0);

    // INPUTS
    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }

    // USO DA FUNÇÃO DE ORDENAÇÃO
    sort(filmes.begin(), filmes.end(), my_compare);

    int maxSelecionados = 0;
    int duracaoMaxTotal = 0;

    vector<filme> selecaoAtual;
    vector<int> cntCat(m, 0);

    buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, 0, 0,
                    maxSelecionados, duracaoMaxTotal, melhorSelecao);

    cout << "Quantidade de filmes da maratona: " << maxSelecionados << std::endl;
    cout << "Duração total dos filmes selecionados: " << duracaoMaxTotal << std::endl;
    cout << "----------" << std::endl;
    cout << "Filmes da maratona: " << std::endl;
    for (const auto& filme : melhorSelecao) {
        cout << filme.inicio << " " << filme.fim << " " << filme.categoria << std::endl;
    }

    return 0;
}


// PARA RODAR NO CMD:
// g++ -o exaustiva exaustiva.cpp
// ./exaustiva < inputs/input1.txt