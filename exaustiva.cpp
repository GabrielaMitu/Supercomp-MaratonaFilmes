#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

// STRUCT DO FILME
struct filme {
    int inicio;
    int fim;
    int categoria;
};

// usados: conseguir controlar depois quais itens que eu coloquei na mochila
double maratonaFilmes(int m, vector<filme> filmes, vector<filme>& assistidos, vector<filme>& melhor, vector<int> max_filmes, vector<int> cntCat, vector<int> maratona) {
    int inicio = 0;
    int fim = 0;
    int categoria;
    double sem_f = 0.0, com_f = 0.0; // sem_i = sem itens; com_i = com itens
    vector<filme> filmes2 = filmes;
    vector<int> cntCat2 = cntCat;
    vector<int> maratona2 = maratona;

    // verifica se o vetor de items esta vazio ou capacidade é zero
    if (filmes.empty() || m == 0) {
        return 0;
    }

    // dois braços da recursao:
    // verifica se o primeiro filme da lista de filmes tem uma categoria que não esteja no valor máximo e não madruga e não começa e termina no msm horario
    if (cntCat[filmes[0].categoria - 1] < max_filmes[filmes[0].categoria - 1] && filmes[0].fim - filmes[0].inicio > 0) {
        assistidos.push_back(filmes[0]);
        inicio = filmes[0].inicio;
        fim = filmes[0].fim;
        categoria = filmes[0].categoria;

        // SE O FILME DEVE ENTRAR NA MARATONA
        bool disponivel = true;
        for (int m = inicio; m <= fim; m++)
        {
            if (maratona[m] == 1)
            {
                disponivel = false;
                break;
            }
        }

        if (disponivel)
        {
            for (int m = inicio; m <= fim; m++)
            {
                maratona[m] = 1;
            }
            cntCat[filmes[0].categoria - 1]++;
            filmes.erase(filmes.begin()); // apaga o primeiro item do vetor de filmes original
            com_f = maratonaFilmes(m, filmes, assistidos, melhor, max_filmes, cntCat, maratona); // versão incluindo o item
            //std::cout << "com_f: " << com_f << endl;
        }
    }
    filmes2.erase(filmes2.begin());
    sem_f = maratonaFilmes(m, filmes2, assistidos, melhor, max_filmes, cntCat2, maratona2); // versão não incluindo o filme
    //std::cout << "sem_f: " << sem_f << endl;

    int countMaratona = 0; // com filme
    int countMaratona2 = 0; // sem filme
    for (int i : maratona) {
        if (i == 1) {
            countMaratona += 1;
        }
    }
    for (int j : maratona2) {
        if (j == 1) {
            countMaratona2 += 1;
        }
    }

    if (countMaratona2 >= countMaratona) {
        melhor = assistidos;
    }

    if (sem_f == com_f) {
        return sem_f;
    }
    assistidos.clear();

    return max(com_f, sem_f);
}

int main() {
    int n = 0; // numero de filmes
    int m = 0; // numero de categorias totais
    cin >> n >> m;
    vector<filme> assistidos, melhor;
    vector<filme> filmes(n);     // Vetor dos filmes dados pelo input
    vector<int> max_filmes(m);   // Máximo de filmes por categoria
    vector<int> cntCat(m, 0);    // Armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
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

    cout << "Resultado " << maratonaFilmes(m, filmes, assistidos, melhor, max_filmes, cntCat, maratona) << "\n"; 
    for (auto& el: melhor) {
        cout << el.inicio << " " << el.fim << " " << el.categoria << "\n";
    }
    return 0;
}

// cmd:
// g++ -o exaustiva exaustiva.cpp
// ./exaustiva < inputs/input1.txt