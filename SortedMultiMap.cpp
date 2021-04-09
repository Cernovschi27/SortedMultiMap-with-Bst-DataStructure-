#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
	for (int i = 0; i < 10000; i++)
	{
		dreapta[i] = -1;
		stanga[i] = -1;
	}
	radacina = 0;
	dim = 0;
	prim_liber = 1;
	relatie = r;
	for (int i = 0; i < 10000; i++)
	{
		elements[i].ocupat = false;
		elements[i].key = NULL_TVALUE;
	}
}
void SortedMultiMap::actualizare_prim_liber() {
	while (elements[prim_liber].ocupat)
		prim_liber++;
}

void SortedMultiMap::add(TKey c, TValue v) {
	if (dim == 0) {
		elements[radacina].key = c;
		elements[radacina].valori.push_back(v);
		dim++;
		elements[radacina].ocupat = true;
		return;
	}

	dim++;

	// avem cel putin un nod
	vector<TValue> vect = search(c);
	if (vect.size() == 0) {
		//nu exista cheia pe care dorim sa o adaugam
		//facem un nod nou si parcurgem pana la nodul care va fi parintele lui

		int nod_curent = radacina; //ne pozitionam pe radacina

		while(true) {
			if (stanga[nod_curent] != -1 && relatie(c, elements[nod_curent].key))
				nod_curent = stanga[nod_curent];
			else if (dreapta[nod_curent] != -1 && relatie(elements[nod_curent].key, c))
				nod_curent = dreapta[nod_curent];
			else {
				//am ajuns in nodul care va fi parintele celui pe care il adaugam
				if (stanga[nod_curent] == -1 && dreapta[nod_curent] == -1) {
					//verificam relatia
					//nu are copii
					if (relatie(c, elements[nod_curent].key)) {
						//adaugam in partea stanga
						stanga[nod_curent] = prim_liber;
						elements[prim_liber].key = c;
						elements[prim_liber].valori.push_back(v);
						elements[prim_liber].ocupat = true;
						break;
					}
					else {
						//adaugam in partea dreapta
						dreapta[nod_curent] = prim_liber;
						elements[prim_liber].key = c;
						elements[prim_liber].valori.push_back(v);
						elements[prim_liber].ocupat = true;
						break;
					}
				}
				else {
					//are un fiu
					//vom adauga pe fiul care este null
					if (stanga[nod_curent] == -1) {
						//adaugare fiu stang
						stanga[nod_curent] = prim_liber;
						elements[prim_liber].key = c;
						elements[prim_liber].valori.push_back(v);
						elements[prim_liber].ocupat = true;
						break;

					}
					else if (dreapta[nod_curent] == -1) {
						//adaugare fiu drept
						dreapta[nod_curent] = prim_liber;
						elements[prim_liber].key = c;
						elements[prim_liber].valori.push_back(v);
						elements[prim_liber].ocupat = true;
						break;
					}
				}
			}

		}
		actualizare_prim_liber();
	}
	else {
		//cheia exista
		int nod_curent = radacina;
		while(true) {
			if (elements[nod_curent].key == c)
				break;
			else if (stanga[nod_curent] != -1 && relatie(c, elements[nod_curent].key))
				nod_curent = stanga[nod_curent];
			else if (dreapta[nod_curent] != -1 && relatie(elements[nod_curent].key, elements[dreapta[nod_curent]].key))
				nod_curent = dreapta[nod_curent];
		}
		//ciclul s-a oprit pe nodul cu cheia c
		elements[nod_curent].valori.push_back(v);
	}
}

vector<TValue> SortedMultiMap::search(TKey c) const {
	int aux;
	int nod_curent = radacina;//radacina
	while(true) {
		aux = nod_curent;
		if (elements[nod_curent].key == c)
			return elements[nod_curent].valori;
		if (relatie(c, elements[stanga[nod_curent]].key) && stanga[nod_curent] != -1) {
			nod_curent = stanga[nod_curent];
		}
		else if (relatie(elements[nod_curent].key, c) && dreapta[nod_curent] != -1) {
			nod_curent = dreapta[nod_curent];
		}
		if (stanga[nod_curent] == -1 && dreapta[nod_curent] == -1 && elements[nod_curent].key != c)
			return vector<TValue>();
		if (aux == nod_curent)
			return vector<TValue>();
	}

}

