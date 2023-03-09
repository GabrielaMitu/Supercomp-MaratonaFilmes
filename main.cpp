
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
    vector<int> max_filmes(m); // numero max de filmes por categoria

    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }


    for (int i = 0; i < n; i++) {
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
    }
    
    // ORDENAÇÃO CRESCENTE DO HORARIO DE TERMINO DOS FILMES 
    sort(filmes.begin(), filmes.end(), my_compare);

    int selQuant = 0; // número máximo de filmes que podem ser assistidos de acordo com as restrições de horários e número máximo por categoria
    vector<int> cntCat(m, 0); // armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    vector<filme> selected; // lista dos filmes selecionados

    for (int i = 0; i < n; i++) {
      if (filmes[i].fim - filmes[i].inicio < 0) {
          continue;
      }

      // Verifica se os filmes selecionadps não ultrapassam o máximo por categoria
      if (cntCat[filmes[i].categoria-1] >= max_filmes[filmes[i].categoria-1]) {
          continue;
      }

      cout << "catQuant: " << cntCat[filmes[i].categoria-1] << " " << "maxPorCat: " << max_filmes[filmes[i].categoria-1] << endl;

      cntCat[filmes[i].categoria-1]++;

      for (int j = i + 1; j < n; j++) {
          if (filmes[j].inicio > filmes[i].fim) {
              i = j - 1;
              selQuant++;
              selected.push_back(filmes[i]);
              break;
          }
      }
    }

    // -------- OUTPUT --------
    cout << "Máximo de filmes que pode ser assistidos: " << selQuant << endl;

    // filmes selecionados para assistir
    for (int s = 0; s < selected.size(); s++) {
      cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
    }


  return 0;
}

// CMD:
// g++ -o filmesM filmesM.cpp
// ./filmesM < "input.txt"