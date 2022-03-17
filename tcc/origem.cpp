#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include <vector>
#include <iterator>


using namespace std;

#define DONTCARE  9

list <list <int>> isop;

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

list<list<int>> negarECriarFormatoSOP(int** tabelaVerdade, int* minTermos, int numeroInputs, int numeroMinTermos) {

	list<list<int>> sop_neg;
	list <int> line;
	for (int i = 0; i < numeroMinTermos; i++) {
		if (minTermos[i] == 0) {
			for (int j = 0; j < numeroInputs; j++) {
				line.push_back(tabelaVerdade[i][j]);
			}
			sop_neg.push_back(line);
			line.clear();
		}
	}

	return sop_neg;
}

void printSingleList(list <int> single_list) {
	for (auto const& v : single_list)
		 cout << v << "\n";
}

void printNestedList(list<list<int> > nested_list) {
	cout << "[\n";
	list<list<int> >::iterator nested_list_itr;
	for (nested_list_itr = nested_list.begin(); nested_list_itr != nested_list.end(); ++nested_list_itr) {
		cout << "  [";
		list<int>::iterator single_list_itr;
		list<int>& single_list_pointer = *nested_list_itr;
		for (single_list_itr = single_list_pointer.begin(); single_list_itr != single_list_pointer.end(); single_list_itr++) {
			cout << " " << *single_list_itr << " ";
		}
		cout << "]\n";
	}
	cout << "]";
}

list<list<int>> quineMcCluskey(list<list<int> > nested_list, list<list<int>>isop_preenchido_na_recursao) {
	list < list<int>> nested_list_aux = nested_list;
	list < list <int>> isop_preenchido_na_recursao_ne = isop_preenchido_na_recursao;
	list <list<int>> isop;
	list <list<int>> lista_recursao;
	list <list<int>> isop_aux;
	int flag_continuar_recursao_isop = 0;
	int foi_usado = 0;
	list <int> novo;
	list <int> linha_isop;
	int count;
	int flag_se_combinou_com_alguem = 0;
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
					novo.push_back(DONTCARE);  

				}
				else {
					novo.push_back(*single_list_itr);
				}
				single_list_itr++;
				single_list_itr2++;
			}

			if (distanceHamming == 1) {
				lista_recursao.push_back(novo);
				flag_continuar_recursao_isop = 1;
				flag_se_combinou_com_alguem = 1;
			}
			
			novo.clear();
			
		}
		/*
		if (flag_se_combinou_com_alguem == 0) {
			list<int>::iterator single_list_itr3 = single_list_pointer.begin();
			while (single_list_itr3 != single_list_pointer.end()) {
				linha_isop.push_back(*single_list_itr3);
				single_list_itr3++;
			}
			isop_preenchido_na_recursao_ne.push_back(linha_isop);
			isop.push_back(linha_isop);
			linha_isop.clear();

		}
		else {
			flag_se_combinou_com_alguem = 0;
		}*/
		
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
	return isop;
}

int numberOfUpdatedVariables(list <int> chowParameters) {
	list <int> c = chowParameters;
	c.unique();
	int n=0;
	for (auto i = c.begin(); i != c.end(); ++i) {
		n++;
	}
	return n;
}

//vai ser preciso criar a isop da função negara, por isso é preciso criar uma função que negue

/* PASSOS*/

bool checkUnateness(list < list <int>> funcaoASerChecada) {
	bool isUnate = true;
	list<int> countZeros;
	list<int> countOnes;

	//for para inicializar as listas dos contadores
	
	for (auto i = funcaoASerChecada.begin(); i != funcaoASerChecada.end(); ++i) {
		list<int>& single_list_pointerX = *i;
		list<int>::iterator single_list_itrX = single_list_pointerX.begin();
		while (single_list_itrX != single_list_pointerX.end()) {
			countZeros.push_back(0);
			countOnes.push_back(0);
			single_list_itrX++;
		}
		break; //que bagaceiro!
	}


	for (auto i = funcaoASerChecada.begin(); i != funcaoASerChecada.end(); ++i) {
		list<int>& single_list_pointer = *i;
		list<int>::iterator single_list_itr = single_list_pointer.begin();

		list<int>::iterator zeros_itr = countZeros.begin();
		list<int>::iterator ones_itr = countOnes.begin();

		while (single_list_itr != single_list_pointer.end()) {
			if (*single_list_itr == 0) {
				*zeros_itr+=1;
			}
			else if (*single_list_itr == 1) {
				*ones_itr+=1;
			}
			single_list_itr++;
			zeros_itr++;
			ones_itr++;
		}
	}


	return isUnate;
}

