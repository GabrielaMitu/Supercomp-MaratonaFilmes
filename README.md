# Relatório Parcial - Maratona de Filmes
------------
**Aluna:** Gabriela Mitu

## Gerador

Responsável por criar os inputs das heurísticas, foi apenas modificado em relação ao tempo de duração de filme que foi deixado desta vez em 2.0 e o desvio padrão permaneceu em 1.0. Esta modificação se deve em razão de que na realidade, é mais comum o lançamento de filmes com 2h de duração e uma variação próxima (desvio padrão) de 1h.

Código do Gerador utilizado:

```C++
int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    ofstream inputFile;
    inputFile.open("input.txt");
    inputFile << n << " " << m << endl;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);

    // Definindo distribuição normal com média de 3 e desvio padrão de 1
    normal_distribution<double> distribution_dif(2, 1.0);

    uniform_int_distribution<int> distribution_hr(0, 23);
    uniform_int_distribution<int> distribution_cat(1, m);

    vector<int> maxFilmes(m); // Vetor para armazenar o máximo de filmes por categoria
    for (int i = 0; i < m; i++) {
        maxFilmes[i] = distribution_cat(generator); // Gerando o máximo de filmes para cada categoria
        inputFile << maxFilmes[i] << " "; // Escrevendo o valor no arquivo de entrada
    }
    inputFile << endl;

    for (int i = 0; i < n; i++) {
        int hora_inicio = distribution_hr(generator);
        double dif_media = distribution_dif(generator);
        int hora_fim = ((int)hora_inicio + (int)round(dif_media)) % 24;
        int categoria = distribution_cat(generator);

        inputFile << hora_inicio << " " << hora_fim << " " << categoria << endl;
    }


    inputFile.close();
    return 0;
}
```

## Heurística Gulosa

Para fazer considerações sobre o profiling (valgrind) do código-fonte, foi utilizado o input.txt (considerando 100 filmes e 5 categorias diferentes).

Os seguintes comandos foram realizados:

> g++ -Wall -O3 -g heuristicaGulosa.cpp -o gulosa

> valgrind --tool=callgrind ./gulosa < input.txt

> callgrind_annotate callgrind.out.7983 heuristicaGulosa.cpp > gulosaValgrindInput.txt

**Obs.:** para não perder cada uma das análises, foi colocado o output dentro de gulosaValgrindInput.txt


Neste output, são visualizadas várias informações sobre a execução do seu programa, incluindo a porcentagem de tempo gasto em cada parte do código. Fazendo uma análise total de todo o processamento do código, a parte do código que mais se gasta tempo é no input dos filmes:

![image](https://user-images.githubusercontent.com/49621844/226221111-2d06b05d-e1e0-4cae-8e2c-844b6c58c296.png)
- A primeira linha indica a quantidade de vezes que o loop for é executado. No caso, o loop é executado 406 vezes, o que representa 0.01% do tempo total gasto em execução do programa.
- A segunda linha indica o tempo gasto para executar a leitura de dados do vetor de filmes. No caso, a leitura é executada 900 vezes e representa 0.03% do tempo total gasto em execução do programa.
- A terceira linha indica o momento que ele percorre todo o input.txt, que é bem longo. E por razão disso, ele acaba representando uma porcentagem tão mais significativa que o restante do codigo

O total de tempo registrado é de 2,689,673 unidades de tempo.


------------
## Heurística Aleatória

Para fazer considerações sobre o profiling (valgrind) do código-fonte, foi utilizado também o input.txt (considerando 100 filmes e 5 categorias diferentes).

Os seguintes comandos foram realizados:

> g++ -Wall -O3 -g aleatorizacao.cpp -o aleatorizacao

> valgrind --tool=callgrind ./aleatorizacao < input.txt

> callgrind_annotate callgrind.out.6967 aleatorizacao.cpp > aleatorizacaoValgrindInput.txt

**Obs.:** para não perder cada uma das análises, foi colocado o output dentro de aleatorizacaoValgrindInput.txt


azendo uma análise total de todo o processamento do código, a parte do código que mais se gasta tempo é no input dos filmes, assim como ocorre na Heurística Gulosa. Como nessa parte do código ambas são equivalentes, então o tempo gasto é o mesmo, assim como a porcentagem.

Já acerca do tempo total gasto, foi registrado 2,721,851 unidades de tempo, ou seja, aproximadamente 1.19% a mais que a da Heurística Gulosa

