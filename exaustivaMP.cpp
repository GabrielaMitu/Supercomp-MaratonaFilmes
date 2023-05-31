#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

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

void buscaExaustiva(const vector<filme>& filmes, const vector<int>& max_filmes,
                    vector<filme>& selecaoAtual, vector<int>& cntCat,
                    int bitmask, int pos, int& maxSelecionados, int& duracaoMaxTotal,
                    vector<filme>& melhorSelecao) {

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

        // GARANTE QUE APENAS UMA THREAD POR VEZ EXECUTE O CODIGO DENTRO DA SEÇÃO CRITICA
        #pragma omp critical
        {
            if (valido && duracaoAtual > duracaoMaxTotal) {
                duracaoMaxTotal = duracaoAtual;
                maxSelecionados = selecaoAtual.size();
                melhorSelecao = selecaoAtual;
            }
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
    int n;
    int m;
    cin >> n >> m;

    vector<filme> filmes(n);
    vector<int> max_filmes(m);
    vector<filme> melhorSelecao;
    vector<int> duracaoTotal(m, 0);

    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }

    sort(filmes.begin(), filmes.end(), my_compare);

    int maxSelecionados = 0;
    int duracaoMaxTotal = 0;

    // CODIGO DEVE SER EXECUTADO EM PARALELO POR VÁRIAS THREADS
    #pragma omp parallel
    {
        // VETORES LOCAIS PARA CADA THREAD
        vector<filme> selecaoAtual;
        vector<int> cntCat(m, 0);

        // USADO PARA PARALELIZAR O LOOP
        #pragma omp for
        for (int i = 0; i < n; i++) {
            buscaExaustiva(filmes, max_filmes, selecaoAtual, cntCat, 0, i,
                            maxSelecionados, duracaoMaxTotal, melhorSelecao);
        }
    }

    cout << "Quantidade de filmes da maratona: " << maxSelecionados << endl;
    cout << "Duração total dos filmes selecionados: " << duracaoMaxTotal << endl;
    cout << "----------" << endl;
    cout << "Filmes da maratona: " << endl;
    for (const auto& filme : melhorSelecao) {
        cout << filme.inicio << " " << filme.fim << " " << filme.categoria << endl;
    }

    return 0;
}


// PARA RODAR NO CMD:
// g++ -o exaustivaMP exaustivaMP.cpp -fopenmp
// ./exaustivaMP < inputs/input0.txt