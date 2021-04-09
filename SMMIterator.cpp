#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <algorithm>

SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	curent = map.radacina;
	for (auto nod : map.elements)
		if (nod.valori.size())
			bst.push_back(nod);
	//auto min = map.get_min();
	for (int i = 0; i < bst.size();i++) {
		for (int j =0;j<bst.size();j++)
			if (map.relatie(bst[i].key, bst[j].key))
				std::swap(bst[i], bst[j]);
	}
}

void SMMIterator::first(){
	curent = map.radacina;
}

void SMMIterator::next(){
	curent++;
}

bool SMMIterator::valid() const{
	if (map.dim == 0)
		return false;
	if (curent == bst.size())
		return false;
	if (curent == -1)
		return false;
	return true;
}

TElem SMMIterator::getCurrent() const{
	return std::make_pair(bst[curent].key, bst[curent].valori[0]);
	
}


