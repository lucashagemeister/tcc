#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include <vector>
#include <iterator>

#define DONT_CARE = 9;

using namespace std;



/* FUNÇÕES PARA RECEBER A FUNÇÃO BOOLEANA DE ENTRADA E CRIAR A TABELA-VERDADE */
int solicitarNumeroInputs() {
	int numeroInputs;
	cout << "Insira o numero de inputs da funcao booleana: ";
	cin >> numeroInputs;
	return numeroInputs;
}

int calcularNumeroMinTermos(int inputs) {
	return pow(2, inputs);
}

void solicitarMinTermos(int m, int* v) {
	cout << "Ok! Entao esta funcao booleana tem " << m << " mintermos/linhas. Informe os valores de cada mintermo" << endl;
	for (int i = 0; i < m; i++) {
		cout << "Qual o valor do mintermo m" << i << "? ";
		cin >> v[i];
	}

}

int** criarTabelaVerdade(int linhas, int colunas) {
	int** tabelaVerdade;

	tabelaVerdade = (int**)malloc(linhas * sizeof(int*));

	for (int i = 0; i < linhas; i++) {
		tabelaVerdade[i] = (int*)malloc(colunas * sizeof(int));
	}


	for (int i = 0; i < linhas; i++) {
		for (int j = 0; j < colunas; j++) {
			int x = pow(2, j);
			tabelaVerdade[i][colunas - j - 1] = (i & x) ? 1 : 0;
		}
	}

	return tabelaVerdade;
}

/* TRANSFORMAR A FUNÇÃO BOOLEANA NO FORMATO ISOP - ALGORITMO DE QUINE-MCCLUSKEY*/

list<list<int>> criarFormatoSOP(int** tabelaVerdade, int* minTermos, int numeroInputs, int numeroMinTermos) {

	list<list<int>> sop;
	list <int> line;
	for (int i = 0; i < numeroMinTermos; i++) {
		if (minTermos[i] == 1) {
			for (int j = 0; j < numeroInputs; j++) {
				line.push_back(tabelaVerdade[i][j]);
			}
			sop.push_back(line);
			line.clear();
		}
	}

	return sop;
}

void printSingleList(list <int> single_list) {
	for (auto const& v : single_list)
		 cout << v << "\n";
}
void printNestedList(list<list<int> > nested_list) //peguei da internet, dar uma mudada
{
	cout << "[\n";

	// nested_list`s iterator(same type as nested_list)
	// to iterate the nested_list
	list<list<int> >::iterator nested_list_itr;

	// Print the nested_list
	for (nested_list_itr = nested_list.begin(); nested_list_itr != nested_list.end(); ++nested_list_itr) {


		cout << "  [";

		// normal_list`s iterator(same type as temp_list) to iterate the normal_list
		list<int>::iterator single_list_itr;

		// pointer of each list one by one in nested list  as loop goes on
		list<int>& single_list_pointer = *nested_list_itr;

		for (single_list_itr = single_list_pointer.begin(); single_list_itr != single_list_pointer.end(); single_list_itr++) {
			cout << " " << *single_list_itr << " ";
		}
		cout << "]\n";
	}
	cout << "]";
}

list<list<int> > quineMcCluskey(list<list<int> > nested_list, list<list<int>>isop_preenchido_na_recursao) {
	list < list<int>> nested_list_aux = nested_list;
	list < list <int>> isop_preenchido_na_recursao_ne = isop_preenchido_na_recursao;
	list <list<int>> isop;
	list <list<int>> lista_recursao;
	list <list<int>> isop_aux;
	int flag_continuar_recursao_isop = 0;
	int foi_usado = 0;
	list <int> novo;
	int count;
	for (auto i = nested_list.begin(); i != nested_list.end(); ++i) {
		nested_list_aux.pop_front();
		
		for (auto j = nested_list_aux.begin(); j != nested_list_aux.end(); ++j) {
			count = 0;
			list<int>& single_list_pointer = *i;
			list<int>& single_list_pointer2 = *j;
			list<int>::iterator single_list_itr = single_list_pointer.begin();
			list<int>::iterator single_list_itr2 = single_list_pointer2.begin();
			int distanceHamming = 0;


			while (single_list_itr != single_list_pointer.end() && single_list_itr2 != single_list_pointer2.end()) {

				if (*single_list_itr != *single_list_itr2) {
					distanceHamming++;
					novo.push_back(9);  //9 = DONT_CARE

				}
				else {
					novo.push_back(*single_list_itr);
					count++;
				}
				single_list_itr++;
				single_list_itr2++;
			}

			if (distanceHamming == 1) {
				lista_recursao.push_back(novo);
				flag_continuar_recursao_isop = 1;
			}

			//problema está aqui: está quebrando o código
			if (count == 0) {//significa que o primeiro cara não se combinou com ninguém da lista, logo ele deve ser incluído na isop
				printf("\n----PASSEI POR AQUI!------\n");
				list <int> linha_isop;
				list<int>::iterator single_list_itr3 = single_list_pointer.begin();
				while (single_list_itr3 != single_list_pointer.end()) {
					linha_isop.push_back(*single_list_itr);
					single_list_itr3++;
				}
				isop_preenchido_na_recursao_ne.push_back(linha_isop);
				printf("\n----ISOP SENDO PREENCHIDO!------\n");
				printNestedList(isop_preenchido_na_recursao_ne);
			}
			
			novo.clear();
		}

		

	}


	if (flag_continuar_recursao_isop == 1) {
		lista_recursao.sort();
		lista_recursao.unique();
		isop_aux = quineMcCluskey(lista_recursao, isop_preenchido_na_recursao_ne);
		printf("\n----ISOP AUXILIAR!------\n");
		isop_aux.sort();
		isop_aux.unique();
		printNestedList(lista_recursao);
	}

	isop.sort();
	isop.unique();
	//isop.splice(isop.end(), isop_aux);
	isop = isop_preenchido_na_recursao_ne;
	return isop_aux;

}

