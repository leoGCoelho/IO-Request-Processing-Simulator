# Trabalho 3 - Arquitetura e Organização de Computadores II - 2018/1
Prof. Maurício Lima Pilla - http://lups.inf.ufpel.edu.br/~pilla

## Dados do(a)s aluno(a)s

Declaro que o presente trabalho contém código desenvolvido exclusivamente por mim e que não foi compartilhado de nenhuma forma com terceiros a não ser o professor da disciplina. Compreendo que qualquer violação destes princípios será punida rigorosamente de acordo com o Regimento da UFPEL.

(Preencha com seus dados)

- Nome completo: Leonardo Gularte Coelho
- Username do Bitbucket: LeoGCoelho
- Email @inf: lgcoelho@inf.ufpel.edu.br


## Descrição

Este trabalho pode ser feito em grupos de dois alunos. No entanto, os grupos devem ser organizados de forma que apenas alunos que entregaram pelo menos um dos trabalhos podem formar grupos junto. Os alunos que não entregaram trabalhos podem formar grupos entre si.


Um processador possui uma controladora para tratar interrupções, as quais possuem prioridades de 1 a 7 (menor identificador significa maior prioridade).

O barramento de dados e endereços possui frequência de operação de 1 GHz e transmite 8~bits por ciclo. Há ainda barramentos de controle. Um endereço no barramento consiste de 1~byte. 

Os dispositivos são organizados da seguinte forma:

| IRQ | DISPOSITIVO | PEDIDO (ciclos) | LATÊNCIA | TRANSMISSÃO | TIPO |
| --- | --- | --- | --- | --- | --- |  
| 1 | Disco rígido | 6 | 5 ms | 80 MiB/s | DMA |
| 2 | Disco rígido | 6 | 7 ms | 60 MiB/s | DMA |
| 3 | USB | 6 | 1 ms | 40 MiB/s | DMA |
| 4 | USB | 6 | 1 ms | 20 MiB/s | DMA |
| 5 | USB | 6 | 1 ms | 20 MiB/s | Interrupção |
| 6 | Teclado | 4 | 100 ns | 10 KiB/s | Interrupção |
| 7 | Porta serial | 6 | 10 ns | 1 KiB/s | Polling | 



O tipo de um pedido define como o barramento será utilizado. Por exemplo, um pedido de tipo DMA levará os ciclos de pedido e confirmação para começar usando CPU, barramento, memória e dispositivo. Depois, a latência é o tempo de espera que o dispositivo fica esperando para começar a transferência. Neste tempo, os demais recursos estão disponíveis para outros pedidos. Durante o tempo de transferência, o dispositivo, o barramento e a memória estão ocupados. Finalmente, o dispositivo deve gerar uma interrupção. Considere que qualquer interrupção leva 2~ciclos, ocupando barramento, CPU e o dispositivo.

O árbitro do barramento é responsável por decidir quem deve usar o barramento ocioso, levando em conta a prioridade. No entanto, considere que uma operação em andamento no barramento nunca é interrompida. 

**Lembre de transformar as latências e tempos de transmissão para ciclos**.

## Entrada e saída

A entrada consiste de um vetor de pedidos. Cada pedido tem o identificador do dispositivo (mesmo número que a interrupção) e o número de bytes a serem transferidos. Considere que todos os pedidos estão esperando para serem realizados no tempo zero.

        struct req{
            int device;
            int length;
        };


O simulador deve validar todos os pedidos e, caso algum não seja válido, deve ser desconsiderado.

O simulador implementado retorna o número de ciclos necessários para resolver todos os pedidos. Caso não haja pedidos válidos, deve retornar **-1**. A assinatura da função é:

        int sim(struct req * devices, int num_devices);

A saída consiste no número de ciclos que foram necessários para tratar todas as requisições.

## Testes do aluno

Cada grupo deverá fornecer seus próprios casos de teste. Os testes deverão verificar:

- Se as entradas estão corretamente definidas;
- Se a simulação para cada um dos dispositivos está funcionando isoladamente (um pedido apenas em cada simulação);
- Se simulações com múltiplos dispositivos está funcionando.

**Cada caso de teste deverá ser documentado com uma explicação do que está sendo testado, o resultado esperado e os cálculos que foram realizados para chegar no valor final.**

## Produtos

Os seguintes produtos devem ser entregues:

* Implementação (*sim.h* e *sim.c*) -- 8 pontos 
* Casos de teses do(a) aluno(a) (*aluno.c*) -- 2 pontos 
* Makefile e configuração do pipeline -- 0 pontos (mas o trabalho não vai funcionar se não estiverem corretos...)

O pipeline do Bitbucket deve estar executando ambos os testes (primeiro o teste do aluno, depois o do professor) em duas regras:

        make aluno
        make grade

**Grupos com _commits_ predominantemente de um componente apenas terão descontos na nota.**

## Cronograma

* Primeira discussão em aula: _29/06/2018_
* Segunda discussão em aula: _06/07/2018_
* Terceira discussão em aula: _13/07/2018_
* Envio da versão final: _21/07/2018_ 
* Apresentação dos trabalhos: _27/07/2018_ 


## Sugestão de implementação

O trabalho pode ser implementado usando múltiplas listas, uma por prioridade. Cada lista contém um pedido, a fase em que ele está e o ciclo em que o próximo evento acontece. As listas são ordenadas por ciclo. 

Um _scoreboard_ mantém quais recursos (dispositivos, CPU, barramento e memória) estão sendo usados e em que ciclo serão liberados.

O simulador contém um relógio global com o ciclo da simulação. A cada rodada, o simulador verifica em ordem de prioridade as listas, procurando por pedidos cujo ciclo seja menor ou igual ao atual e que todos os recursos estejam disponíveis. 

Caso todos os recursos estejam disponíveis, são marcados no _scoreboard_, o pedido é adicionado novamente à lista marcado para a próxima fase com o ciclo já calculado.

A simulação termina quando todas as listas estão vazias.