bool SortedMultiMap::remove(TKey c, TValue v) {
	if (dim == 0)
		return false;
	vector<TValue> a = search(c);
	if(a.size()==0)
		return false;

	dim--;
	int index = get_index(c);

	if (stanga[index] == -1 && dreapta[index] == -1)
		return sterge_frunza(c, v);

	if (are_un_copil(index)) {
		return sterge_nod_cu_un_copil(c, v);
	}
	else {
		//are doi copii
		return sterge_nod_cu_doi_copii(c, v);
	}
}
bool SortedMultiMap::sterge_frunza(TKey c, TValue v)
{
	int nod_curent = radacina;
	int parinte = -1;
	while (elements[nod_curent].key != c) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			parinte = nod_curent;
			nod_curent = stanga[nod_curent];
		}
		else {
			parinte = nod_curent;
			nod_curent = dreapta[nod_curent];
		}
	}

	//nod curent = ndul de sters
	//parinte = parintele lui

	auto it = find_if(elements[nod_curent].valori.begin(), elements[nod_curent].valori.end(), [v](TValue val) {return v == val; });
	if (it == elements[nod_curent].valori.end()) {
		//nu exista valoarea
		dim++;
		return false;
	}
	else {
		if (elements[nod_curent].valori.size() == 1) {
			stanga[nod_curent] = dreapta[nod_curent] = stanga[parinte] = dreapta[parinte] = -1;
			elements[nod_curent].key = NULL_TVALUE;
			elements[nod_curent].ocupat = false;

			prim_liber = nod_curent;
			actualizare_prim_liber();
		}
		elements[nod_curent].valori.erase(it);
		return true;
	}
}
bool SortedMultiMap::sterge_nod_cu_un_copil(TKey c, TValue v)
{

	int nod_curent = radacina;
	int parinte = -1;
	while (elements[nod_curent].key != c) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			parinte = nod_curent;
			nod_curent = stanga[nod_curent];
		}
		else {
			parinte = nod_curent;
			nod_curent = dreapta[nod_curent];
		}
	}

	//nod curent = ndul de sters
	//parinte = parintele lui


	auto it = find_if(elements[nod_curent].valori.begin(), elements[nod_curent].valori.end(), [v](TValue val) {return v == val; });
	if (it == elements[nod_curent].valori.end()) {
		dim++;
		return false;
	}

	//exista valoarea
	if (elements[nod_curent].valori.size() > 1) {
		elements[nod_curent].valori.erase(it);
		return true;
	}
	else {
		//se sterge si cheia
		if (parinte == -1) {
			//se sterge radacina si are un singur fiu
			if (stanga[radacina] != -1) {
				//noua radacina e fiul stang
				prim_liber = radacina;
				//actualizare_prim_liber();

				elements[radacina].valori.clear();
				radacina = stanga[radacina];
				elements[radacina].ocupat = false;
				return true;
			}
			else {
				//noua rad e fiul drept
				prim_liber = radacina;
				//actualizare_prim_liber();

				elements[radacina].valori.clear();
				radacina = dreapta[radacina];
				elements[radacina].ocupat = false;
				return true;
			}
		}
		else {
			if ((stanga[parinte] == nod_curent)) {
				//nodul de sters se afla in sanga parintelui

				if (stanga[nod_curent] != -1) {
					//fiul nodului de sters este in stanga
					stanga[parinte] = stanga[nod_curent];
					stanga[nod_curent] = -1;
					elements[nod_curent].key = NULL_TVALUE;
					elements[nod_curent].valori.erase(it);
					elements[nod_curent].ocupat = false;

					prim_liber = nod_curent;
					//actualizare_prim_liber();

					return true;
				}
				else {
					//fiul nodului de sters este in dreapta
					stanga[parinte] = dreapta[nod_curent];
					dreapta[nod_curent] = -1;
					elements[nod_curent].key = NULL_TVALUE;
					elements[nod_curent].valori.erase(it);
					elements[nod_curent].ocupat = false;

					prim_liber = nod_curent;
					//actualizare_prim_liber();

					return true;
				}
			}
			else {
				//nodul de sters se afla in dreapta parintelui
				if (stanga[nod_curent] != -1) {
					//fiul nodului de sters se afla in stanga iar parintele este la dreapta
					dreapta[parinte] = stanga[nod_curent];
					stanga[nod_curent] = -1;
					elements[nod_curent].key = NULL_TVALUE;
					elements[nod_curent].valori.erase(it);
					elements[nod_curent].ocupat = false;

					prim_liber = nod_curent;
					//actualizare_prim_liber();

					return true;
				}
				else {
					//fiul nodului de sters este in dreapta
					dreapta[parinte] = dreapta[nod_curent];
					dreapta[nod_curent] = -1;
					elements[nod_curent].key = NULL_TVALUE;
					elements[nod_curent].valori.erase(it);
					elements[nod_curent].ocupat = false;

					prim_liber = nod_curent;
					//actualizare_prim_liber();

					return true;
				}

			}
		}
	}
}
bool SortedMultiMap::sterge_nod_cu_doi_copii(TKey c, TValue v)
{
	int nod_curent = radacina;
	int parinte = -1;
	while (elements[nod_curent].key != c) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			parinte = nod_curent;
			nod_curent = stanga[nod_curent];
		}
		else {
			parinte = nod_curent;
			nod_curent = dreapta[nod_curent];
		}
	}

	//nod curent = ndul de sters
	//parinte = parintele lui

	//nod_curent este nodul pe care se gaeste cheia

	auto it = find_if(elements[nod_curent].valori.begin(), elements[nod_curent].valori.end(), [v](TValue val) {return v == val; });
	if (it == elements[nod_curent].valori.end()) {
		//nu exista valoarea de sters
		dim++;
		return false;
	}

	if (elements[nod_curent].valori.size() == 1) {
		//se sterge cu totul cheia si nodul din arbore

		int min_din_partea_dreapta = get_inorder_succesor(nod_curent);
		//mutam informatiile utile din succesor in nodul de sters
		elements[nod_curent].key = elements[min_din_partea_dreapta].key;
		elements[nod_curent].valori = elements[min_din_partea_dreapta].valori;

		if (stanga[min_din_partea_dreapta] == -1 && dreapta[min_din_partea_dreapta] == -1) {
			stergere_totala_frunza(elements[min_din_partea_dreapta].key);
			return true;
		}
		else {
			stergere_totala_nod_cu_fiu_drept(elements[min_din_partea_dreapta].key);
			return true;
		}

	}
	else {
		// se sterge doare o valoare fara a se moifica structura arborelui
		elements[nod_curent].valori.erase(it);
		return true;
	}

}
void SortedMultiMap::stergere_totala_nod_cu_fiu_drept(TKey c) {
	int nod_curent = radacina;
	int parinte = -1;
	while (elements[nod_curent].key != c) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			parinte = nod_curent;
			nod_curent = stanga[nod_curent];
		}
		else {
			parinte = nod_curent;
			nod_curent = dreapta[nod_curent];
		}
	}

	//nod curent = ndul de sters
	//parinte = parintele lui

	if (stanga[parinte] != -1) {
		//nodul de sters e in stanga parintelui

		//fiul nodului de sters este in dreapta
		stanga[parinte] = dreapta[nod_curent];
		dreapta[nod_curent] = -1;
		elements[nod_curent].key = NULL_TVALUE;
		elements[nod_curent].valori.clear();
		elements[nod_curent].ocupat = false;
		return;
	}
	else {
		//nodul de sters e in dreapta parintelui

		dreapta[parinte] = dreapta[nod_curent];
		dreapta[nod_curent] = -1;
		elements[nod_curent].key = NULL_TVALUE;
		elements[nod_curent].valori.clear();
		elements[nod_curent].ocupat = false;
		return;
	}

}
void SortedMultiMap::stergere_totala_frunza(TKey c) {
	int nod_curent = radacina;
	int parinte = -1;
	while (elements[nod_curent].key != c) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			parinte = nod_curent;
			nod_curent = stanga[nod_curent];
		}
		else {
			parinte = nod_curent;
			nod_curent = dreapta[nod_curent];
		}
	}

	//nod curent = ndul de sters
	//parinte = parintele lui

	stanga[nod_curent] = dreapta[nod_curent] = stanga[parinte] = dreapta[parinte] = -1;
	elements[nod_curent].key =NULL_TVALUE ;
	elements[nod_curent].ocupat = false;
	elements[nod_curent].valori.clear();
}