/* PASSOS*/

bool checkUnateness() {

	return false;
}

void chowParametersComputation(int arr[7][4]) {
	int m[4]={0,0,0,0};
	int n[4]={0,0,0,0};
	int p[4]={0,0,0,0};

	for (int linha = 0; linha < 7; linha++) {
		for (int coluna = 0; coluna < 4; coluna++) {
			if (arr[linha][coluna] == 1) {
				m[coluna]++;
			}
			else if (arr[linha][coluna] == 0) {
				n[coluna]++;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		p[i] = 2 * m[i] - 2 * n[i];
	}
}


int thresholdValueComputation(int *w) {
	int high_value = 0;
	for (int m = 0; m < sizeof(w); m++)	{
		if (w[m] > high_value)
			high_value = w[m];
	}
	int threshold = high_value + 1;
	return threshold;
}

int main() {
	int numeroInputs;
	int numeroMinTermos;
	int* valoresMinTermos;
	int** tabela;

	list <list <int>> isop_preenchido_na_recursao;
	
	numeroInputs = solicitarNumeroInputs();
	numeroMinTermos = calcularNumeroMinTermos(numeroInputs);
	valoresMinTermos = (int*)malloc(numeroMinTermos * sizeof(int));
	solicitarMinTermos(numeroMinTermos, valoresMinTermos);


	tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);

	list <list <int>> sop = criarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	printNestedList(sop);
	printf("\n----------------\n");
	list <list <int>> isop = quineMcCluskey(sop, isop_preenchido_na_recursao);
	printf("\n------ISOP FINAL----\n");
	printNestedList(isop);
	

	/*
	list <list <int>> isop;
	list <int> linha1;
	list <int> linha2;
	list <int> linha3;

	linha1.push_back(0);
	linha1.push_back(0);
	linha1.push_back(9);
	linha1.push_back(9);
	isop.push_back(linha1);

	linha2.push_back(9);
	linha2.push_back(1);
	linha2.push_back(0);
	linha2.push_back(1);
	isop.push_back(linha2);

	linha3.push_back(1);
	linha3.push_back(1);
	linha3.push_back(1);
	linha3.push_back(0);
	isop.push_back(linha3);

	printNestedList(isop);
	*/

	/*TESTE CHOW'S PARAMETERS COMPUTATION!*/
	//int arr[7][4];
	//arr[0][0] = 0;
	//arr[0][1] = 0;
	//arr[0][2] = 0;
	//arr[0][3] = 0;

	//arr[1][0] = 0;
	//arr[1][1] = 0;
	//arr[1][2] = 0;
	//arr[1][3] = 1;

	//arr[2][0] = 0;
	//arr[2][1] = 0;
	//arr[2][2] = 1;
	//arr[2][3] = 0;

	//arr[3][0] = 0;
	//arr[3][1] = 0;
	//arr[3][2] = 1;
	//arr[3][3] = 1; 

	//arr[4][0] = 0;
	//arr[4][1] = 1;
	//arr[4][2] = 0;
	//arr[4][3] = 1;

	//arr[5][0] = 1;
	//arr[5][1] = 1;
	//arr[5][2] = 0;
	//arr[5][3] = 1;

	//arr[6][0] = 1;
	//arr[6][1] = 1;
	//arr[6][2] = 1;
	//arr[6][3] = 0;

	//chowParametersComputation(arr);




	/*TESTE THRESHOLD VALUE COMPUTATION!*/
	//int* weights;
	//weights = (int*)malloc(3 * sizeof(int));
	//weights[0] = 22;
	//weights[1] = 25;
	//weights[2] = 24;
	//int t = thresholdValueComputation(weights);
	//cout << t << endl;
	
	/*
	estrutura final!
	list <list <int>> isop = quineMcCluskey(sop);
	if (checkUnateness(isop) == false){
		cout << "A função não é TF" << endl;
	}
	else {
		a = chowParametersComputation(isop);
		b = generationOfSystemOfIrredudantInequalities(a);
		if (isAgainstVariableOrdering == true){
			cout << "A função não é TF" << endl;
		}
		else{
			weightAssignment(b);


		}

	}
	
	
	*/
	
	return 0;


}
