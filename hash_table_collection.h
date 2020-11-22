/*
Greeley Lindberg
11/5/19, hw7
Description: Implementation of Collect using a hash table
*/

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include <vector>
#include <algorithm>
#include <functional>
#include "collection.h"
template <typename K, typename V>
class HashTableCollection: public Collection<K,V> {
	public:
		// create an empty linked list
		HashTableCollection();

		// copy a linked list
		HashTableCollection(const HashTableCollection<K,V>& rhs);

		// assign a linked list
		 HashTableCollection<K,V>& operator =(const HashTableCollection<K,V>& rhs);

		// delete a linked list
		~HashTableCollection();

		// insert a key -value pair into the collection
		void insert(const K& key , const V& val);

		// remove a key -value pair from the collection
		void remove(const K& key);

		// find the value associated with the key
		bool find(const K& key , V& val) const;

		// find the keys associated with the range
		void find(const K& k1, const K& k2, std::vector<K>& keys) const;

		// return all keys in the collection
		void keys(std::vector<K>& keys) const;

		// return collection keys in sorted order
		void sort(std::vector<K>& keys) const;

		// return the number of keys in collection
		int size() const;

	private:
		// helper to empty entire hash table
		void make_empty();

		// resize and rehash the hash table
		void resize_and_rehash();

		// linked list node structure
		struct Node {
			K key;
			V value;
			Node* next;
		};

	// number of k-v pairs in the collection
	int collection_size;

	// number of hash table buckets(default is 16)
	int table_capacity;

	// hash table array load factor(set at 75% for resizing)
	const double load_factor_threshold;

	// hash table array
	Node** hash_table;
};


template <typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(): collection_size(0), table_capacity(16), load_factor_threshold(0.75) {
	// dynamically allocate the hash table array
	hash_table = new Node*[table_capacity];
	// initialize the hash table chains
	for(int i = 0; i < table_capacity; ++i)
		hash_table[i] = nullptr;
}

template <typename K, typename V>
void HashTableCollection<K,V>::make_empty() {
	// make sure hash table exists
	if(!hash_table) 
		return;
	// remove each key
	for (int i = 0; i < table_capacity; i++) {
		Node* curr_node = hash_table[i];
		Node* previous = curr_node;
		while (curr_node) {
			previous = curr_node;
			curr_node = curr_node->next;
			delete previous;
		}
	}
	delete hash_table;
	hash_table = nullptr;
	collection_size = 0;
}

template <typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection() {
	make_empty();
}


template <typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs): hash_table(nullptr), load_factor_threshold(rhs.load_factor_threshold) {
	*this = rhs;
}

template <typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::operator=(const HashTableCollection<K,V>& rhs) {
	// check if rhs is current object and return current object
	if(this == &rhs)
		return *this;
	// delete current object
	make_empty();
	// initialize current object
	collection_size = 0;
	table_capacity = rhs.table_capacity;
	// create the hash table
	hash_table = new Node*[table_capacity];
	for(int i = 0; i < table_capacity; ++i)
		hash_table[i] = nullptr;
	// do the copy
	Node* curr_node = new Node;
	Node* lhs_node;
	for(int i = 0; i < table_capacity; i++) {
		curr_node = rhs.hash_table[i];
		while(curr_node) {
			insert(curr_node->key, curr_node->value);
			curr_node = curr_node->next;
		}
	}
	return *this;
}

template <typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash() {
	// setup new table
	int new_capacity = table_capacity * 2;
	int new_collection_size = 0;
	// dynamically allocate the new table
	Node** new_table = new Node*[new_capacity];
	// initialize new table
	for(int i = 0; i < new_capacity; ++i)
		new_table[i] = nullptr;
	// insert key values
	std::vector<K> ks;
	keys(ks);

	for(K key: ks) {
		// hash the key
		std::hash<K> hash_fun;
		size_t value = hash_fun(key);
		size_t index = value % new_capacity;
		// create a new node in new table
		Node* ptr = new Node;
		V val;
		find(key, val);
		ptr->key = key;
		ptr->value = val;
		ptr->next = new_table[index];
		new_table[index] = ptr;
		new_collection_size++;
	}
	// clear the current data
	make_empty();
	// update to the new settings
	hash_table = new_table;
	table_capacity = new_capacity;
	collection_size = new_collection_size;
}

template <typename K, typename V>
void HashTableCollection<K,V>::insert(const K& key , const V& val) {
	// check current load factor versus load factor threshold ,
	// and resize and copy if necessary by calling resize_and_rehash()
	double load_factor = static_cast<double>(collection_size) / table_capacity;
	if (load_factor > 0.75)
		resize_and_rehash();
	// hash the key
	std::hash<K> hash_fun;
	size_t value = hash_fun(key);
	size_t index = value % table_capacity;
	// create the new node
	Node* ptr = new Node;
	ptr->key = key;
	ptr->value = val;
	ptr->next = nullptr;

	Node* curr_node;
	if (!hash_table[index])
		hash_table[index] = ptr;
	else {
		curr_node = hash_table[index];
		hash_table[index] = ptr;
		ptr->next = curr_node;
	}
	// update the size
	collection_size++;
}

template <typename K, typename V>
void HashTableCollection<K,V>::remove(const K& key) {
	if (collection_size == 0)
		return;

	std::hash<K> hash_fun;
	size_t value = hash_fun(key);
	size_t index = value % table_capacity;

	Node* curr_node = hash_table[index];
	Node* curr_node_previous = curr_node;
	while (curr_node) {
		if (curr_node->key == key) {
			if (curr_node == hash_table[index]) {
				if (!curr_node->next)
					hash_table[index] = nullptr;
				else
					hash_table[index] = curr_node->next;
			}
			else 
				curr_node_previous->next = curr_node->next;
			delete curr_node;
			collection_size--;
			return;
		}
		curr_node_previous = curr_node;
		curr_node = curr_node->next;
	}
	return;

}

template <typename K, typename V>
bool HashTableCollection<K,V>::find(const K& key , V& val) const {
	if (collection_size == 0)
		return false;

	std::hash<K> hash_fun;
	size_t value = hash_fun(key);
	size_t index = value % table_capacity;

	Node* curr_node = hash_table[index];
	while (curr_node) {
		if (curr_node->key == key) {
			val = curr_node->value;
			return true;
		}
		curr_node = curr_node->next;
	}
	return false;
}

template <typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const {
	if (collection_size == 0)
		return;
	keys.clear();
	std::vector<K> all_keys;
	this->keys(all_keys);
	for (int i = 0; i < all_keys.size(); i++) {
		if (all_keys[i] >= k1 && all_keys[i] <= k2)
			keys.push_back(all_keys[i]);
	}
	return;
}

template <typename K, typename V>
void HashTableCollection<K,V>::keys(std::vector<K>& keys) const {
	keys.clear();
	if (collection_size == 0)
		return;

	Node* curr_node;
	for(int i = 0; i < table_capacity; i++) {
		if(hash_table[i]) {
			curr_node = hash_table[i];
			while(curr_node) {
				keys.push_back(curr_node->key);
				curr_node = curr_node->next;
			}
		}
	}
	return;
}

template <typename K, typename V>
void HashTableCollection<K,V>::sort(std::vector<K>& ks) const {
	if (collection_size == 0)
		return;
	keys(ks);
	std::sort(ks.begin(), ks.end());
}

template <typename K, typename V>
int HashTableCollection<K,V>::size() const {
	return collection_size;
}

#endif
