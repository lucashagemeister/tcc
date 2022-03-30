#include <stdio.h>
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include <vector>
#include <iterator>


using namespace std;

#define DONTCARE  9
//#define ISNOT_TF 0
//#define IS_TF 1
//#define IS_UNDEFINED 2

list <list <int>> isop;

								/*FUNÇÕES AUXILIARES DE IMPRESSÃO DE LISTAS*/
void printSingleList(list <int> single_list) {
	cout << "[" << " ";
	for (auto const& v : single_list)
		cout << v << " ";
	cout << "]\n";
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
	cout << "]\n";
}

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

list<int> minTermoCorrespondente(list <int> linha) {
	
	int numeroDontCares = 0;
	for (auto i = linha.begin(); i != linha.end(); ++i) {
		if (*i == 9) {
			numeroDontCares++;
		}
	}

	int qte = 0;
	for (auto i = linha.begin(); i != linha.end(); ++i) {
		qte++;
	}

	int j = qte - 1;

	list <int> listaMinTermo;
	int nMinTermos = pow(2, numeroDontCares);
	for (int i = 0; i < nMinTermos; i++) {
		listaMinTermo.push_back(0);
	}
	int nDontCares = numeroDontCares;

	for (auto i = linha.begin(); i != linha.end(); ++i) {
		list <int>::iterator it = listaMinTermo.begin();
		int flag = 0;
		int v = 0;
		if (*i == 9) {
			nDontCares--;
		}
		
		while (it != listaMinTermo.end()) {
			int k = pow(2, j);
			int l = pow(2, nDontCares);
			if (*i != 9) {
				*it += (*i) * k;
			}
			else {
				*it += flag * k;
				
				v++;
				if (v == l) {
					v = 0;
					switch (flag) {
					case 0: flag = 1; break;
					case 1: flag = 0; break;
					}

				}
				
			}
			it++;
		}
		j--;
	}

	return listaMinTermo;
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

void negarFuncaoBooleana(int* funcao, int* funcao_neg, int m) {
	for (int i = 0; i < m; i++) {
		if (funcao[i] == 1) {
			funcao_neg[i] = 0;
		}
		else if (funcao[i] == 0) {
			funcao_neg[i] = 1;
		}
	}

}

list <list <int>> converterFuncaoNegadasNosPesosCorrespondentes(list <list <int>> funcaoNegada) {
	for (auto i = funcaoNegada.begin(); i != funcaoNegada.end(); ++i) {
		list<int>& point = *i;
		list<int>::iterator j = point.begin();

		while (j != point.end()) {
			if (*j == 9) {
				*j = 1;
			}
			else if (*j == 0) {
				*j = 9;
			}
			j++;
		}
	}
	return funcaoNegada;
}

list<list<int>> quineMcCluskey(list<list<int> > sop, list<list<int>>isop_incompleta) {

	list <list <int>> copiaSop = sop;
	list <list<int>> lista_recursao;
	list < list <int>> primes;
	list <list<int>> isop;
	list <list <int>> minTermsCovered;

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
		primes = quineMcCluskey(lista_recursao, isop_incompleta);
	}
	else {
		isop_incompleta.sort();
		isop_incompleta.unique();
		primes.clear();
		primes = isop_incompleta;
	}

	

	//descobrir mintermos associados a cada primo
	for (auto i = primes.begin(); i != primes.end(); ++i) {
		list <int> minTermos = minTermoCorrespondente(*i);
		minTermsCovered.push_back(minTermos);
	}


	//tabela de cobertura para descobrir quem são os primos essenciais
	
	list <list <int>>::iterator it_primes = primes.begin();
	list <list <int>>::iterator it_mTC = minTermsCovered.begin();
	list < list <int>> mTC_copy = minTermsCovered;

	list <int> allMinTermsCovered;
	list <list <int>> essentialPrimes;
	list <int> mTCinOneLine;

	while (it_mTC != minTermsCovered.end()) {
		list<int>& point = *it_mTC;
		list<int>::iterator it1 = point.begin();
		while (it1 != point.end()) {
			mTCinOneLine.push_back(*it1);
			it1++;
		}
		it_mTC++;
	}

	list <list <int>>::iterator it_mT = minTermsCovered.begin();
	

	while (it_primes != primes.end()) {
		list<int>& pointer = *it_mT;
		list<int>::iterator it1 = pointer.begin();

		
		while (it1 != pointer.end()) {
			int count = 0;
			list<int>::iterator single_line = mTCinOneLine.begin();
			while (single_line != mTCinOneLine.end()) {
				if (*it1 == *single_line) {
					count++;
				}
				single_line++;
			}
			bool jaEssencial = (std::find(essentialPrimes.begin(), essentialPrimes.end(), *it_primes) != essentialPrimes.end());
			if (count == 1 && jaEssencial == false) {
				essentialPrimes.push_back(*it_primes);
				list<int>& pointer2 = *it_mT;
				list<int>::iterator it2 = pointer2.begin();
				while (it2 != pointer2.end()) {
					allMinTermsCovered.push_back(*it2);
					it2++;
				}
				
			}
			it1++;
		}

		

		it_mT++;
		it_primes++;
	}

	//se houver algum bug, é nesta parte final (abaixo) onde eu criei um algoritmo para escolher os implicantes não essenciais que entram na ISOP
	list <list <int>> notSelected;
	
	list <list <int>>::iterator x = minTermsCovered.begin();

	for (auto i = primes.begin(); i != primes.end(); ++i) {
		list<int>& pointer = *x;
		list<int>::iterator it1 = pointer.begin();

		while (it1 != pointer.end()) {
			for (auto j = allMinTermsCovered.begin(); j != allMinTermsCovered.end(); ++j) {
				bool isEssential = (std::find(essentialPrimes.begin(), essentialPrimes.end(), *i) != essentialPrimes.end());
				bool isNotSelected = (std::find(notSelected.begin(), notSelected.end(), *i) != notSelected.end());
				if (*it1 == *j && isEssential == false && isNotSelected == false) {
					notSelected.push_back(*i);
				}
			}
			it1++;
		}
		x++;
	}

	list <list <int>> primeImplicants;

	list <list <int>>::iterator y = minTermsCovered.begin();
	for (auto i = primes.begin(); i != primes.end(); ++i) {
		list<int>& pointer = *y;
		list<int>::iterator it1 = pointer.begin();

		
		bool isEssential = (std::find(essentialPrimes.begin(), essentialPrimes.end(), *i) != essentialPrimes.end());
		bool isNotSelected = (std::find(notSelected.begin(), notSelected.end(), *i) != notSelected.end());
		if (isEssential == false && isNotSelected == false) {
			bool isImplicant = true;
			while (it1 != pointer.end()) {
				bool mTCovered = (std::find(allMinTermsCovered.begin(), allMinTermsCovered.end(), *it1) != allMinTermsCovered.end());
				if (mTCovered == true) {
					isImplicant = false;
				}
				it1++;
			}
			if (isImplicant == true) {
				primeImplicants.push_back(*i);
				list<int>& pointer2 = *y;
				list<int>::iterator it2 = pointer2.begin();
				while (it2 != pointer2.end()) {
					allMinTermsCovered.push_back(*it2);
					it2++;
				}
			}
		}
		y++;
	}

	list <list <int>>::iterator y2 = minTermsCovered.begin();
	for (auto i = primes.begin(); i != primes.end(); ++i) {
		bool isEssential = (std::find(essentialPrimes.begin(), essentialPrimes.end(), *i) != essentialPrimes.end());
		bool isNotSelected = (std::find(notSelected.begin(), notSelected.end(), *i) != notSelected.end());
		if (isEssential == false && isNotSelected == false) {
			list<int>& pointer = *y2;
			list<int>::iterator it = pointer.begin();
			while (it != pointer.end()) {
				bool isCovered = (std::find(allMinTermsCovered.begin(), allMinTermsCovered.end(), *it) != allMinTermsCovered.end());
				if (isCovered == false) {
					primeImplicants.push_back(*i);
					list<int>& pointer2 = *y2;
					list<int>::iterator it2 = pointer2.begin();
					while (it2 != pointer2.end()) {
						allMinTermsCovered.push_back(*it2);
						it2++;
					}
				}
				it++;
			}
		}
		y2++;
	}

	for (auto i = essentialPrimes.begin(); i != essentialPrimes.end(); ++i) {
		primeImplicants.push_back(*i);
	}

	return primeImplicants;
	
}

