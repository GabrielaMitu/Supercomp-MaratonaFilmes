#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

struct filme {
    int inicio;
    int fim;
    int categoria;
};


bool my_compare(filme a, filme b) {
    if (a.fim != b.fim) {
        return a.fim < b.fim; // se a.fim < b.fim, então retorna true
    } else {
        return a.inicio < b.inicio; // se a.inicio < b.inicio, então retorna true
    }
}

int main() {
    int n; // numero de filmes
    int m; // numero de categorias
    cin >> n >> m;
    vector<filme> filmes(n);

    vector<int> max_filmes(m); // numero max por categoria

    for (int i = 0; i < m; i++) {
        //input_file >> max_filmes[i];
        cin >> max_filmes[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }
    
    // ORDENAÇÃO CRESCENTE DO HORARIO DE TERMINO DOS FILMES 
    sort(filmes.begin(), filmes.end(), my_compare);

    int ans = 0; // número máximo de filmes que podem ser assistidos de acordo com as restrições de horários e número máximo por categoria
    vector<int> cnt(m, 0); // armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    for (int i = 0; i < n; i++) {
      // Percorre os filmes em ordem crescente de horário de término e, para cada filme, verifica se ainda é possível assistir um filme daquela categoria
        if (cnt[filmes[i].categoria-1] < max_filmes[filmes[i].categoria-1]) {
            ans++;
            cnt[filmes[i].categoria-1]++; // aumenta o numero de filme selecionado de cada categoria 
        }
    }

    cout << ans << endl;

  return 0;
}