list<int> chowParametersComputation(list < list <int>> funcaoASerChecada) {
	list<int> countZeros;
	list<int> countOnes;
	list<int> chowParameters;
	//for para inicializar as listas dos contadores

	for (auto i = funcaoASerChecada.begin(); i != funcaoASerChecada.end(); ++i) {
		list<int>& single_list_pointerX = *i;
		list<int>::iterator single_list_itrX = single_list_pointerX.begin();
		while (single_list_itrX != single_list_pointerX.end()) {
			countZeros.push_back(0);
			countOnes.push_back(0);
			chowParameters.push_back(0);
			single_list_itrX++;
		}
		break; //que bagaceiro!
	}


	for (auto i = funcaoASerChecada.begin(); i != funcaoASerChecada.end(); ++i) {
		list<int>& single_list_pointer = *i;
		list<int>::iterator single_list_itr = single_list_pointer.begin();

		list<int>::iterator zeros_itr = countZeros.begin();
		list<int>::iterator ones_itr = countOnes.begin();

		while (single_list_itr != single_list_pointer.end()) {
			if (*single_list_itr == 0) {
				*zeros_itr += 1;
			}
			else if (*single_list_itr == 1) {
				*ones_itr += 1;
			}
			single_list_itr++;
			zeros_itr++;
			ones_itr++;
		}

		list<int>::iterator n = countZeros.begin();
		list<int>::iterator m = countOnes.begin();
		list<int>::iterator p = chowParameters.begin();

		while (p != chowParameters.end()) {
			*p = 2*(* m) - 2*(* n);
			p++;
			m++;
			n++;
		}
	}

	return chowParameters;
}

//Inequações
void inequalitiesSystemGeneration(list <list <int>>greater, list <list <int>> lesser) {
	for (auto i = greater.begin(); i != greater.end(); ++i) {
		for (auto j = lesser.begin(); j != lesser.end(); ++j) {
			list<int>& single_list_pointer = *i;
			list<int>& single_list_pointer2 = *j; 
			list<int>::iterator single_list_itr = single_list_pointer.begin();
			list<int>::iterator single_list_itr2 = single_list_pointer2.begin();

			while (single_list_itr != single_list_pointer.end()) {
				cout << *single_list_itr;
				single_list_itr++;
			}

			cout << " > ";
			while (single_list_itr2 != single_list_pointer2.end()) {
				cout << *single_list_itr2;
				single_list_itr2++;
			}

			cout << endl;

		}
	}

}

list <list <int>> inequalitiesSystemWithUpdatedVariables(list <list <int>>greater, list <int> chowParameters, int numberOfChows) {
	list <list <int>> pGreaters;
	//list <list <int>> pSmallers;
	std::list<int>::iterator it;
	list <int> linha_pGreaters;
	list <int> vwo;
	vwo.push_back(0);
	vwo.push_back(1);
	vwo.push_back(2);
	vwo.push_back(2);


	for (auto i = greater.begin(); i != greater.end(); ++i) {
		
		int l = 0;
		
		while (l != numberOfChows) {
			linha_pGreaters.push_back(0);
			l++;
		}

		list<int>& single_list_pointer = *i;

		list<int>::iterator it1 = single_list_pointer.begin();
		list<int>::iterator it2 = vwo.begin();
		
		
		while (it1 != single_list_pointer.end() && it2 != vwo.end()) {
			list<int>::iterator it3 = linha_pGreaters.begin();

			for (int count = 0; count < *it2; count++) {
				it3++;
			}
			
			*it3 += *it1;

			it1++;
			it2++;
		}

		pGreaters.push_back(linha_pGreaters);
		linha_pGreaters.clear();
	}
	return pGreaters;
	
}


