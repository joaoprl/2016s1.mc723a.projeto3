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
O software escolhido para testar o periférico acelerador e o paralelismo feitos no simulador é um programa que calcula os retornos de capital de investidores com diferentes montantes iniciais, taxas de retorno e períodos de investimento.  
Este é um problema extremamente relevante no contexto bancário, especialmente devido a certos tipos de investimento usarem taxas de retorno mensais mas com tempo calculado em dias de investimento, tal que a exponenciação se dá com expoentes em ponto flutuante.  
Desta forma os _inputs_ do programa foram 3 arquivos contendo os [montantes iniciais](mips_multi_lock_multiplyperipheral_example/y4k_initial_capital.txt), as [taxas de retorno](mips_multi_lock_multiplyperipheral_example/monthly_interest.txt), e os [períodos de investimento](mips_multi_lock_multiplyperipheral_example/y4k_investment_time.txt). Todos são precedidos pelo número de clientes para os quais o retorno dos investimentos serão calculados, sendo dez mil investimentos neste exemplo.

Como a arquitetura de CPU utilizada foi a MIPS, operações de ponto flutuante são muito custosas visto que não existem unidades aritiméticas dedicadas para este tipo de operação; assim foi projetado um periférico que realiza as exponenciações fora da CPU. Foi discutido incluir também mais uma variável (capital inicial) a ser passada ao periférico para que ela fosse multiplicada pelo resultado da exponenciação, tal que seriam sanadas completamente as limitações da CPU com relação a estas operações de ponto flutuante; no entanto esta ideia (apesar de ser simplesmente implementável) não foi usada visto que este é um periférico dedicado para exponenciação, sendo considerada uma modificação excessivamente restrita ao cenário considerado. Assim a multiplicação do capital inicial pela taxa de retorno total continuou sendo realizado pela CPU, o que limita o ganho de performance mas torna o periférico mais simples e de uso mais geral.  

## Periférico de aceleração e paralelismo da CPU
Um periférico de aceleração de exponenciais pode utilizar para testes o método de cálculo de exponenciais utilizando tabelas pré-processadas de logaritmos e exponenciais. Dessa forma, o acelerador possui uma memória do tipo read-only que permite calcular com boa precisão e boa eficiência uma exponencial em ponto flutuante. O periférico é um coprocessador de exponenciação paralela usando a metodologia de _lookup table_ de logarítimos e exponenciais na base 2; no cenário projetado este hardware foi desenvolvido especificamente para o processador deste sistema, tal que cada _core_ tem acesso exclusivo a uma unidade aritmética de exponenciação do periférico, não havendo concorrência de acesso.  

Outra característica interessante deste periférico é que como apenas a mantissa (23 bytes do 32 totais) das variáveis de ponto flutuante será utilizada como indexador tanto da tabela de logarítimos quanto da tabela de exponenciais, cada tabela ocupará apenas 32MB de ROM.  

Desta forma o periférico realiza, independentemente para cada _core_, a exponenciação com a taxa de retorno como base e o valor do período como expoente para se obter o a taxa total de rendimento. Cada core então multiplica este resultado pelo montante inicial para obter o resultado final. Obviamente ele não foi projetado para lidar com números complexos, tal que o resultado para qualquer combinação de base negativa e expoente com parte fracional retorna um `nan`.

Resumidamente, o fluxo de dados entre cada _core_ e cada núcleo exclusivo de processamento do periférico se dá através de 3 endereços de 32 bits: _status_, _base_, e _exponent_. O _core_ escreve os valores de taxa de retorno e período de investimento em _base_ e _exponent_, então escrevendo `1` em _status_ para que aquele núcleo do periférico comece a exponenciação. Em seguida o _core_ fica em _loop_ até que o valor de _status_ mude para `0`, que é como o núcleo sinaliza que a operação foi concluída e o resultado está disponível no endereço _base_.

Na simulação o _loop_ leva apenas um ciclo, visto que o cálculo do número de ciclos gasto pelo periférico será realizado a parte a partir das considerações abaixo.

## Ciclos consumidos por cada operação do periférico
Cada operação de exponenciação emulada em ponto flutuante ocupa o surpreendentemente alto valor de ~43k ciclos, enquanto o periférico de aceleração idealmente totalizará apenas 25 ciclos na seguinte sequência de operações abaixo (onde _x_ e _y_ representam respectivamente a _base_ e _exponent_).  

|operação|descrição|ciclos|
|:---|:---|---:|
|x = sx · 2 ^ ex|separar sx (mantissa) e ex (expoente)|1|
|a = lg2(sx)|acessar tabela de logarítimos usando o valor de sx (23b)|2|
|b = lg2(x) = ex · a|multiplicação|1|
|c = lg2(x ^ y) = y · b|multiplicação|1|
|c = nc + fc|separar nc (parte inteira) e fc (parte fracionária)|1|
|d = x ^ y / 2 ^ nc = 2 ^ fc|acessar tabela de exponenciais usando o valor de fc (<= 23b)|10|
|x ^ y = d * 2 ^ nc |aumentar o valor do expoente de d em 2 ^ nc|1|

## Aplicações
Possíveis aplicações encontradas foram de minimização de monômios multivariado, posinômios [3] [4] e métodos que encontram raízes de polinômios (como o método de Newton-Raphson) [5], assim como diversas aplicações que utilizam tais soluções.
## Referências
1. http://www.ic.unicamp.br/~lucas/teaching/mc723/2016-1/p3.html
2. http://turner.faculty.swau.edu/mathematics/math110de/materials/logtable/
