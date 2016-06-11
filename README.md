MC723a - Laboratório de Projetos de Sistemas Computacionais
====
###### 2016-06-10
#### Professor: Lucas Wanner
#### 033342 - Yugo Kuno
#### 139546 - João Lopes

Projeto 3 - Multicore e Aceleração em Hardware
====

## Objetivo
Utilizando um simulador de arquitetura _MIPS_, rodar um programa em um sistema com múltiplos cores e acesso a um periférico extra que auxilie no processamento; avaliar os ganhos de desempenho devidos ao paralelismo multicore e à aceleração devido ao periférico comparado a um sistema single-core sem acesso ao hardware extra.  
O simulador utilizado foi criado com a linguagem _ArchC_, desenvolvidos pelo time do IC/Unicamp de mesmo nome e baseado na linguagem de descrição de hardware _SystemC_, tendo recebido também mais contribuições por outros pesquisadores do instituto.  

## Software a ser executado
O software escolhido para testar o periférico acelerador e o paralelismo feitos no simulador é um programa que calcula um número arbitrário de exponenciais a partir de duas _arrays_ de mesmos tamanhos, a primeira contendo os valores a serem exponenciados e a segunda os valores de potencias. Cada exponenciação será acelerada com o periférico e o paralelismo ocorrerá com a divisão das operações de exponenciação entre os _cores_ da CPU.  

## Periférico de aceleração e paralelismo da CPU
Será utilizado para testes o método de cálculo de exponenciais utilizando tabelas pré-processadas de logaritmos e exponenciais. Dessa forma, o acelerador deverá possuir uma memória do tipo read-only que permite calcular com boa precisão e boa eficiência uma exponencial em pontos flutuante. O periférico é um coprocessador de exponenciação paralela usando a metodologia de _lookup table_ de logarítimos e exponenciais na base 2; no cenário projetado este hardware foi desenvolvido especificamente para o processador deste sistema, tal que cada _core_ tem acesso exclusivo a uma unidade aritmética de exponenciação do periférico, não havendo concorrência de acesso.  

Outra característica interessante do periférico é que como apenas a mantissa (23 bytes do 32 totais) das variáveis de ponto flutuante será utilizada como indexador tanto da tabela de logarítimos quanto da tabela de exponenciais, cada tabela ocupará apenas 32MB de ROM.

Desta forma cada core realizará a exponenciação com o valor `a` da primeira e o valor `b` da segunda, para se obter o resultado `a ^ b`, tal que cada seção destes _arrays_ será calculado por cada um dos _cores_ com apoio do periférico.  

## Aceleração esperada
Considerando que a arquitetura nativa não realiza operações de ponto flutuante, espera-se um ganho de desempenho significativo não só pela metodologia de _lookup table_, mas também devido às unidades de multiplicação e soma de ponto flutuante internas do periférico.  

O cenário de testes envolverá a leitura de um arquivo contendo os valores dos dois arrays de entrada, o cálculo do array de saída e a escrita da saída em outro arquivo.  
Em testes realizados com o simulador realizando 10000 exponenciações, as operações de exponenciação ocupam ~86.23% dos ciclos do programa, enquanto a a leitura/escrita de arquivo e o resto do arcabouço ocupam somente ~13.77%.  

Cada operação de exponenciação emulada em ponto flutuante ocupa o surpreendentemente alto valor de ~43k ciclos, enquanto o periférico de aceleração idealmente totalizará apenas 25 ciclos na seguinte sequência de operações (assumido 0 < x devido ao expoente em ponto flutuante; caso deseje-se um expoente inteiro e permita-se o x <= 0 o número de operações finais não muda significativamente).  

|operação|descrição|ciclos|
|:---|:---|---:|
|x = sx · 2 ^ ex|separar sx (mantissa) e ex (expoente)|1|
|a = lg2(sx)|acessar tabela de logarítimos usando o valor de sx (23b)|10|
|b = lg2(x) = ex · a|multiplicação|1|
|c = lg2(x ^ y) = y · b|multiplicação|1|
|c = nc + fc|separar nc (parte inteira) e fc (parte fracionária)|1|
|d = x ^ y / 2 ^ nc = 2 ^ fc|acessar tabela de exponenciais usando o valor de fc (<= 23b)|10|
|x ^ y = d * 2 ^ nc |aumentar o valor do expoente de d em 2 ^ nc|1|

Isso representaria uma redução de 99.9% no número de ciclos perdidos em exponenciação, o que hipoteticamente reduziria o número total de ciclos do programa para ~13.83% do valor original, o que corresponde em linhas gerais para apenas os tempos de acesso aos arquivos de entrada e saída. É claro que quanto menos exponenciações menos dramática é a diferença, e vice-versa.  

O uso do paralelismo com 2 _cores_, sem aceleração, reduziria o percentual total de ciclos consumidos para ~56.89%; com aceleração, o percentual final cairia para ~13.79%.  

## Referências
1. http://www.ic.unicamp.br/~lucas/teaching/mc723/2016-1/p3.html
2. http://turner.faculty.swau.edu/mathematics/math110de/materials/logtable/

## PS: Apresentação
Se o horário estiver disponível, deseja-se marcar a apresentação do grupo para 09:45.  