void inequalitiesSimplification(list < list <int>> greater, list < list <int>> lesser) {
	//parte 1


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

	//int numeroInputs;
	//int numeroMinTermos;
	//int* valoresMinTermos;
	//int** tabela;
	//bool isTF;

	//list <list <int>> isop_preenchido_na_recursao;

	//numeroInputs = solicitarNumeroInputs();
	//numeroMinTermos = calcularNumeroMinTermos(numeroInputs);
	//valoresMinTermos = (int*)malloc(numeroMinTermos * sizeof(int));
	//solicitarMinTermos(numeroMinTermos, valoresMinTermos);


	//tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);

	//list <list <int>> sop = criarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	//printNestedList(sop);
	//printf("\n----------------\n");
	//list <list <int>> sop_neg = negarECriarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	//printNestedList(sop_neg);
	//quineMcCluskey(sop, isop_preenchido_na_recursao);
	//printNestedList(isop);


	/* ESTE VAI SER O CÓDIGO FINAL!
	int numeroInputs;
	int numeroMinTermos;
	int* valoresMinTermos;
	int** tabela;
	bool isTF;

	list <list <int>> isop_preenchido_na_recursao;

	numeroInputs = solicitarNumeroInputs();
	numeroMinTermos = calcularNumeroMinTermos(numeroInputs);
	valoresMinTermos = (int*)malloc(numeroMinTermos * sizeof(int));
	solicitarMinTermos(numeroMinTermos, valoresMinTermos);


	tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);

	list <list <int>> isop = quineMcCluskey(sop, isop_preenchido_na_recursao);
	
	if (checkUnateness(isop) == false){
		isTF = false;
	}
	else{
		chowParametersComputation(isop);
		(...)
	}
	
	*/


	
	list <list <int>> greater_side;
	list <list <int>> lesser_side;

	list <int> g1;
	list <int> g2;
	list <int> l1;
	list <int> l2;
	list <int> l3;

	g1.push_back(1);
	g1.push_back(0);
	g1.push_back(1);
	g1.push_back(1);

	g2.push_back(1);
	g2.push_back(1);
	g2.push_back(0);
	g2.push_back(0);

	l1.push_back(0);
	l1.push_back(1);
	l1.push_back(1);
	l1.push_back(1);

	l2.push_back(1);
	l2.push_back(0);
	l2.push_back(0);
	l2.push_back(1);

	l3.push_back(1);
	l3.push_back(0);
	l3.push_back(1);
	l3.push_back(0);

	greater_side.push_back(g1);
	greater_side.push_back(g2);
	
	lesser_side.push_back(l1);
	lesser_side.push_back(l2);
	lesser_side.push_back(l3);

	list <int> chowParameters;
	chowParameters.push_back(10);
	chowParameters.push_back(6);
	chowParameters.push_back(2);
	chowParameters.push_back(2);
	
	list <int> vwo;
	vwo.push_back(0);
	vwo.push_back(1);
	vwo.push_back(2);
	vwo.push_back(2);

	int n = numberOfUpdatedVariables(chowParameters);
	//cout << n << endl;

	//inequalitiesSystemGeneration(greater_side, lesser_side);
	
	list <list <int>> g_side;
	list <list <int>> l_side;

	g_side = inequalitiesSystemWithUpdatedVariables(greater_side, chowParameters, n);
	l_side = inequalitiesSystemWithUpdatedVariables(lesser_side, chowParameters, n);

	cout << "***************** Greater Side ******************" << endl;
	printNestedList(g_side);
	cout << "***************** Lesser Side ******************" << endl;
	printNestedList(l_side);
	inequalitiesSimplification(g_side, l_side);

	return 0;


}
