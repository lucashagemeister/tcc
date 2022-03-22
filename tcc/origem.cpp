#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include <vector>
#include <iterator>


using namespace std;

#define DONTCARE  9
#define ISNOT_TF 0
#define IS_TF 1
#define IS_UNDEFINED 2

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

void negarFuncaoBooleana(int* funcao, int *funcao_neg, int m) {
	for (int i = 0; i < m; i++) {
		if (funcao[i] == 1) {
			funcao_neg[i] = 0;
		}
		else if (funcao[i] == 0) {
			funcao_neg[i] = 1;
		}
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

list<list<int>> quineMcCluskey(list<list<int> > sop, list<list<int>>isop_incompleta) {
	list <list <int>> copiaSop = sop;
	list <list<int>> lista_recursao;
	list <list<int>> isop;
	
	list <int> novo;
	bool continuarRecursao = false;
	for (auto i = sop.begin(); i != sop.end(); ++i) {
		copiaSop.pop_front();	
		bool deuMatch = false;
		
		for (auto j = copiaSop.begin(); j != copiaSop.end(); ++j) {
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
				deuMatch = true;
				lista_recursao.push_back(novo);
				continuarRecursao = true;	
			}
			novo.clear();

		}

		if (deuMatch == false) {
			//primeiro checar se este valor já não se combinou com alguém anterior. Para isso, checar com a lista de recursão
			for (auto it_lista = lista_recursao.begin(); it_lista != lista_recursao.end(); ++it_lista) {
				list<int>& single_list_pointer3 = *it_lista;
				list<int>::iterator single_list_itr3 = single_list_pointer3.begin();
				int newDistanceHamming = 0;
				list<int>& single_list_pointer4 = *i;
				list<int>::iterator single_list_itr4 = single_list_pointer4.begin();
				while (single_list_itr3 != single_list_pointer3.end()) {
					if (*single_list_itr3 != *single_list_itr4) {
						newDistanceHamming++;
					}
					single_list_itr3++;
					single_list_itr4++;
				}
				
				if (newDistanceHamming == 1) {
					deuMatch = true;
				}
			}

		}

		if (deuMatch == false) {
			list<int>& point = *i;
			list<int>::iterator it = point.begin();
			list <int> pertenceIsop;
			while (it != point.end()) {
				pertenceIsop.push_back(*it);
				it++;
			}
			isop_incompleta.push_back(pertenceIsop);
			pertenceIsop.clear();
		}
		
	}

	if (continuarRecursao == true) {
		isop_incompleta.sort();
		isop_incompleta.unique();
		lista_recursao.sort();
		lista_recursao.unique();
		isop = quineMcCluskey(lista_recursao, isop_incompleta);
	}
	else {
		isop_incompleta.sort();
		isop_incompleta.unique();
		isop.clear();
		isop = isop_incompleta;
	}

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

/* PASSOS*/

bool isUnate(list < list <int>> funcaoASerChecada) {
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
list < list <int>> inequalitiesSystemGeneration(list <list <int>>greater, list <list <int>> lesser, bool ladoDaInequacao ) {
	list < list <int>> g;
	list < list <int>> l;

	for (auto i = greater.begin(); i != greater.end(); ++i) {
		for (auto j = lesser.begin(); j != lesser.end(); ++j) {
			list<int>& single_list_pointer = *i;
			list<int>& single_list_pointer2 = *j; 
			list<int>::iterator single_list_itr = single_list_pointer.begin();
			list<int>::iterator single_list_itr2 = single_list_pointer2.begin();
			list <int> g_line;
			list <int> l_line;
			while (single_list_itr != single_list_pointer.end()) {
				g_line.push_back(*single_list_itr);
				single_list_itr++;
					
			}

			while (single_list_itr2 != single_list_pointer2.end()) {
				l_line.push_back(*single_list_itr2);
				single_list_itr2++;
			}

			g.push_back(g_line);
			l.push_back(l_line);
			g_line.clear();
			l_line.clear();
		}
	}

	//printNestedList(g);
	//cout << endl;
	//printNestedList(l);

	if (ladoDaInequacao == 0)
		return g;
	else
		return l;
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

list <list <int>> inequalitiesSimplification(list < list <int>> greater, list < list <int>> lesser, int n) {
	
	//parte 1: Elimination of variables that appear in both sides of inequalities 
	list <list <int>>::iterator gg = greater.begin();
	list <list <int>>::iterator ll = lesser.begin();

	while (gg != greater.end() && ll != lesser.end()) {
		list<int>& pG = *gg;
		list<int>& pL = *ll;
		list<int>::iterator g = pG.begin();
		list<int>::iterator l = pL.begin();

		while (g != pG.end() && l != pL.end()) {
			if (*g != 0 && *l != 0) {
				if (*g == *l) {
					*g = 0;
					*l = 0;
				}
				else if (*g > *l) {
					*g -= *l;
					*l = 0;
				}
				else if (*g < *l) {
					*g = 0;
					*l -= *l;
				}
			}
			g++;
			l++;
		}
		gg++;
		ll++;
	}

	//parte 2: elimination of inequalities with no elements on the lesser side 
	list <list <int>>::iterator gg2 = greater.begin();
	list <list <int>>::iterator ll2 = lesser.begin();
	
	while (gg2 != greater.end() && ll2 != lesser.end()) {
		list<int>& pG2 = *gg2;
		list<int>& pL2 = *ll2;
		list<int>::iterator g2 = pG2.begin();
		list<int>::iterator l2 = pL2.begin();
		bool flag_apagarInequacao = 1;
		
		while (l2 != pL2.end()) {
			if (*l2 != 0) {
				flag_apagarInequacao = 0;
			}
			l2++;
		}

		
		if (flag_apagarInequacao == 1) {
			greater.erase(gg2++);
			lesser.erase(ll2++);
		} 
		else {
			gg2++;
			ll2++;
		}
	}

	//parte 3: Elimination of inequalities with a single element on the lesser side
	list <list <int>>::iterator gg3 = greater.begin();
	list <list <int>>::iterator ll3 = lesser.begin();
	while (gg3 != greater.end() && ll3 != lesser.end()) {
		list<int>& pG3 = *gg3;
		list<int>& pL3 = *ll3;
		list<int>::iterator g3 = pG3.begin();
		list<int>::iterator l3 = pL3.begin();
		bool flag_apagarInequacao = 1;
		bool achei_o_1 = 0;

		while (l3 != pL3.end()) {
			if (*l3 > 1) {
				flag_apagarInequacao = 0;
			}
			else if (*l3 == 1 && achei_o_1 == 0) {
				achei_o_1 = 1;
			}
			else if (*l3 == 1 && achei_o_1 == 1) {
				flag_apagarInequacao = 0;
			}
			l3++;
		}


		if (flag_apagarInequacao == 1) {
			greater.erase(gg3++);
			lesser.erase(ll3++);
		}
		else {
			gg3++;
			ll3++;
		}
	}

	if (n == 1) {
		return greater;
	}
	else if (n == 2) {
		return lesser;
	}
}


bool isAgainstVariableOrdering() {
	//não entendi o que é pra fazer
	return false;
}

void weightAssignment(list <int> weights, list <int> vwo_updatedVariables, int nUpdatesVariables ) {
	list <int>::iterator w_init = weights.begin();
	
	int count = nUpdatesVariables;
	if (*w_init == 0) { //atribuição inicial
		while (w_init != weights.end()) {
			list <int>::iterator v_init = vwo_updatedVariables.begin();
			while (v_init != vwo_updatedVariables.end()) {
				if (*v_init == count - 1) {
					*w_init = *v_init + 1;   //tá errado, sempre dá 5 4 3 2 1, arrumar isso!
				}
				v_init++;
			}
			w_init++;
			count--;
		}
	}
	printSingleList(weights);


}

bool allTheInequalitiesAreSatisfied(list <list <int>>greater, list <list <int>> lesser, list <int> weights) {
	list <list <int>>::iterator gg = greater.begin();
	list <list <int>>::iterator ll = lesser.begin();
	bool inequalitiesAreSatisfied = true;

	while (gg != greater.end() && ll != lesser.end()) {
		list<int>& pG = *gg;
		list<int>& pL = *ll;
		list<int>::iterator g = pG.begin();
		list<int>::iterator l = pL.begin();
		list<int>::iterator w = weights.begin();
		int greaterSum = 0;
		int lesserSum = 0;
		while (g != pG.end() && l != pL.end() && w != weights.end()) { //dúvida: o tamanho de weights é o mesmo de greater e lesser?
			greaterSum += (*g) * (*w);
			lesserSum += (*l) * (*w);
			w++;
			g++;
			l++;
		}
		if (lesserSum >= greaterSum) {
			inequalitiesAreSatisfied = false;
		}
		gg++;
		ll++;
	}

	return inequalitiesAreSatisfied; //ainda não testei esta função para saber se está certa ou não.
}

bool theLargestWeightIsEqualToTheTheoreticallyMaximumWeight(list <int> weights, int nVariables) {
	int largestWeight = 0;
	list<int>::iterator w = weights.begin();

	while (w != weights.end()) {
		if (*w > largestWeight) {
			largestWeight = *w;
		}
		w++;
	}
	int maximumWeight = 2 * pow(((float)nVariables + 1) / 4, ((float)nVariables + 1) / 2); //Teorema 9.3.2.1 Muroga

	if (largestWeight == maximumWeight) {
		return true;
	}
	else {
		return false; //ainda não testei esta função para saber se está certa ou não.
	}
}

bool existsCWToBeIncrased(list <list <int>> pGreaters, list <list <int>> pLessers,  list <bool> ineq_satisf, int nUpdVar) {

	list <list <int>>::iterator gg = pGreaters.begin();
	list <list <int>>::iterator ll = pLessers.begin();
	list <bool>::iterator it1 = ineq_satisf.begin();
	bool thereAreCW = false;

	list <int> g_cw;
	list <int> l_cw;
	list <int> d_cw;

	for (int i = 0; i < nUpdVar; i++) {
		g_cw.push_back(0);
		l_cw.push_back(0);
		d_cw.push_back(0);	
	}
	

	while (gg != pGreaters.end() && ll != pLessers.end()) {
		list<int>& pG = *gg;
		list<int>& pL = *ll;
		list<int>::iterator g = pG.begin();
		list<int>::iterator l = pL.begin();
		
		list <int>::iterator itG = g_cw.begin();
		list <int>::iterator itL = l_cw.begin();
		list <int>::iterator itD = d_cw.begin();

		while (g != pG.end() && l != pL.end()) {		
			
			if (*it1 ==  false) {
				*itG += *g;
				*itL += *l;
				*itD = *itG - *itL;
			}

			itG++;
			itL++;
			itD++;

			g++;
			l++;
		}
		it1++;
		gg++;
		ll++;
		

	}
	
	list <int>::iterator check_D = d_cw.begin();

	while (check_D != d_cw.end()) {
		if (*check_D > 0) {
			thereAreCW = true;
		}
		check_D++;
	}
	return thereAreCW;
}


list <int> increasingCW(list <list <int>> pGreaters, list <list <int>> pLessers, list <bool> ineq_satisf, int nUpdVar, list <int> weightsAssigned) {

	list <list <int>>::iterator gg = pGreaters.begin();
	list <list <int>>::iterator ll = pLessers.begin();
	list <bool>::iterator it1 = ineq_satisf.begin();
	list <int>::iterator it2 = weightsAssigned.begin();

	list <int> g_cw;
	list <int> l_cw;
	list <int> d_cw;

	for (int i = 0; i < nUpdVar; i++) {
		g_cw.push_back(0);
		l_cw.push_back(0);
		d_cw.push_back(0);
	}


	while (gg != pGreaters.end() && ll != pLessers.end()) {
		list<int>& pG = *gg;
		list<int>& pL = *ll;
		list<int>::iterator g = pG.begin();
		list<int>::iterator l = pL.begin();

		list <int>::iterator itG = g_cw.begin();
		list <int>::iterator itL = l_cw.begin();
		list <int>::iterator itD = d_cw.begin();

		while (g != pG.end() && l != pL.end()) {

			if (*it1 == false) {
				*itG += *g;
				*itL += *l;
				*itD = *itG - *itL;
			}

			itG++;
			itL++;
			itD++;

			g++;
			l++;
		}
		it1++;
		gg++;
		ll++;


	}

	list <int>::iterator check_D = d_cw.begin();

	while (check_D != d_cw.end() && it2 != weightsAssigned.end()) {
		if (*check_D > 0) {
			*it2+=1;
		}
		check_D++;
		it2++;
	}
	
	return weightsAssigned;
}


int thresholdValueComputation(list <int> weightedSummation) {
	int high_value = 0;
	list<int>::iterator sum = weightedSummation.begin();

	while (sum != weightedSummation.end()) {
		if (*sum < high_value) {
			high_value = *sum;
		}
		sum++;
	}

	int threshold = high_value + 1;
	return threshold; //ainda não testei esta função para saber se está certa ou não.
}

int main() {

	int numeroInputs;
	int numeroMinTermos;
	int* valoresMinTermos;
	int* valoresMinTermos_Neg;
	int** tabela;


	list <list <int>> isop;

	numeroInputs = solicitarNumeroInputs();
	numeroMinTermos = calcularNumeroMinTermos(numeroInputs);

	valoresMinTermos = (int*)malloc(numeroMinTermos * sizeof(int));
	valoresMinTermos_Neg = (int*)malloc(numeroMinTermos * sizeof(int));

	solicitarMinTermos(numeroMinTermos, valoresMinTermos);
	negarFuncaoBooleana(valoresMinTermos, valoresMinTermos_Neg, numeroMinTermos);

	tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);
	
	list <list <int>> sop = criarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	//printNestedList(sop);
	//printf("\n----------------\n");
	//list <list <int>> sop_neg = criarFormatoSOP(tabela, valoresMinTermos_Neg, numeroInputs, numeroMinTermos);
	//printNestedList(sop_neg);
	

	isop = quineMcCluskey(sop, isop);
	printf("\n----------------\n");
	printNestedList(isop);


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


	/*
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

	list <list <int>> lado_esquerdo = inequalitiesSystemGeneration(greater_side, lesser_side, 0);

	list <list <int>> lado_direito = inequalitiesSystemGeneration(greater_side, lesser_side, 1);


	
	list <list <int>> g_side;
	list <list <int>> l_side;

	g_side = inequalitiesSystemWithUpdatedVariables(lado_esquerdo, chowParameters, n);
	l_side = inequalitiesSystemWithUpdatedVariables(lado_direito, chowParameters, n);


	list <list <int>> g_simplificado;
	list <list <int>> l_simplificado;
	
	g_simplificado = inequalitiesSimplification(g_side, l_side,1);
	l_simplificado = inequalitiesSimplification(g_side, l_side,2);
	*/



	/*
	list <int> vwo_variaveisAtualizadas;
	vwo_variaveisAtualizadas.push_back(0);
	vwo_variaveisAtualizadas.push_back(4);
	vwo_variaveisAtualizadas.push_back(2);
	vwo_variaveisAtualizadas.push_back(1);
	vwo_variaveisAtualizadas.push_back(3);

	list <int> weights_variaveisAtualizadas;
	weights_variaveisAtualizadas.push_back(0);
	weights_variaveisAtualizadas.push_back(0);
	weights_variaveisAtualizadas.push_back(0);
	weights_variaveisAtualizadas.push_back(0);
	weights_variaveisAtualizadas.push_back(0);

	weightAssignment(weights_variaveisAtualizadas, vwo_variaveisAtualizadas,5);
	*/

	/*
	list <list <int>> pGreaters;
	list <list <int>> pSmallers;
	list <bool> ineq_satisf;
	int n = 5;

	ineq_satisf.push_back(false);
	ineq_satisf.push_back(false);
	ineq_satisf.push_back(false);
	ineq_satisf.push_back(false);
	ineq_satisf.push_back(false);
	ineq_satisf.push_back(false);
	ineq_satisf.push_back(true);
	ineq_satisf.push_back(true);

	list <int> g1;
	list <int> g2;
	list <int> g3;
	list <int> g4;
	list <int> g5;
	list <int> g6;
	list <int> g7;
	list <int> g8;

	list <int> l1;
	list <int> l2;
	list <int> l3;
	list <int> l4;
	list <int> l5;
	list <int> l6;
	list <int> l7;
	list <int> l8;

	g1.push_back(2);
	g1.push_back(0);
	g1.push_back(0);
	g1.push_back(0);
	g1.push_back(0);

	g2.push_back(1);
	g2.push_back(0);
	g2.push_back(0);
	g2.push_back(0);
	g2.push_back(0);

	g3.push_back(0);
	g3.push_back(1);
	g3.push_back(0);
	g3.push_back(0);
	g3.push_back(0);

	g4.push_back(0);
	g4.push_back(2);
	g4.push_back(0);
	g4.push_back(0);
	g4.push_back(0);

	g5.push_back(0);
	g5.push_back(2);
	g5.push_back(0);
	g5.push_back(0);
	g5.push_back(0);

	g6.push_back(1);
	g6.push_back(0);
	g6.push_back(0);
	g6.push_back(0);
	g6.push_back(0);

	g7.push_back(0);
	g7.push_back(1);
	g7.push_back(1);
	g7.push_back(0);
	g7.push_back(0);

	g8.push_back(0);
	g8.push_back(1);
	g8.push_back(0);
	g8.push_back(1);
	g8.push_back(0);


	l1.push_back(0);
	l1.push_back(2);
	l1.push_back(0);
	l1.push_back(1);
	l1.push_back(0);

	l2.push_back(0);
	l2.push_back(0);
	l2.push_back(1);
	l2.push_back(1);
	l2.push_back(1);

	l3.push_back(0);
	l3.push_back(0);
	l3.push_back(1);
	l3.push_back(0);
	l3.push_back(1);

	l4.push_back(1);
	l4.push_back(0);
	l4.push_back(0);
	l4.push_back(1);
	l4.push_back(1);

	l5.push_back(1);
	l5.push_back(0);
	l5.push_back(1);
	l5.push_back(0);
	l5.push_back(0);

	l6.push_back(0);
	l6.push_back(1);
	l6.push_back(0);
	l6.push_back(0);
	l6.push_back(1);

	l7.push_back(1);
	l7.push_back(0);
	l7.push_back(0);
	l7.push_back(0);
	l7.push_back(1);

	l8.push_back(1);
	l8.push_back(0);
	l8.push_back(0);
	l8.push_back(0);
	l8.push_back(0);

	pGreaters.push_back(g1);
	pGreaters.push_back(g2);
	pGreaters.push_back(g3);
	pGreaters.push_back(g4);
	pGreaters.push_back(g5);
	pGreaters.push_back(g6);
	pGreaters.push_back(g7);
	pGreaters.push_back(g8);

	pSmallers.push_back(l1);
	pSmallers.push_back(l2);
	pSmallers.push_back(l3);
	pSmallers.push_back(l4);
	pSmallers.push_back(l5);
	pSmallers.push_back(l6);
	pSmallers.push_back(l7);
	pSmallers.push_back(l8);

	list <int> w;
	w.push_back(5);
	w.push_back(4);
	w.push_back(3);
	w.push_back(2);
	w.push_back(1);
	*/

	/*
	while (existsCWToBeIncrased(pGreaters, pSmallers, ineq_satisf, n) == true) {
		w =increasingCW(pGreaters, pSmallers, ineq_satisf, n,w);
	}
	printSingleList(w);
	*/
	



	return 0;


}
