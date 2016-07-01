MC723a - Laboratório de Projetos de Sistemas Computacionais
====
###### 2016-07-01
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
Um periférico de aceleração de exponenciais pode utilizar para testes o método de cálculo de exponenciais utilizando tabelas pré-processadas de logaritmos e exponenciais. Dessa forma, o acelerador possui uma memória do tipo read-only que permite calcular com boa precisão e boa eficiência uma exponencial em pontos flutuante. O periférico é um coprocessador de exponenciação paralela usando a metodologia de _lookup table_ de logarítimos e exponenciais na base 2; no cenário projetado este hardware foi desenvolvido especificamente para o processador deste sistema, tal que cada _core_ tem acesso exclusivo a uma unidade aritmética de exponenciação do periférico, não havendo concorrência de acesso.  

Outra característica interessante do é que como apenas a mantissa (23 bytes do 32 totais) das variáveis de ponto flutuante será utilizada como indexador tanto da tabela de logarítimos quanto da tabela de exponenciais, cada tabela ocupará apenas 32MB de ROM.

Desta forma cada core realizara a exponenciação com o valor `a` da primeira e o valor `b` da segunda, para se obter o resultado `a ^ b`, tal que cada seção destes _arrays_ será calculado por cada um dos _cores_ com apoio do periférico.

Entretando, para efeitos de simulação foi utilizada a função pow da biblioteca Math.h, visto que a potencial precisão do método que utiliza a tabela de logaritmos se assemelha à função escolhida.

## Ganhos de _performance_ 
Considerando que a arquitetura nativa não realiza operações de ponto flutuante, o ganho de desempenho foi significativo: quando acelerado, o código executou cerca de 20 vezes mais rapido.

## Aplicações
Possíveis aplicações encontradas foram de minimização de monômios multivariado, posinômios [3] [4] e métodos que encontram raízes de polinômios (como o método de Newton-Raphson) [5], assim como diversas aplicações que utilizam tais soluções.
## Referências
1. http://www.ic.unicamp.br/~lucas/teaching/mc723/2016-1/p3.html
2. http://turner.faculty.swau.edu/mathematics/math110de/materials/logtable/
3. http://arxiv.org/pdf/1002.1167.pdf
4. https://stanford.edu/~boyd/papers/pdf/gp_tutorial.pdf
5. https://en.wikipedia.org/wiki/Newton's_method

#### PS: Apresentação
Se o horário estiver disponível, deseja-se marcar a apresentação do grupo para 09:45.  

