MC723 - Laboratório de Projetos de Sistemas Computacionais
====
###### Campinas, 10 de junho de 2016
#### Professor: Lucas Wanner
#### 033342 - Yugo Kuno
#### 139546 - João Lopes

Projeto 3 - Multicore e Aceleração em Hardware
====

## Objetivo
Simular, na arquitetura m1ips, paralelismo e aceleração em hardware utilizando o simulador archc.

## Introdução
Será utilizado para testes o método de cálculo de exponenciais utilizando tabelas pré-processadas de logaritmos. Dessa forma, o acelerador deverá possuir uma memória do tipo read-only que permite calcular com boa eficiência uma exponencial de pontos flutuantes. Juntamente estará disponível no mesmo periférico as operações elementares sob pontos flutuantes. O periférico será capaz de lidar com diversas operações simultâneas, visando a ser utilizado em operações paralelas. 

Software: 

Periférico de aceleração em hardware: exponenciais

Periféricos: Lock (para determinar região crítica do paralelismo do software implementado)

Experimentos: 

Aceleração esperada: 

## Referências
1. http://www.ic.unicamp.br/~lucas/teaching/mc723/2016-1/p3.html
