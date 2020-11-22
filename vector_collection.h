/*
Greeley Lindberg
9/24/19, hw3
Description: Concrete class implementation of Collection using vectors.
Member functions are defined below.
*/

#ifndef VECTOR_COLLECTION_H
#define VECTOR_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"

template<typename K, typename V>
class VectorCollection : public Collection <K,V>
{
	public:

	// insert a key-value pair into the collection
	void insert(const K& key, const V& val);

	// remove a key-value pair from the collection
	void remove(const K& key);

	// find and return the value associated with the key
	bool find(const K& key, V& val) const;

	// find and return the list of keys >= to k1 and <= to k2
	void find(const K& k1, const K& k2, std::vector<K>& keys) const;

	// return all of the keys in the collection
	void keys(std::vector<K>& keys) const;

	// return all of the keys in ascending (sorted) order
	void sort(std::vector<K>& keys) const;

	// return the number of keys in collection
	int size() const;

	private:
	std::vector<std::pair<K,V>> kv_list;

};


template<typename K, typename V>
void VectorCollection <K,V>::insert(const K& key , const V& val)
{
	std::pair<K,V> p(key, val);
	kv_list.push_back(p);
}


template<typename K, typename V>
void VectorCollection<K,V>::remove(const K& key)
{
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		if (p.first == key) {
			kv_list.erase(kv_list.begin() + i);
			break;
		}
		i++;
	}
}

template<typename K, typename V>
bool VectorCollection<K,V>::find(const K& key, V& val) const 
{
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		if (p.first == key) {
			val = p.second;
			return true;
		}
		i++;
	}
	return false;
}

template<typename K, typename V>
void VectorCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const
{
	keys.clear();
	int begin = -1;
	int end = kv_list.size();
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		if (p.first == k1)
			begin = i;
		if (p.first == k2) {
			end = i;
			break;
		}
		i++;
	}
	if (!(end < begin || begin == -1 || end == kv_list.size()))
		for (int i = begin; i <= end; i++)
			keys.push_back(kv_list[i].first);
}

template<typename K, typename V>
void VectorCollection<K,V>::keys(std::vector<K>& keys) const
{
	keys.clear();
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		keys.push_back(kv_list[i].first);
		i++;
	}
}

template<typename K, typename V>
void VectorCollection<K,V>::sort(std::vector<K>& keys) const 
{
	keys.clear();
	unsigned int i = 0;
	for(std::pair<K,V> p : kv_list) {
		keys.push_back(kv_list[i].first);
		i++;
	}
	std::sort(keys.begin(), keys.end());
}

template<typename K, typename V>
int VectorCollection<K,V>::size() const
{
	return kv_list.size();
}

#endif
