#include <iostream>
#include <vector>
#include <cmath>
#include<fstream>
#include <bits/stdc++.h>
#include<algorithm>
using namespace std;

struct filme {
    int inicio;
    int fim;
    int categoria;
    int duracao;
};

bool my_compare(filme a, filme b) {
    if (a.fim != b.fim) {
        return a.fim < b.fim; // se a.fim < b.fim, então retorna true
    } else {
        return a.inicio < b.inicio; // se a.inicio < b.inicio, então retorna true
    }
}

int main() {
    default_random_engine generator;
    generator.seed(10);
    uniform_real_distribution<double> distribution(0.0,1.0);

    int n;
    int m;
    cin >> n >> m;

    vector<filme> filmes(n);
    vector<int> max_filmes(m);
    vector<int> duracao(n);

    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }

    for(int i = 0 ; i < n; i++){
        //duracao[i] = filmes[i].fim - filmes[i].inicio;
        //cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria >> duracao[i];
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
        //filmes.push_back({filmes[i].inicio, filmes[i].fim, filmes[i].categoria, filmes[i].duracao});
        //filmes.push_back({filmes[i].inicio, filmes[i].fim, filmes[i].categoria});
      }
      
    //sinvariante - elementos ordenado pelo peso (eh um ponto de certeza)
    sort(filmes.begin(),filmes.end(), my_compare);
/*     cout << "oi";
    for(int i = 0 ; i < n; i++){
        cout << filmes[i].inicio << " " << filmes[i].fim << " " << filmes[i].categoria << endl;
    }
    cout << "oi"; */

    cout << "Filmes size: " << filmes.size() << endl;

    int i = 0;
    vector<int> cntCat(m, 0); // armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    vector<filme> selected; // lista dos filmes selecionados
    
    int dia = 23;
    int maratona = 0;

    for(auto& el: filmes){
        if (distribution(generator) > 0.25 and i < n){
            uniform_int_distribution<int> distribution(i,n);
            int p = distribution(generator);

            if (filmes[p].fim - filmes[p].inicio < 0) {
                continue;
            }
            if (cntCat[filmes[p].categoria-1] >= max_filmes[filmes[p].categoria-1]) {
                continue;
            }
            //else if (filmes[p].duracao + maratona <= dia) {
            else if ((filmes[i].fim - filmes[i].inicio) + maratona <= dia) {
                selected.push_back(filmes[p]);
                maratona = filmes[p].fim;
                filmes.erase(filmes.begin()+p-1);
                n=n-1;
            }
        }
        i=i+1;
    }

    for(auto& el: filmes){
        if (el.fim - el.inicio < 0) {
          continue;
        }
        if (cntCat[el.categoria-1] >= max_filmes[el.categoria-1]) {
          continue;
        }
        else if ((el.fim - el.inicio) + maratona <= dia){
            selected.push_back(el);
            maratona = el.fim;
        }
    }
    
    //ordenando para imprimir
    //sort(selected.begin(),selected.end(),[](auto&i, auto&j){return i.id < j.id;});
    selected.reserve(selected.size());
    cout << "Máximo de filmes que pode ser assistidos: " << selected.size() << endl;

    // filmes selecionados para assistir
    for (int s = 0; s < selected.size(); s++) {
      cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
    }

    
    
    return 0;
  }

// g++ -o aleatorizacao aleatorizacao.cpp