int SortedMultiMap::get_index(TKey c)
{
	int nod_curent = radacina;
	while (elements[nod_curent].key != c && nod_curent != -1) {
		if (relatie(elements[stanga[nod_curent]].key, elements[nod_curent].key)) {
			nod_curent = stanga[nod_curent];
		}
		else {
			nod_curent = dreapta[nod_curent];
		}
	}
	return nod_curent;
}
bool SortedMultiMap::are_un_copil(int poz)
{
	if (stanga[poz] == -1 && dreapta[poz] != -1)
		return true;
	if (dreapta[poz] == -1 && stanga[poz] != -1)
		return true;
	return false;
}
int SortedMultiMap::size() const {
	return dim;
}

bool SortedMultiMap::isEmpty() const {
	return dim == 0;
}
int SortedMultiMap::get_inorder_succesor(int nod)
{
	if (dreapta[nod] != -1)
		return minValue(dreapta[nod]);

	return maxValue(stanga[nod]);
}
int SortedMultiMap::minValue(int nod)
{
	int curent = nod;
	while (stanga[curent] != -1) {
		curent = stanga[curent];
	}
	return curent;
}

int SortedMultiMap::maxValue(int nod) {
	int curent = nod;
	while (dreapta[curent] != -1)
		curent = dreapta[curent];
	return curent;
}

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {

}
