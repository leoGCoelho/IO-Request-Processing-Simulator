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

void test_1(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {1,1};
	/* pedido = 6 ciclos
	   espera 5ms = 5000000 ciclos
	   1B a 80 MiB/s = 83886080 B/s = 1/83886080*10^9 ns
	   = 12 ciclos
	   + 18 = 5000021 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,5000021,1);
}

void test_2(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {2,1};
    /* pedido = 6 ciclos
       int = 2
	   espera 7ms = 7000000 ciclos
	   1B a 60 MiB/s = 62814560 B/s = 1/62814560*10^9 ns
	   = 17 ciclos
	   + 25 = 7000025 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,7000025,1);
}

void test_3(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {3,1};
    /* pedido = 6 ciclos
       int = 2
	   espera 1ms = 1000000 ciclos
	   1B a 40 MiB/s = 41943040 B/s = 1/41943040*10^9 ns
	   = 24 ciclos
	   + 32 = 1000032 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,1000032,1);
}

void test_4(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {4,1};
    /* pedido = 6 ciclos
       int = 2
	   espera 1ms = 1000000 ciclos
	   1B a 20 MiB/s = 20971520 B/s = 1/20971520*10^9 ns
	   = 48 ciclos
	   + 56 = 1000056 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,1000056,1);
}

void test_5(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {5,1};
    /* pedido = 6 ciclos
       int = 2
	   espera 1ms = 1000000 ciclos
       1B a 20 MiB/s = 20971520 B/s = 1/20971520*10^9 ns
	   = 48 ciclos
	   + 56 = 1000056 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,1000056,1);
}

void test_6(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

	IF("Um pedido para um dispositivo");
	THEN("Deve terminar no tempo calculado");
	struct req req1 = {6,1};
    /* pedido = 4 ciclos
       int = 2
	   espera 100ns = 100 ciclos
	   1B a 10 KiB/s = 10240 B/s = 1/10240*10^9 ns
	   = 97657 ciclos
	   + 106 = 97763 */
	ciclos = sim(&req1,1);
	isEqual(ciclos,97763,1);
}

void test_7(){
    int ciclos=0;

	DESCRIBE("Teste de pedido para dispositivo 1");

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

void test_mult(){
	int ciclos=0;

	DESCRIBE("Teste de dispositivo Interrupção com pedidos extras");

	IF("Dois pedidos para um dispositivo");
	THEN("Deve terminar no dobro do tempo calculado para um pedido");
	struct req req1[] = {{1,1},{1,1}};
		/*
        A = 5000021 * 2 = 10000040 ciclos
	*/


	ciclos = sim(req1,2);
	isEqual(ciclos,10000040,1);



	IF("3 pedidos para 3 dispositivo");
	THEN("Deve terminar mais rapidamente");
	struct req req2[] = {{1,1024*1024},{6,2},{1,1}};
	/* A = 6 ciclos
	   B = 6 ciclos
       C = 5000021 ciclos



		A _6_  espera 5.000.000 _12.500.000_ _2_
		B    _6_ espera 100 _2_ _195.313_

		17.500.008 ciclos (tempo de A)
        + C = 22500029 ciclos
	*/


	ciclos = sim(req2,3);
	isEqual(ciclos,22500029,1);

    IF("4 pedidos para 3 dispositivo");
	THEN("Deve terminar mais rapidamente");
	struct req req3[] = {{1,1024*1024},{6,2},{1,1},{1,1024*1024}};
	/* A = 17.500.008 ciclos
	   B = 195.419 ciclos
       C = 5000021 ciclos
       D = A



		2A + B + C = 40000037 ciclos
	*/


	ciclos = sim(req3,4);
	isEqual(ciclos,40000037,1);

}



int main(){
    test_parametros();
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_mult();


    GRADEME();

    if (grade==maxgrade)
        return 0;
    else return grade;
}