list <list <int>> converterVariaveisEmPesos(list <list <int>> funcao) {
	for (auto i = funcao.begin(); i != funcao.end(); ++i) {
		list<int>& point = *i;
		list<int>::iterator j = point.begin();

		while (j != point.end()) {
			if (*j == 9) {
				*j = 0;
			}
			j++;
		}
	}
	return funcao;
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

	list<int>::iterator zeros_itr = countZeros.begin();
	list<int>::iterator ones_itr = countOnes.begin();

	while (zeros_itr != countZeros.end() && ones_itr != countOnes.end()) {
		if (*zeros_itr != 0 && *ones_itr != 0) {
			isUnate = false;
		}
		zeros_itr++;
		ones_itr++;
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

list <int> vwoGeneration(list <int> chowParameters) {
	list <int> vwo;
	list <int>::iterator it1 = chowParameters.begin();
	list <int>::iterator it2 = chowParameters.begin();
	
	while (it1 != chowParameters.end()) {
		int nMaiores = 0;
		list <int> jaUsados;
		list <int>::iterator it2 = chowParameters.begin();

		while (it2 != chowParameters.end()) {
			bool jaUsado = (std::find(jaUsados.begin(), jaUsados.end(), *it2) != jaUsados.end());
			if (*it1 < *it2 && jaUsado ==false) {
				nMaiores += 1;
				jaUsados.push_back(*it2);
			}
			it2++;
		}
		vwo.push_back(nMaiores);
		it1++;
	}

	return vwo;
}

list <int> vwo_UpdatedVariables(list <int> vwo) { //ver direitinho se é só isso mesmo, se não vai dar ruim lá na frente.
	list <int> x = vwo;
	x.unique();
	return x;
}
	//Inequações

//list <list <int>> redundantWeightedSummationRemoval(list <int> sideOfInequality, list <int> vwo) {

//}
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



	if (ladoDaInequacao == 0)
		return g;
	else
		return l;
}

//list <list <int>> redundantInequalityRemoval() {
	
	
//}

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

bool isAgainstVariableOrdering(list <int> vwo, list <list <int>> greater, list <list <int>>lesser) {
	
	//não entendi o que é pra fazer
	return false;
}

list <int> initialWeightAssignment(list <int> vwoUpdatedVariables) {
	list <int> weights;
	int sizeOfVwo = 0;
	list <int>::iterator it = vwoUpdatedVariables.begin();
	
	while (it != vwoUpdatedVariables.end()) {
		sizeOfVwo++;
		it++;
		weights.push_back(0);
	}

	list <int>::iterator it1 = vwoUpdatedVariables.begin();
	list <int>::iterator it2 = weights.begin();

	while (it1 != vwoUpdatedVariables.end() && it2 != weights.end()) {
		*it2 = sizeOfVwo - *it1;
		it1++;
		it2++;
	}

	return weights;
}

int calculateTheoreticallyMaximumWeight(int nVariables) {
	int maximumWeight = 2 * pow(((float)nVariables + 1) / 4, ((float)nVariables + 1) / 2); //Teorema 9.3.2.1 Muroga
	return maximumWeight;
}

list <int> allTheInequalitiesAreSatisfied(list <list <int>>greater, list <list <int>> lesser, list <int> weights) {
	list <list <int>>::iterator gg = greater.begin();
	list <list <int>>::iterator ll = lesser.begin();

	list <int> falseInequalities;
	while (gg != greater.end() && ll != lesser.end()) {

		list<int>& pG = *gg;
		list<int>& pL = *ll;
		list<int>::iterator g = pG.begin();
		list<int>::iterator l = pL.begin();
		list<int>::iterator w = weights.begin();
		int greaterSum = 0;
		int lesserSum = 0;
		while (g != pG.end() && l != pL.end() && w != weights.end()) {
			greaterSum += (*g) * (*w);
			lesserSum += (*l) * (*w);
			w++;
			g++;
			l++;
		}
		if (lesserSum >= greaterSum) {
			falseInequalities.push_back(1);
		}
		else {
			falseInequalities.push_back(0);
		}
		gg++;
		ll++;
	}
	return falseInequalities;
}

bool theLargestWeightIsEqualToTheTheoreticallyMaximumWeight(list <int> weights, int maximumWeight) {
	int largestWeight = 0;
	list<int>::iterator w = weights.begin();

	while (w != weights.end()) {
		if (*w > largestWeight) {
			largestWeight = *w;
		}
		w++;
	}

	if (largestWeight == maximumWeight) {
		return true;
	}
	else {
		return false;
	}
}

bool existsCWToBeIncrased(list <list <int>> pGreaters, list <list <int>> pLessers, list <int> ineq_satisf, int nUpdVar) {

	list <list <int>>::iterator gg = pGreaters.begin();
	list <list <int>>::iterator ll = pLessers.begin();
	list <int>::iterator it1 = ineq_satisf.begin();
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

	while (check_D != d_cw.end()) {
		if (*check_D > 0) {
			thereAreCW = true;
		}
		check_D++;
	}
	return thereAreCW;
}

list <int> increasingCW(list <list <int>> pGreaters, list <list <int>> pLessers, list <int> ineq_satisf, int nUpdVar, list <int> weights) {

	list<int> weightsAssigned = weights;
	list <list <int>>::iterator gg = pGreaters.begin();
	list <list <int>>::iterator ll = pLessers.begin();
	list <int>::iterator it1 = ineq_satisf.begin();
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

			if (*it1 == 0) {
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
			*it2 += 1;
		}
		check_D++;
		it2++;
	}

	return weightsAssigned;
}

