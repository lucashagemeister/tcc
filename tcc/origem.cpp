#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
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

void solicitarMinTermos(int m, int *v) {
	cout << "Ok! Entao esta funcao booleana tem " << m << " mintermos/linhas. Informe os valores de cada mintermo" << endl;
	for (int i = 0; i < m; i++) {
		cout << "Qual o valor do mintermo m" << i << "? ";
		cin >> v[i];
	}

}

int** criarTabelaVerdade(int linhas, int colunas) {
	int **tabelaVerdade;

	tabelaVerdade = (int**) malloc(linhas * sizeof(int*));

	for (int i = 0; i < linhas; i++) {
		tabelaVerdade[i] = (int*)malloc(colunas * sizeof(int));
	}


	for (int i = 0; i < linhas; i++) {
		for (int j = 0 ; j < colunas ; j++) { 
			int x = pow(2, j);
			tabelaVerdade[i][colunas - j-1]= (i & x) ? 1 : 0;
		}
	}

	return tabelaVerdade;
}

											/* TRANSFORMAR A FUNÇÃO BOOLEANA NO FORMATO ISOP - ALGORITMO DE QUINE-MCCLUSKEY*/

list<list<int>> criarFormatoSOP(int** tabelaVerdade, int *minTermos, int numeroInputs, int numeroMinTermos) {
	
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

list<list<int> > quineMcCluskey(list<list<int> > nested_list) {
	list < list<int>> nested_list_aux = nested_list;
	list <list<int>> isop;
	list <list<int>> isop_aux;
	int flag_continuar_recursao_isop = 0;
	int foi_usado = 0;
	list <int> novo;
	list <int> velho;

	for (auto i = nested_list.begin(); i != nested_list.end(); ++i) {
		nested_list_aux.pop_front();
		for (auto j = nested_list_aux.begin(); j != nested_list_aux.end(); ++j) {
			list<int>& single_list_pointer = *i;
			list<int>& single_list_pointer2 = *j;
			list<int>::iterator single_list_itr=single_list_pointer.begin();
			list<int>::iterator single_list_itr2=single_list_pointer2.begin();
			int distanceHamming = 0;


			while (single_list_itr != single_list_pointer.end() && single_list_itr2 != single_list_pointer2.end()) {

				if (*single_list_itr != *single_list_itr2) {
					distanceHamming++;
					velho.push_back(*single_list_itr);
					novo.push_back(9); //9 = X, don't care. 
				}
				else {
					novo.push_back(*single_list_itr);
				}
				single_list_itr++;
				single_list_itr2++;
			}

			if (distanceHamming == 1) {
				isop.push_back(novo);
				flag_continuar_recursao_isop = 1;
			}

			novo.clear();
			velho.clear();

		}
		
	}


	if (flag_continuar_recursao_isop == 1) {
		isop_aux = quineMcCluskey(isop);
		printf("\n----ISOP AUXILIAR!------\n");
		printNestedList(isop_aux);
	}

	isop.splice(isop.end(), isop_aux);
	return isop;
		
}


int main() {
	int numeroInputs;
	int numeroMinTermos;
	int* valoresMinTermos;
	int** tabela;


	numeroInputs = solicitarNumeroInputs();
	numeroMinTermos = calcularNumeroMinTermos(numeroInputs);
	valoresMinTermos = (int*) malloc(numeroMinTermos * sizeof(int));
	solicitarMinTermos(numeroMinTermos, valoresMinTermos);


	tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);
	
	list <list <int>> sop = criarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	printNestedList(sop);
	printf("\n----------------\n");
	list <list <int>> isop = quineMcCluskey(sop);
	printf("\n------ISOP FINAL----\n");
	printNestedList(isop);
	return 0;


}