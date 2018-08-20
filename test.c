#include "sim.h"
#include "simplegrade.h"






void test_parametros(){
	struct req reqs[1] = {{1,2}};
	DESCRIBE("Testa operação com parâmetros inválidos");

	IF("Número de requests for zero");
	THEN("Não deve executar");
	isEqual(sim(reqs, 0),-1,1);

	IF("Número de requests for menor que zero");
	THEN("Não deve executar");
	isEqual(sim(reqs, -5),-1,1);

	IF("Vetor de requests for NULL");
	THEN("Não deve executar");
	isEqual(sim(NULL,1),-1,1);

}


/**
| IRQ | DISPOSITIVO | PEDIDO (ciclos) | LATÊNCIA | TRANSMISSÃO | TIPO |
| --- | --- | --- | --- | --- | --- |
| 1 | Disco rígido | 6 | 5 ms | 80 MiB/s | DMA |
| 2 | Disco rígido | 6 | 7 ms | 60 MiB/s | DMA |
| 3 | USB | 6 | 1 ms | 40 MiB/s | DMA |
| 4 | USB | 6 | 1 ms | 20 MiB/s | DMA |
| 5 | USB | 6 | 1 ms | 20 MiB/s | Interrupção |
| 6 | Teclado | 4 | 100 ns | 10 KiB/s | Interrupção |
| 7 | Porta serial | 6 | 10 ns | 1 KiB/s | Polling |
*/

void test_polling(){
	int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo com polling");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {7,1};
	/* pedido = 6 ciclos
	   espera 10ns = 10 ciclos
	   1B a 1 KB/s = 1024 B/s = 1/1024*10^9 ns
	   = 976.562,5 ns = 977.563 ciclos
	   + 16 = 976.579 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,976579,1);
}


void test_int(){
	int ciclos=0;

	DESCRIBE("Teste de dispositivo Interrupção");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {6,2};
	/* pedido = 4 ciclos
	   espera 100ns = 100  ciclos
	   int = 2 ciclos
	   2 B/ 10KiB/s = 0.00019531250 s = 195312,5 = 195.313 ciclos
	   195.419 ciclos
	*/

	ciclos = sim(&req1,1);
	isEqual(ciclos,195419,1);
}

void test_DMA(){
	int ciclos=0;

	DESCRIBE("Teste de dispositivo DMA");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {1,1024*1024};
	/* pedido = 6 ciclos
	   1 ms = 10^6 ns
	   espera 5ms = 5*10^6 = 5.000.000  ciclos
	   1 MiB/80 MiB/s = 12.500.000 ciclos
	   + 2 ciclos interrupcao
	   17.500.008 ciclos
	*/

	ciclos = sim(&req1,1);
	isEqual(ciclos,17500008,1);

}


void test_duas_int(){
	int ciclos=0;

	DESCRIBE("Teste de dispositivo Interrupção com pedidos extras");

	IF("Dois pedidos para um dispositivo");
	THEN("Deve terminar no dobro do tempo calculado para um pedido");
	struct req req1[] = {{6,2},{6,2}};
		/* pedido = 4 ciclos
	   espera 100ns = 100  ciclos
	   int = 2 ciclos
	   2 B/ 10KiB/s = 0.00019531250 s = 195312,5 = 195.313 ciclos
	   195.419 ciclos
	   * 2 = 390.838
	*/


	ciclos = sim(req1,2);
	isEqual(ciclos,390838,1);



	IF("Dois pedidos para dois dispositivo");
	THEN("Deve terminar mais rapidamente");
	struct req req2[] = {{1,1024*1024},{6,2}};
	/* pedido A = 6 ciclos
	   pedido B = 6 ciclos



		A _6_  espera 5.000.000 _12.500.000_ _2_
		B    _6_ espera 100 _2_ _195.313_

		17.500.008 ciclos (tempo de A)
	*/


	ciclos = sim(req2,2);
	isEqual(ciclos,17500008,1);

}

void test_ordenamento(){
	int ciclos=0;

	DESCRIBE("Teste de ordenamento de pedidos");

	IF("Dois pedidos feitos na ordem inversa");
	THEN("Deve terminar no tempo calculado");
	struct req req1[] = {{6,2},{1,1024*1024}};
	/* pedido B = 6 ciclos
	   pedido A = 6 ciclos



		B _6_  espera 5.000.000 _12.500.000_ _2_
		A    _6_ espera 100 _2_ _195.313_

		17.500.008 ciclos (tempo de B)
	*/


	ciclos = sim(req1,2);
	isEqual(ciclos,17500008,1);

}





int main(){

	test_parametros();
	test_polling();
	test_int();
	test_DMA();
	test_duas_int();
	test_ordenamento();



	GRADEME();

	if (grade==maxgrade)
		return 0;
	else return grade;



}