list <int> weightAssignment(list <int> weights, list <list<int>> irredundantGreater, list <list<int>> irredundantLesser, int maximumWeight, int nUpdatedVariables ) {
	
	list <int> falseInequalities = 	allTheInequalitiesAreSatisfied(irredundantGreater, irredundantLesser, weights);

	bool existsFalseInequalities = (std::find(falseInequalities.begin(), falseInequalities.end(), 1) != falseInequalities.end());
	if (existsFalseInequalities == true) {
		bool largestIsMax = theLargestWeightIsEqualToTheTheoreticallyMaximumWeight(weights, maximumWeight);
		if (largestIsMax == true) {
			//f is an underterminated function (ao menos nesta folha)
		}
		else {
			bool thereAreCw = existsCWToBeIncrased(irredundantGreater, irredundantLesser, falseInequalities, nUpdatedVariables);
			if (thereAreCw == false) {
				//f is a non-TF (ao menos nesta folha)
			}
			else {
				list <int>::iterator it_Weights;
				list <int> possiblesIncrementedWeights = increasingCW(irredundantGreater, irredundantLesser, falseInequalities, nUpdatedVariables, weights);
				//a linha de cima está certa?
			}
		}
	}
	else {
		return weights;
	}
	

}

int thresholdValueComputation(list <int> weights, list <list<int>> irredundantLesser) { //irredundantLesser = pesos originais (w1,w2,...)
	
	list <int> weightSummation;
	int threshold;
	
	list <list<int>>::iterator g= irredundantLesser.begin();

	for (auto i = irredundantLesser.begin(); i != irredundantLesser.end(); ++i) {
		int sum = 0;
		list<int>& itr = *i;
		list<int>::iterator g = itr.begin();
		list <int>::iterator w = weights.begin();
		while (g != itr.end() && w != weights.end()) {
			sum += (*g) * (*w);
			g++;
			w++;
		}
		weightSummation.push_back(sum);
	}

	int high_value = 0;
	list <int>::iterator wS = weightSummation.begin();

	while (wS != weightSummation.end()) {
		if (*wS > high_value) {
			high_value = *wS;
		}
		wS++;
	}

	threshold = high_value + 1;

	return threshold; 
}

