#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// STRUCT DO FILME
struct filme
{
    int inicio;
    int fim;
    int categoria;
};

// FUNCAO DE ORDENACAO CRESCENTE DO FIM DOS FILMES
bool my_compare(filme a, filme b)
{
    if (a.fim != b.fim)
    {
        return a.fim < b.fim;
    }
    else
    {
        return a.inicio < b.inicio;
    }
}

int main()
{
    // GERADOR DE NÚMERO ALEATÓRIO ENTRE 0.0 E 1.0
    default_random_engine generator;
    generator.seed(10);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // DECLARAÇÃO DE VARIAVEIS E VETORES
    int n;         // Numero de filmes
    int m;         // Numero de categorias totais
    cin >> n >> m; // Input das variáveis acima

    vector<filme> filmes(n);     // Vetor dos filmes dados pelo input
    vector<int> max_filmes(m);   // Máximo de filmes por categoria
    int i = 0;                   // Usado para percorrer os filmes do vetor "filmes"
    vector<int> cntCat(m, 0);    // Armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    vector<filme> selected;      // Lista dos filmes selecionados para a maratona
    vector<int> maratona(23, 0); // Relacionado à maratona inicializa todos os valores com zero. Muda para 1 se o tempo estiver ocupado

    // INPUT DO MÁXIMO DE FILMES POR CATEGORIA
    for (int i = 0; i < m; i++)
    {
        cin >> max_filmes[i];
    }

    // INPUTS DOS FILMES
    for (int i = 0; i < n; i++)
    {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }

    // ANALISE ALEATORIA DE FILME QUE CAIBA NA MARATONA
    auto filmeP = filmes.begin(); // filmeP percorre todos os filmes
    while (filmeP != filmes.end())
    {
        if (distribution(generator) > 0.75 and i < n)
        {                                                           // 25% de chance de pegar outro filme qualquer que respeite o horário.
            uniform_int_distribution<int> distributionFilmes(i, n); // numero aleatorio para escolher um filme
            int p = distributionFilmes(generator);

            // FILTRO DE FILMES SE COMEÇAREM A NOITE E TERMINAR DE MADRUGADA NO PROX DIA
            if (filmes[p].fim - filmes[p].inicio < 0)
            {
                continue;
            }

            // FILTRO SE FILME SELECIONADO NÃO ULTRAPASSA O MAX POR CATEGORIA
            if (cntCat[filmes[p].categoria - 1] >= max_filmes[filmes[p].categoria - 1])
            {
                continue;
            }

            // FILTRO SE FILME NÃO COMEÇA E TERMINA NO MESMO HORÁRIO
            if ((filmes[p].fim - filmes[p].inicio) == 0)
            {
                continue;
            }

            // SE O FILME DEVE ENTRAR NA MARATONA
            bool disponivel = true;
            for (int m = filmes[p].inicio; m <= filmes[p].fim; m++)
            {
                if (maratona[m] == 1)
                {
                    disponivel = false;
                    break;
                }
            }
            if (disponivel)
            {
                for (int m = filmes[p].inicio; m <= filmes[p].fim; m++)
                {
                    maratona[m] = 1;
                }
                cntCat[filmes[p].categoria - 1]++;
                selected.push_back(filmes[p]);
                filmes.erase(filmes.begin() + p);
                n = n - 1;
            }
        }
        i = i + 1;
        filmeP++;
    }

    // ANALISANDO O RESTO DE FILMES NÃO ANALISADOS ANTERIORMENTE
    for (auto &el : filmes)
    {
        // FILTRO DE FILMES SE COMEÇAREM A NOITE E TERMINAR DE MADRUGADA NO PROX DIA
        if (el.fim - el.inicio < 0)
        {
            continue;
        }

        // FILTRO SE FILME SELECIONADO NÃO ULTRAPASSA O MAX POR CATEGORIA
        if (cntCat[el.categoria - 1] >= max_filmes[el.categoria - 1])
        {
            continue;
        }

        // FILTRO SE FILME NÃO COMEÇA E TERMINA NO MESMO HORÁRIO
        if ((el.fim - el.inicio) == 0)
        {
            continue;
        }

        // SE O FILME DEVE ENTRAR NA MARATONA
        bool disponivel = true;
        for (int m = el.inicio; m <= el.fim; m++)
        {
            if (maratona[m] == 1)
            {
                disponivel = false;
                break;
            }
        }
        if (disponivel)
        {
            for (int m = el.inicio; m <= el.fim; m++)
            {
                maratona[m] = 1;
            }
            cntCat[el.categoria - 1]++;
            selected.push_back(el);
        }
    }

    // -------- OUTPUT (TIRAR PARA A ANALISE NO VALGRIND) --------
    // ORDENANDO PARA IMPRIMIR
    /*     sort(selected.begin(),selected.end(), my_compare);

        // ESTADO FINAL DA MARATONA
        cout << "Maratona: ";
        for (int m = 0; m < static_cast<int>(maratona.size()); m++) {
            cout << maratona[m];
        }
        cout << endl;

        // FILMES DA MARATONA
        cout << "Quantidade de filmes da maratona: " << selected.size() << endl;

        cout << "----------" << endl;
        // FILMES SELECIONADOS PARA ASSISTIR
        cout << "Filmes da maratona: " << endl;
        for (int s = 0; s < static_cast<int>(selected.size()); s++) {
          cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
        }
         */
    return 0;
}

// PARA RODAR NO CMD:
// g++ -o aleatorizacao aleatorizacao.cpp
// ./aleatorizacao < "input.txt"

// PARA RODAR PARA ANALISAR COM VALGRIND:
// g++ -Wall -O3 -g aleatorizacao.cpp -o aleatorizacao
// valgrind --tool=callgrind ./aleatorizacao < input.txt
// callgrind_annotate callgrind.out.6978 aleatorizacao.cpp > aleatorizacaoValgrindInput.txt