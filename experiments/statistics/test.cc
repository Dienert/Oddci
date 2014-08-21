#include "statistic.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace statistic;

int main4(int argc, char *argv[])
{
	int entrada;
	double ci = 95;
	Statistic *test;
	char *lixo;

	test = new Statistic( "testando amostras", true ); 

	if (argc > 1) {
		ci = strtod(argv[1],&lixo);
	}

	cout << "Teste da classe TStatistic\n";
	cout << "Digite os valores da amostra, um por linha (digite \"-1\" para terminar):\n";
	cin >> entrada;
	while (entrada != -1) {
		test->addObservation(entrada);
		cin >> entrada;
	}


	test->setConfidenceLevel(ci);
	test->print( stdout );
	
	cout << "\nO resumo estatistico dessa amostra eh:";
	cout << "\nTamanho............................: " << test->getSampleSize();
	cout << "\nMin................................: " << test->getSampleMin();
	cout << "\nMax................................: " << test->getSampleMax();
	cout << "\nMedia..............................: " << test->getSampleMean();
	cout << "\nDesvio padrao amostral.............: " << test->getSampleStandardDeviation();
	cout << "\nNivel de Confianca.................: " << test->getConfidenceLevel();
	cout << "\nIntervalo Confianca................: " << test->getConfidenceInterval();
	cout << "\nPrecisao do intervalo de confianca.: " << test->getPrecision() << endl;

	cout << "\nInforme agora a precisao desejada para calcular o numero de replicacoes necessarias: " << endl;
	cin >> entrada;
	cout << "\nPara uma precisao de " << entrada << ", sao necessarias " << test->getRequiredNumberOfReplications((double)entrada) << " replicacoes." << endl;
	return 0;
}