int main() {

	int numeroInputs;
	int numeroMinTermos;
	int* valoresMinTermos;
	int* valoresMinTermos_Neg;
	int** tabela;

	list <list <int>> isop;
	list <list <int>> isop_neg;

											/*SETUP INICIAL: INPUT E GERAÇÃO DA ISOP*/
	numeroInputs = solicitarNumeroInputs();
	numeroMinTermos = calcularNumeroMinTermos(numeroInputs);

	valoresMinTermos = (int*)malloc(numeroMinTermos * sizeof(int));
	valoresMinTermos_Neg = (int*)malloc(numeroMinTermos * sizeof(int));

	solicitarMinTermos(numeroMinTermos, valoresMinTermos);
	negarFuncaoBooleana(valoresMinTermos, valoresMinTermos_Neg, numeroMinTermos);

	tabela = criarTabelaVerdade(numeroMinTermos, numeroInputs);
	
	list <list <int>> sop = criarFormatoSOP(tabela, valoresMinTermos, numeroInputs, numeroMinTermos);
	list <list <int>> sop_neg = criarFormatoSOP(tabela, valoresMinTermos_Neg, numeroInputs, numeroMinTermos);


	isop = quineMcCluskey(sop, isop);
	isop_neg = quineMcCluskey(sop_neg, isop_neg); 
	isop_neg = converterFuncaoNegadasNosPesosCorrespondentes(isop_neg);

													/*FLUXOGRAMA DO ALGORITMO*/	
	
	if (isUnate(isop) == false) {
		cout << "Não eh Threshold" << endl;
	}
	else {
		list <int> chowParameters = chowParametersComputation(sop); 
		int nVariables = numberOfUpdatedVariables(chowParameters);
		list <int> vwo = vwoGeneration(chowParameters); 

		list < list <int>> greaterThanThreshold = converterVariaveisEmPesos(isop);
		list < list <int>> lesserThanThreshold = converterVariaveisEmPesos(isop_neg);


		/*
		aqui, antes da geração do sistema de inequações, vai o redundantWeightedSummationRemoval dos chineses
		*/

		list <list <int>> greater_side = inequalitiesSystemGeneration(greaterThanThreshold, lesserThanThreshold, 0);
		list <list <int>> lesser_side = inequalitiesSystemGeneration(greaterThanThreshold, lesserThanThreshold, 1);
		


		/*
		aqui, depois  da geração do sistema de inequações, vai o redundantInequalityRemoval dos chineses
		*/

		//list <int> vwo;
		//próximas listas são resultados do passo acima (redundantInequalityRemoval)
		list <list <int>> greaterAfterRendudantInequalityRemoval;
		list <list <int>> lesserAfterRedundantInequalityRemoval;

		/*
		e aqui vai o Against Variable Ordering, a partir do resultado do redundantInequalityRemoval
		*/


		list <list <int>> greater_side_updatedVariables = inequalitiesSystemWithUpdatedVariables(greater_side, chowParameters, nVariables);
		list <list <int>> lesser_side_updatedVariables = inequalitiesSystemWithUpdatedVariables(lesser_side, chowParameters, nVariables);

		list <list <int>> greater_simplificado = inequalitiesSimplification(greater_side_updatedVariables, lesser_side_updatedVariables, 1);
		list <list <int>> lesser_simplificado = inequalitiesSimplification(greater_side_updatedVariables, lesser_side_updatedVariables, 2);

		printNestedList(greater_simplificado);
		printNestedList(lesser_simplificado);

		list <int> vwoUpdatedVariables; //como preenchê-la?
		list <int> wa = initialWeightAssignment(vwoUpdatedVariables);
		
		//weightAssignment(list <int> weights, list <list<int>> irredundantGreater, list <list<int>> irredundantLesser, int maximumWeight, int nUpdatedVariables )9
	}
	
	return 0;

}
