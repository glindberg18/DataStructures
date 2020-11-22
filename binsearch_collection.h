/*
Greeley Lindberg
10/10/19, hw5
Description: Implementation of Collection using a binary search vector
*/

#ifndef BINSEARCH_COLLECTION_H
#define BINSEARCH_COLLECTION_H

#include <vector>
#include "collection.h"

template <typename K, typename V>
class BinSearchCollection : public Collection <K,V> {
public:

// insert a key-value pair into the collection
void insert(const K& key, const V& val);

// remove a key-value pair from the collection
void remove(const K& key);

// find and return the value associated with the key
bool find(const K& key, V& val) const;

// find and return the list of keys >= to k1 and <= to k2
void find(const K& k1, const K& k2, std::vector <K>& keys) const;

// return all of the keys in the collection
void keys(std::vector <K>& keys) const;

// return all of the keys in ascending (sorted) order
void sort(std::vector <K>& keys) const;

// return the number of keys in collection
int size() const;

private:

// helper function for binary search
bool binsearch(const K& key, int& index) const;

// vector storage
std::vector <std::pair <K,V>> kv_list;

};


// This function returns true and sets index if key is found in
// kv_list, and returns false and sets index to where key should go in
// kv_list otherwise. If list is empty, index is unchanged.
template <typename K, typename V>
bool BinSearchCollection<K,V>::binsearch(const K& key, int& index) const {
	if (kv_list.empty())
		return false;
	int low = 0;
	int high = kv_list.size() - 1;
	while (low <= high) {
		index = (low + high) / 2;
		if (key == kv_list[index].first)
			return true;
		else if (key < kv_list[index].first)
			high = index - 1;
		else if (key > kv_list[index].first) 
			low = index + 1;
	}
	index = low;		// index = where key would go if it were in the list
	return false;
}

template <typename K, typename V>
void BinSearchCollection<K,V>::insert(const K& key, const V& val) {
	int i = 0;
	binsearch(key, i);
	std::pair<K, V> p (key, val);
	kv_list.insert(kv_list.begin() + i, p);
}

template <typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& key) {
	int i = 0;
	if (binsearch(key, i))
		kv_list.erase(kv_list.begin() + i);
}

template <typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& key, V& val) const {
	int i = 0;
	if (binsearch(key, i)) {
		val = kv_list[i].second;
		return true;
	}	
	return false;
}

template <typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, std::vector <K>& keys) const {
	keys.clear();
	int start = 0;
	int end = 0;
	binsearch(k1, start);
	binsearch(k2, end);
	if (start <= end && start < kv_list.size()) {
		if (end == size())
			end--;
		for (int i = start; i <= end; i++)
			keys.push_back(kv_list[i].first);
	}
}

template <typename K, typename V>
void BinSearchCollection<K,V>::keys(std::vector <K>& keys) const {
	keys.clear();
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		keys.push_back(kv_list[i].first);
		i++;
	}
}

template <typename K, typename V>
void BinSearchCollection<K,V>::sort(std::vector <K>& keys) const {}

template <typename K, typename V>
int BinSearchCollection<K,V>::size() const {
	return kv_list.size();
}

#endif
