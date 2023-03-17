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

    for (int i = 0; i < m; i++) {
        cin >> max_filmes[i];
    }

    for(int i = 0 ; i < n; i++){
        cin >> filmes[i].inicio >> filmes[i].fim >> filmes[i].categoria;
      }
    

    cout << "Filmes size: " << filmes.size() << endl;

    int i = 0;  // usado para percorrer os filmes do vetor "filmes"
    vector<int> cntCat(m, 0); // armazena o número de filmes assistidos em cada categoria, sendo m o tamanho do vetor e o valor inicial de cada elemento é zero
    vector<filme> selected; // lista dos filmes selecionados
    vector<int> maratona(23, 0);  // inicializa todos os vetores com zero. Muda para 1 se o tempo estiver ocupado

    uniform_int_distribution<int> distribution(i,n);
    for(auto& el: filmes){
        if (distribution(generator) > 0.75 and i < n){ // 25% de chance de pegar outro filme qualquer que respeite o horário.
            int p = distribution(generator);
            if (filmes[p].fim - filmes[p].inicio < 0) {
                continue;
            }
            if (cntCat[filmes[p].categoria-1] >= max_filmes[filmes[p].categoria-1]) {
                continue;
            }
            if ((filmes[p].fim - filmes[p].inicio) == 0) {
                continue;
            }
            bool disponivel = true;
            for (int m = filmes[p].inicio; m <= filmes[p].fim; m++) {      
                if (maratona[m] == 1) {
                    disponivel = false;
                    break;
                }
            }
            if (disponivel) {
                for (int m = filmes[p].inicio; m <= filmes[p].fim; m++) {
                    maratona[m] = 1;
                }
                cntCat[filmes[p].categoria-1]++;
                selected.push_back(filmes[p]);
                filmes.erase(filmes.begin()+p);
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
        if ((el.fim - el.inicio) == 0) {
            continue;
        }
        bool disponivel = true;
        for (int m = el.inicio; m <= el.fim; m++) {      
            if (maratona[m] == 1) {
                disponivel = false;
                break;
            }
        }
        if (disponivel) {
            for (int m = el.inicio; m <= el.fim; m++) {
                maratona[m] = 1;
            }
            cntCat[el.categoria-1]++;
            selected.push_back(el);
        }
    }

    // ordenando para imprimir
    sort(selected.begin(),selected.end(), my_compare);  

    selected.reserve(selected.size());

    cout << "Maratona: ";
    for (int m = 0; m < maratona.size(); m++) {
        cout << maratona[m];
    }
    cout << endl;

    cout << "Máximo de filmes que pode ser assistidos: " << selected.size() << endl;

    // filmes selecionados para assistir
    for (int s = 0; s < selected.size(); s++) {
      cout << selected[s].inicio << " " << selected[s].fim << " " << selected[s].categoria << endl;
    }    
    
    return 0;
  }

// g++ -o aleatorizacao aleatorizacao.cpp