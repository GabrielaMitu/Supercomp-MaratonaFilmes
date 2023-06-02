# Relatório - Maratona de Filmes
------------
**Aluna:** Gabriela Mitu

## Introdução
Um pessoa quer passar um final de semana assistindo o máximo de filmes possível em um dia, mas há restrições quanto aos horários disponíveis e ao número de títulos que podem ser vistos em cada categoria (comédia, drama, ação, etc).
 - Objetivo: Maximizar o número de filmes assistidos em um dia, respeitando o limite de filmes por categoria.
 - Cuidados que devem ser tomados: se nenhum filme se sobrepõe e se não passa do tempo máximo de maratona (1 dia)
 - Algoritmos: Heurística Gulosa e Aleatorizada

Para o início da solução do problema, foi criado um gerador que servirá como os inputs das opções de filmes para assistir na maratona.

## Gerador

O gerador é responsável por criar os inputs das heurísticas e modificado em relação ao tempo de duração de filme que foi deixado desta vez em 2.0 e o desvio padrão em 1.0. Esta modificação se deve em razão de que na realidade, é mais comum o lançamento de filmes com 2h de duração e uma variação próxima (desvio padrão) de 1h.

Código do Gerador utilizado:

```C++
int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    ofstream inputFile;
    inputFile.open("input0.txt");
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

## Inputs

Com o Gerador, foram criados 6 inputs diferentes:

|                |        Número de filmes       |    Número de categorias     |
|----------------|-------------------------------|-----------------------------|
|    Input0      |            10                 |              3              |
|    Input1      |            100                |              5              |
|    Input2      |            500                |              10             |
|    Input3      |            1000               |              15             |
|    Input4      |            5000               |              20             |
|    Input5      |            10000              |              25             |


## Heurística Gulosa

A primeira implementação da heurística para nosso projeto consiste em uma implementação gulosa (Greedy), a qual ordena os filmes por hora de fim crescente e escolhe aqueles que começam primeiro e não conflitam com os filmes já escolhidos, além de verificar se há vagas disponíveis na categoria do filme.

Para fazer considerações sobre o profiling (valgrind) do código-fonte, foi utilizado o input.txt (considerando 100 filmes e 5 categorias diferentes).

Os seguintes comandos foram realizados:

> g++ -Wall -O3 -g heuristicaGulosa.cpp -o gulosa

> valgrind --tool=callgrind ./gulosa < inputs/input1.txt

> callgrind_annotate callgrind.out.7983 heuristicaGulosa.cpp

**Obs.:** O callgrind foi deixado dentro da pasta callgrinds

Neste output, são visualizadas várias informações sobre a execução do seu programa, incluindo a porcentagem de tempo gasto em cada parte do código. Fazendo uma análise total de todo o processamento do código, a parte do código que mais se gasta tempo é no input dos filmes:

![image](https://user-images.githubusercontent.com/49621844/226221111-2d06b05d-e1e0-4cae-8e2c-844b6c58c296.png)

- A primeira linha indica a quantidade de vezes que o loop for é executado. No caso, o loop é executado 406 vezes, o que representa 0.01% do tempo total gasto em execução do programa.
- A segunda linha indica o tempo gasto para executar a leitura de dados do vetor de filmes. No caso, a leitura é executada 900 vezes e representa 0.03% do tempo total gasto em execução do programa.
- A terceira linha indica o momento que ele percorre todo o input.txt, que é bem longo. E por razão disso, ele acaba representando uma porcentagem tão mais significativa que o restante do codigo

Foram feitas no total 2,689,673 chamadas de instruções e no restante do código, não houve nenhuma porcentagem signficativa que o algoritmo gastou como aconteceu com os valores de input.


------------
## Heurística Aleatória

Essa implementação consiste na adaptação da heurística gulosa. Nela, foi feita uma modificação de modo que ao longo da seleção de um filme se tenha 25% de chance de pegar outro filme qualquer que respeite o horário. Isso fará com que a heurística tenha um pouco mais de exploration e possamos ter alguns resultados melhores.

Para fazer considerações sobre o profiling (valgrind) do código-fonte, foi utilizado também o input.txt (considerando 100 filmes e 5 categorias diferentes).

Os seguintes comandos foram realizados:

> g++ -Wall -O3 -g aleatorizacao.cpp -o aleatorizacao

> valgrind --tool=callgrind ./aleatorizacao < inputs/input1.txt

> callgrind_annotate callgrind.out.5965 aleatorizacao.cpp

**Obs.:** O callgrind foi deixado dentro da pasta callgrinds

Fazendo uma análise total de todo o processamento do código, a parte do código que mais se gasta tempo é no input dos filmes, assim como ocorre na Heurística Gulosa. Como nessa parte do código ambas são equivalentes, o tempo gasto foi praticamente o mesmo, assim como a porcentagem (desta vez foi de 12.79%)

Já acerca do número de instruções, foram registradas 2,675,378, ou seja, aproximadamente 0.53% menor que a Heurística Gulosa, o que não é significativo. Além de que também não teve nenhuma porcentagem signficativa que o algoritmo gastou como aconteceu com os valores de input.

## Comparação entre Tempos de Execução
Em python, ainda é possível fazer uma função para o cálculo do tempo de execução de ambos os executáveis (tal programação se encontra no arquivo GraficosComparativos.ipynb). Ao se plotar o gráfico do Tempo de Execução pelos inputs, foi obtido o seguinte:

![image](https://user-images.githubusercontent.com/49621844/227335541-65443e0e-cd89-434c-bf06-b306987cb222.png)

Nele, é possível obervar que a Heurística Gulosa, praticamente em todos os casos leva um pouco mais de tempo (pouco significativo) que a Heurística Aleatória de ser executado. Logo, de vantagem da Heurística Aleatória tem-se tanto um menor número de instruções necessárias como um menor tempo de execução. Entretanto, ainda é necessário analisar se houve o cumprimento adequado do objetivo principal de mais filmes assistidos na maratona

## Comparação entre Maratonas
Ao se fazer a análise do gráfico do número de filmes na maratona pelos inputs, é visto o seguinte resultado:

![image](https://user-images.githubusercontent.com/49621844/227200410-104e38c1-3a75-4a92-b0cb-2417eb8119ce.png)

Desta forma, é possível analisar que quanto menor o input (número de filmes e categorias menor) mais próximos são os resultados (número total de filmes na maratona) das heurísticas em questão. No gráfico também é percebido que ambas as Heurísticas em certo momento estabilizam o resultado, aumento o numero de filmes e de categorias. No caso da Heurística Gulosa, desde o input3 se estabilizou em 23 o número máximo de filmes possíveis na maratona. Já na Heurística Aleatória, desde o input a tendência dos filmes inclusos na maratona é de 8. O que significa que a partir de certo número de filmes e categorias, chega-se em um resultado mais eficiente em ambos os casos.

## Tempo de Tela
No caso do melhor tempo de tela em ambos os casos consiste, portanto, no input5, com mais filmes e categorias. Baseado nele, foram feitos os cálculos no arquivo GraficosComparativos.ipynb e é obtido o seguinte:
 - Tempo de tela da Heurística Gulosa: 100%
 - Tempo de tela da Heurística Aleatória: 34.78% aproximadamente

Logo, a Heurística Aleatória teve um desempenho muito pior que da Heurística Gulosa, a qual neste caso, conseguiu obter um resultado ótimo. Entretanto, vale salientar que a Heurística Gulosa nem sempre consegue garantir tal desempenho. Além disso, ao observar o resultado do input, depara-se com o seguinte resultado:

![image](https://user-images.githubusercontent.com/49621844/227331180-fd708f59-d8fa-49a1-ac6b-d856081af7c5.png)

Foram assistidos 23 filmes no dia inteiro, o que se conclui que cada filme tinha apenas uma hora de duração (o que foi possível devido ao gerador com desvio padrão de 1.0). Portanto, mudar a duração média de cada filme certamente diminuiria as chances de conseguir esse resultado otimo

## Tempo de Execução X Numero de Filmes
Outro estudo que é possível fazer é a análise do que impacta mais o tempo de execução, o número de filmes ou número de categorias.
Neste caso, os inputs dentro da pasta inputsCatConst foram usados. Os quais variam o número de filmes e deixam as categorias constantes em 10:

|                |        Número de filmes       |    Número de categorias     |
|----------------|-------------------------------|-----------------------------|
|    Input0      |            500                |              10             |
|    Input1      |            1000               |              10             |
|    Input2      |            5000               |              10             |
|    Input3      |            10000              |              10             |

O seguinte gráfico é plotado:

![image](https://user-images.githubusercontent.com/49621844/227524871-20c84284-f8fc-4915-8de1-2696ef721d0e.png)

Nele, é possível observar que quanto maior o número de filmes envolvidos para serem analisados e colocados na maratona, maior o tempo de execução, o que era esperado, indepedente da heurística.

## Tempo de Execução X Numero de Categorias
No caso do estudo do gráfico referente à variação do número de categorias, o número de filmes está constante em 10000 e foram separados os seguintes inputs (dentro da pasta inputsFilmesConst):

|                |        Número de filmes       |    Número de categorias     |
|----------------|-------------------------------|-----------------------------|
|    Input0      |            10000              |              10             |
|    Input1      |            10000              |              15             |
|    Input2      |            10000              |              20             |
|    Input3      |            10000              |              25             |

Além disso, ao criar esse gráfico, foi percebido que a cada plotagem variava bastante entre os valores de tempo de execução. Portanto, foi feito o processo de cálculo do tempo de execução 30x e foi tirada a média deles para se ter uma noção melhor de como o gráfico realmente se comportava. E foi gerado o seguinte gráfico:

![image](https://user-images.githubusercontent.com/49621844/227525695-a41140ca-891a-4e2f-8578-55ac9acca1c5.png)

Com isso, é possível concluir que a variação do número de categorias não acarreta um efeito muito significativo no tempo de execução

## Busca Exaustiva
Outra implementação que foi feita é a Busca Exaustiva, a qual busca encontrar a melhor seleção de filmes para a maratona. O algoritmo testa todas as combinações possíveis de filmes, verificando se a seleção é válida (respeita o limite de filmes por categoria) e calculando a duração total da seleção. Ele armazena a melhor seleção encontrada até o momento. Dado que ele faz essa busca testantando todas as combinações, certamente ela necessita de um maior tempo de processamento. Desta forma,será testado o valgrind com inputs menores a fim de analisar o funcionamento e resultados de forma mais prática.

Para fazer considerações sobre o profiling (valgrind) do código-fonte, foi utilizado então o input0.txt (considerando 10 filmes e 3 categorias diferentes) e o input50.txt

> g++ -Wall -O3 -g exaustiva.cpp -o exaustiva

> valgrind --tool=callgrind ./exaustiva < inputs/input0.txt

> callgrind_annotate callgrind.out.33991 exaustiva.cpp

**Obs.:** O callgrind foi deixado dentro da pasta callgrinds

Ao fazer a análise de todo o processamento do código, primeiramente é tomado nota do número de instrucões, que foi de 2,442,005, ou seja, a menor comparado com as heurísticas anteriores. Neste caso, houveram 3 partes principais do código que mais gastaram tempo. O input dos filmes (1,49%) dessa vez não foi a que teve maior gasto e sim a função da buscaExaustiva dentro da main (2,01%), o que faz sentido, afinal ela é a responsável pela busca exaustiva de todas as combinações possíveis de filmes de forma recursiva e, em cada passo, decide se o filme atual deve ser selecionado ou não. Para tomar essa decisão, a função realiza diversas verificações, como verificar se a seleção atual é válida (respeita o limite de filmes por categoria) e se o filme atual não se sobrepõe a nenhum filme já selecionado:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/fc7d1aa3-81d6-4347-b09f-8102707ee854)

A outra parte do código que consome bastante tempo é a primeira recursiva da função buscaExaustiva (1,38%), pois embora essa chamada recursiva possa reduzir o espaço de busca ao não selecionar o filme atual, ainda é necessário percorrer todos os filmes restantes e verificar todas as combinações possíveis. Portanto, essa parte do código também consome um tempo significativo de processamento:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/b28fa279-6a4d-4b6a-b4f5-f29d207c5e2a)


**Obs.:** Uma possível razão da segunda recursiva do código desta mesma função não ter um gasto significativo como as outras (0,63%) é por causa da verificação de sobreposição (hasOverlap), o que pode ajudar a reduzir o espaço de busca, evitando a análise de combinações que já sabemos que não são válidas. Isso pode levar a uma economia de tempo de processamento significativa, pois evita a exploração de ramos desnecessários no espaço de busca:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/5ce642e7-33e3-46e3-be66-0caa236eac94)


## Busca Exaustiva x Heurística Gulosa
Dentre as heurísticas estudadas anteriormente, a heurística gulosa teve o melhor desempenho. Então, será feita uma comparação deste novo algoritmo de busca exaustiva com ela.

Dado que a Busca Exaustiva exige um tempo de processamento elevado, serão analisados inputs menores. Por exemplo, no input0.txt que possui apenas 10 filmes e 2 categorias, já é possível analisar uma melhoria na seleção de filmes para a maratona:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/a07ac850-e51f-4379-94a8-598c69a25be7)

Enquanto isso na Gulosa:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/b2f007d8-5855-4939-a8cb-a86ac27eeb4e)

Havendo uma melhora no tempo de tela de 3 horas e mantendo a mesma quantidade de filmes.

Outros inputs que foram utilizados para análise foram: input15.txt, input25.txt, input35.txt e input50.txt:

|                |        Número de filmes       |    Número de categorias     |
|----------------|-------------------------------|-----------------------------|
|    Input0      |            10                 |              3              |
|    Input15     |            15                 |              2              |
|    Input25     |            25                 |              3              |
|    Input35     |            35                 |              4              |
|    Input50     |            50                 |              5              |

Para uma melhor visualização, foi feito um gráfico da eficiência do tempo de tela na maratona em cada um dos inputs:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/c4187cdb-4320-4298-90d8-f58e6fd2b829)

Em todos os casos, como esperado, a busca exaustiva teve um desempenho melhor. Entretanto, o tempo de execução é muito mais demorado. Até 35 filmes a busca exaustiva consegue ter um tempo de processamento maior que a gulosa, mas ainda razoável. Já colocando 50 filmes, ela aumenta o tempo de execução em 24 segundos:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/14a796e1-54cb-468a-ba70-7bcc6d7cd287)

## Exaustiva com OpenMP

Outra implementação feita no estudo é a utilização de OpenMP junto com a busca exaustiva. O OpenMP permitirá que os desenvolvedores escrevam programas que podem ser executados de forma eficiente em sistemas paralelos, logo é esperado que o tempo de execução seja menor.

Nessa versão, a busca exaustiva é distribuída entre as threads usando a diretiva #pragma omp for, enquanto a região crítica de atualização das melhores soluções é protegida por #pragma omp critical. Cada thread terá sua própria cópia das variáveis selecaoAtual e cntCat para evitar condições de corrida.

Cada thread realizará sua própria busca exaustiva dentro de uma parte específica do espaço de busca e, no final, as melhores soluções serão combinadas para obter o resultado final.

Como resultado do tempo de execução, ele teve um desempenho pior do que o esperado:

![image](https://github.com/GabrielaMitu/Supercomp-MaratonaFilmes/assets/49621844/0c3308ba-20ba-4deb-915b-94c3afc0f285)

Como é possível observar, ele teve um tempo de execução equivalente ao dobro do que a busca exaustiva. Uma das razões é que a busca exaustiva em si é um algoritmo de complexidade exponencial, o que significa que o tempo de execução aumenta rapidamente à medida que o tamanho do problema aumenta. Além disso, o uso de regiões críticas e a sincronização entre threads introduzidas pelas diretivas #pragma omp critical podem ter um impacto negativo no desempenho quando o número de threads aumenta.

## Paralelismo com GPU

Esta etapa do projeto consiste em resolver nosso problema por meio da biblioteca Thrust.

Para resolver esse problema utilizando a biblioteca thrust, podemos utilizar um algoritmo de programação dinâmica para construir a solução de forma eficiente. O algoritmo consiste em criar uma matriz dp de tamanho (N+1) x (M+1) para armazenar o número máximo de filmes que podem ser assistidos até o filme i e a categoria j.

O código usa dois loops for aninhados para percorrer todas as combinações possíveis de filmes e categorias.
- O loop externo percorre os filmes de 1 até n (número de filmes disponíveis para a maratona no input).
- O loop interno percorre as categorias de 1 até m (número de categorias).

Para cada combinação de filme e categoria, o código verifica se é possível selecionar o filme atual, levando em consideração as restrições da categoria e a seleção anterior de filmes.

Ele cria uma variável max_count para armazenar o número máximo de filmes selecionados até o momento para a combinação atual.

Em seguida, há um terceiro loop for que percorre os filmes anteriores (até o índice i-1) para comparar com o filme atual.

Dentro desse terceiro loop, o código verifica se o filme anterior pertence à categoria atual (filmeAnterior.categoria == j), se seu horário de término é menor ou igual ao horário de início do filme atual (filmeAnterior.fim <= filmeAtual.inicio), e se a quantidade máxima de filmes permitida para essa categoria ainda não foi atingida (dp[(k * (m + 1)) + j - 1] + 1 <= max_filmes_dev[j - 1]).

Se todas essas condições forem verdadeiras, o código atualiza max_count para o valor máximo entre max_count e o número de filmes selecionados até o filme anterior na categoria anterior, mais 1 (dp[(k * (m + 1)) + j - 1] + 1).

Caso contrário, se as condições não forem atendidas, o código atualiza max_count para o valor máximo entre max_count e o número de filmes selecionados até o filme anterior na mesma categoria (dp[(k * (m + 1)) + j]).

Após o loop sobre os filmes anteriores, o código atribui o valor max_count à posição correspondente na matriz de programação dinâmica dp para a combinação atual (dp[(i * (m + 1)) + j] = max_count).

Em seguida, o código verifica se max_count é maior que o número máximo de filmes já selecionados (max_count > maxSelecionados). Se for, atualiza maxSelecionados com o valor de max_count e a duração máxima total com a diferença entre o horário de término e o horário de início do filme atual (duracaoMaxTotal = filmeAtual.fim - filmeAtual.inicio). O processo continua para todas as combinações de filmes e categorias.

Essa etapa de programação dinâmica permite determinar a seleção ótima de filmes para a maratona, considerando as restrições de categorias e horários de início e término dos filmes. O resultado final será armazenado na matriz dp e usado posteriormente para reconstruir a seleção ótima de filmes.
