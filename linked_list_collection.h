/*
Greeley Lindberg
10/3/19, hw4
Description: Implementation of Collection using linked lists
*/

#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"


template <typename K, typename V>
class LinkedListCollection : public Collection<K,V> {
public:

	// create an empty linked list
	LinkedListCollection();

	// copy a linked list
	LinkedListCollection(const LinkedListCollection<K,V>& rhs);

	// assign a linked list
	LinkedListCollection<K,V>& operator =(const LinkedListCollection<K,V>& rhs);

	// delete a linked list
	~LinkedListCollection();

	// insert a key-value pair into the collection
	void insert(const K& key, const V& val);

	// remove a key-value pair from the collection
	void remove(const K& key);

	// find the value associated with the key
	bool find(const K& key, V& val) const;

	// find the keys associated with the range
	void find(const K& k1, const K& k2, std::vector<K>& keys) const;

	// return all keys in the collection
	void keys(std::vector<K>& keys) const;

	// return collection keys in sorted order
	void sort(std::vector<K>& keys) const;

	// return the number of keys in collection
	int size() const;

private:
	// linked list node structure
	struct Node {
		K key;
		V value;
		Node* next;
	};
	Node* head;  		// pointer to first list node
	Node* tail;  		// pointer to last list node
	int length; 		// number of linked list nodes in list
};

template <typename K, typename V>
LinkedListCollection<K,V>::LinkedListCollection() : head(nullptr), tail(nullptr), length(0) {}

template <typename K, typename V>
LinkedListCollection<K,V>::LinkedListCollection(const LinkedListCollection<K,V>& rhs): head(nullptr), tail(nullptr), length(0) {
	Node* ptr = new Node;
	ptr = rhs.head;
	while (ptr != nullptr) {
		insert(ptr->key, ptr->value);
		ptr = ptr->next;
	}
	
}

template <typename K, typename V>
LinkedListCollection<K,V>& LinkedListCollection<K,V>::operator =(const LinkedListCollection<K,V>& rhs) {
	Node* ptr = new Node;
	Node* next_ptr = new Node;
	if (this == &rhs)
		return *this;
	ptr = head;
	while (ptr != nullptr) {
		next_ptr = ptr->next;
		remove(ptr->key);
		ptr = next_ptr;
	}
	ptr = rhs.head;
	while (ptr != nullptr) {
		insert(ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return *this; 
}

template <typename K, typename V>
LinkedListCollection<K,V>::~LinkedListCollection() {
	Node* ptr = new Node;
	Node* next_ptr = new Node;
	ptr = head;
	while (ptr != nullptr) {
		next_ptr = ptr->next;
		remove(ptr->key);
		ptr = next_ptr;
	}
}

template <typename K, typename V>
void LinkedListCollection<K,V>::insert(const K& key, const V& val) {
	Node* ptr = new Node;
	ptr->key = key;
	ptr->value = val;
	ptr->next = nullptr;
	if (!head) {
		head = ptr;
	}
	else 
		tail->next = ptr;
	tail = ptr;
	length++;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::remove(const K& key) {
	Node* ptr = new Node;
	Node* previous = new Node;
	if (!head)
		return;
	else {
		if (head->key == key) {
			if (head==tail)
				tail = nullptr;
			ptr = head->next;
			delete head;
			head = ptr;
			length--;
		}
		else {
			ptr = head;
			while (ptr != nullptr && ptr->key != key) {
				previous = ptr;
				ptr = ptr->next;
			}
			if (ptr)
				previous->next = ptr->next;
				if (tail==ptr)
					tail = previous;
				delete ptr;
				ptr = nullptr;
				length--;
		}
	}
}

template <typename K, typename V>
bool LinkedListCollection<K,V>::find(const K& key, V& val) const {
	Node* ptr = new Node;
	ptr = head;
	while (ptr != nullptr) {
		if (ptr->key == key) {
			val = ptr->value;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const {
	keys.clear();
	Node* ptr = new Node;
	ptr = head;
	while (ptr != nullptr) {
		if (ptr->key == k1) {
			while (ptr != nullptr) {
				keys.push_back(ptr->key);
				if (ptr->key == k2)
					return;
				ptr = ptr->next;
			}
			break;
		}
		ptr = ptr->next;
	}
	keys.clear(); 		// if k1 was found but not k2, clear the pushbacked keys
	return;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::keys(std::vector<K>& keys) const {
	keys.clear();
	Node* ptr = new Node;
	ptr = head;
	int i = 0;
	while (ptr != nullptr) {
		keys.push_back(ptr->key);
		ptr = ptr->next;
		i++;
	}
}

template <typename K, typename V>
void LinkedListCollection<K,V>::sort(std::vector <K>& keys) const {
	Node* ptr = new Node;
	ptr = head;
	while (ptr != nullptr) {
		keys.push_back(ptr->key);
		ptr = ptr->next;
	}
	std::sort(keys.begin(), keys.end());
}

template <typename K, typename V>
int LinkedListCollection<K,V>::size() const {
	return length;
}

#endif
