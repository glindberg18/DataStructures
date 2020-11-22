/*
Greeley Lindberg
11/26/19, hw10
Description: Implementation of Collection using a red black tree.
*/

#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"


template <typename K, typename V>
class RBTCollection : public Collection<K,V> {
public:

	// create an empty linked list
	RBTCollection();

	// copy a linked list
	RBTCollection(const RBTCollection<K,V>& rhs);

	// assign a linked list
	RBTCollection<K,V>& operator =(const RBTCollection<K,V>& rhs);

	// delete a linked list
	~RBTCollection();

	// insert a key -value pair into the collection
	void insert(const K& key, const V& val);

	// remove a key -value pair from the collection
	void remove(const K& key);

	// find the value associated with the key
	bool find(const K& key, V& val) const;

	// find the keys associated with the range
	void find(const K& k1, const K& k2, std::vector <K>& keys) const;

	// return all keys in the collection
	void keys(std::vector <K>& keys) const;

	// return collection keys in sorted order
	void sort(std::vector <K>& keys) const;

	// return the number of keys in collection
	int size() const;

	// return the height of the tree
	int height() const;

private:

	// binary search tree node structure
	struct Node {
		K key;
		V value;
		Node* left;
		Node* right;
		bool is_black;
	};

	// root node of the search tree
	Node* root;

	// number of k-v pairs in the collection
	int collection_size;

	// helper to recursively empty search tree
	void make_empty(Node* subtree_root);

	// recursive helper to do red-black insert key-val pair (backtracking)
	Node* insert(const K& key, const V& val, Node* subtree_root);

	// helper function to perform a single right rotation
	Node* rotate_right(Node* k2);

	// helper fuction to perform single left rotation
	Node* rotate_left(Node* k2);

	// helper to recursively build sorted list of keys
	void inorder(const Node* subtree, std::vector <K>& keys) const;

	// helper to recursively build sorted list of keys
	void preorder(const Node* subtree, std::vector <K>& keys) const;

	// helper to recursively find range of keys
	void range_search(const Node* subtree, const K& k1, const K& k2,
	std::vector <K>& keys) const;

	// helper to reursively remove key node from subtree
	Node* remove(const K& key, Node* subtree_root);

	// return the height of the tree rooted at subtree_root
	int height(const Node* subtree_root) const;

};


template <typename K, typename V>
RBTCollection<K,V>::RBTCollection(): collection_size (0), root(nullptr) {}


template <typename K, typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root) {
	if (!subtree_root)
		return;

	make_empty(subtree_root->left);
	make_empty(subtree_root->right);
	delete subtree_root;
	collection_size--;
}


template <typename K, typename V>
RBTCollection<K,V>::~RBTCollection() {
	make_empty(root);
}


template <typename K, typename V>
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs): collection_size (0), root(nullptr) {
	*this = rhs;
}


template <typename K, typename V>
RBTCollection<K,V>& RBTCollection<K,V>::operator =(const RBTCollection<K,V>& rhs) {
	if (this == &rhs)
		return *this;
	// delete current
	make_empty(root);
	root = nullptr;
	// build tree
	std::vector <K> ks;
	preorder(rhs.root, ks);
	V val;
	for (int i = 0; i < ks.size(); i++) {
		rhs.find(ks[i], val);
		insert(ks[i], val);
	}

	return *this;
}

template <typename K, typename V>
typename RBTCollection<K,V>::Node* RBTCollection<K,V>::rotate_right(Node* k2) {
	Node* k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	return k1;
}

template <typename K, typename V>
typename RBTCollection<K,V>::Node* RBTCollection<K,V>::rotate_left(Node* k2) {
	Node* k1 = k2->right;
	k2->right = k1->left;
	k1->left = k2;
	return k1;
}

template <typename K, typename V>
typename RBTCollection<K,V>::Node*
RBTCollection<K,V>::insert(const K& key, const V& val, Node* subtree_root) {
	Node* ptr = new Node;
	ptr->key = key;
	ptr->value = val;
	ptr->left = nullptr;
	ptr->right = nullptr;
	ptr->is_black = false;
	 if (!root) 
	 	return ptr;
	if (key < subtree_root->key) 
		if (!subtree_root->left)
			subtree_root->left = ptr;
		else
			subtree_root->left = insert(key, val, subtree_root->left);
	else
		if (!subtree_root->right)
			subtree_root->right = ptr;
		else
			subtree_root->right = insert(key, val, subtree_root->right);

	// check if subtree_root is a grandparent
	if ((subtree_root->left && (subtree_root->left->left || subtree_root->left->right)) || 
		(subtree_root->right && (subtree_root->right->left || subtree_root->right->right))) {
		// check if parent and child are red
		Node* parent = nullptr;
		Node* sibling = nullptr;
		Node* child = nullptr;
	
		if (subtree_root->left && !subtree_root->left->is_black && 
		   ((subtree_root->left->left  && !subtree_root->left->left->is_black) ||
		    (subtree_root->left->right  && !subtree_root->left->right->is_black))) {
			if (subtree_root->left->left && !subtree_root->left->left->is_black) {
				child = subtree_root->left->left;
				parent = subtree_root->left;
				sibling = subtree_root->right;
			}
			else if (subtree_root->left->right && !subtree_root->left->right->is_black) {
				child = subtree_root->left->right;
				parent = subtree_root->left;
				sibling = subtree_root->right;
			}
		}
	    else if (subtree_root->right && !subtree_root->right->is_black &&
	    		((subtree_root->right->left && !subtree_root->right->left->is_black) || 
	    		(subtree_root->right->right && !subtree_root->right->right->is_black))) {
			if (subtree_root->right->left && !subtree_root->right->left->is_black) {
				child = subtree_root->right->left;
				parent = subtree_root->right;
				sibling = subtree_root->left;
			}
			else if (subtree_root->right->right && !subtree_root->right->right->is_black) {
				child = subtree_root->right->right;
				parent = subtree_root->right;
				sibling = subtree_root->left;
			}
		}
		else 
			return subtree_root;

		// case 1: the sibling is red
		if (sibling && !sibling->is_black) {
			subtree_root->is_black = false;
			subtree_root->right->is_black = true;
			subtree_root->left->is_black = true;
		}
		// case 2: the sibling is black and the child is "outside"
		else if (subtree_root->left == parent && parent->left == child) {
			subtree_root = rotate_right(subtree_root);
			subtree_root->right->is_black = false;
			subtree_root->left->is_black = false;
		}
		else if (subtree_root->right == parent && parent->right == child) {
			subtree_root = rotate_left(subtree_root);
			subtree_root->right->is_black = false;
			subtree_root->left->is_black = false;
		}
		//case 3: sibling is black and the child is "inside"
		else if (subtree_root->left == parent && parent->right == child) {
			subtree_root->left = rotate_left(subtree_root->left);
			subtree_root = rotate_right(subtree_root);
			subtree_root->right->is_black = false;
			subtree_root->left->is_black = false;
		}
		else if (subtree_root->right == parent && parent->left == child) {
			subtree_root->right = rotate_right(subtree_root->right);
			subtree_root = rotate_left(subtree_root);
			subtree_root->right->is_black = false;
			subtree_root->left->is_black = false;
		}
	}
	return subtree_root;
}


template <typename K, typename V>
void RBTCollection<K,V>::insert(const K& key, const V& val) {
	root = insert(key, val, root);
	root->is_black = true;
	collection_size++;
}

template <typename K, typename V>
typename RBTCollection<K,V>::Node*
RBTCollection<K,V>::remove(const K& key, Node* subtree_root) {
	if (!subtree_root)
		return subtree_root;
	// find key
	if (subtree_root && key < subtree_root->key) 
		subtree_root->left = remove(key, subtree_root->left);
	else if (subtree_root && key > subtree_root->key)
		subtree_root->right = remove(key, subtree_root->right);
	else if (subtree_root && key == subtree_root->key) {
		// no children
		if (!subtree_root->left && !subtree_root->right) {
			delete subtree_root;
			subtree_root = nullptr;
		}
		// one child
		else if (!subtree_root->left || !subtree_root->right) {
			Node* temp;
			// left child
			if (subtree_root->left) {
				temp = subtree_root->left;
				subtree_root->key = subtree_root->left->key;
				subtree_root->value = subtree_root->left->value;
				subtree_root->right = subtree_root->left->right;
				subtree_root->left = subtree_root->left->left;
			}
			//right child
			else {
				temp = subtree_root->right;
				subtree_root->key = subtree_root->right->key;
				subtree_root->value = subtree_root->right->value;
				subtree_root->left = subtree_root->right->left;
				subtree_root->right = subtree_root->right->right;
			}
			delete temp;
			temp = nullptr;
		}
		// two children
		else {
			Node* successor = subtree_root->right;
			Node* parent = successor;
			while (successor->left) {
				parent = successor;
				successor = successor->left;
			}
			subtree_root->key = successor->key;
			subtree_root->value = successor->value;
			Node* temp;
			if (successor->right) {
				temp = successor->right;
				successor->key = successor->right->key;
				successor->value = successor->right->value;
				successor->left = successor->right->left;
				successor->right = successor->right->right;
				delete temp;
				temp = nullptr;
			}
			else {
				if (successor->key == subtree_root->right->key)
					subtree_root->right = nullptr;
				else
					parent->left = nullptr;
				delete successor;
				successor = nullptr;
			}
		}
	}
	return subtree_root;
}


template <typename K, typename V>
void RBTCollection<K,V>::remove(const K& key) {
	if (!root)
		return;
	root = remove(key, root);
	collection_size--;
}


template <typename K, typename V>
bool RBTCollection<K,V>::find(const K& key, V& val) const {
	Node* curr = root;
	while (curr)
		if (key == curr->key) {
			val = curr->value;
			return true;
		}
		else if (key < curr->key)
			curr = curr->left;
		else
			curr = curr->right;

	return false;
}


template <typename K, typename V> void
RBTCollection<K,V>::range_search(const Node* subtree, const K& k1, const K& k2, std::vector <K>& ks) const {
	if (!subtree)
		return;

	if (subtree->key < k1)
		range_search(subtree->right, k1, k2, ks);
	else if (subtree->key >= k1 && subtree->key <= k2) {
		ks.push_back(subtree->key);
		range_search(subtree->left, k1, k2, ks);
		range_search(subtree->right, k1, k2, ks);
	}
	else
		range_search(subtree->left, k1, k2, ks);
}


template <typename K, typename V> void
RBTCollection<K,V>::find(const K& k1, const K& k2, std::vector <K>& ks) const {
	// defer to the range search (recursive) helper function
	ks.clear();
	range_search(root, k1, k2, ks);
}


template <typename K, typename V>
void RBTCollection<K,V>::inorder(const Node* subtree, std::vector <K>& ks) const {
	if (!subtree)
		return;

	inorder(subtree->left, ks);
	ks.push_back(subtree->key);
	inorder(subtree->right, ks);
}


template <typename K, typename V>
void RBTCollection<K,V>::preorder(const Node* subtree, std::vector <K>& ks) const {
	if (!subtree)
		return;

	ks.push_back(subtree->key);
	preorder(subtree->left, ks);
	preorder(subtree->right, ks);
}


template <typename K, typename V>
void RBTCollection<K,V>::keys(std::vector <K>& ks) const {
	// defer to the inorder (recursive) helper function
	ks.clear();
	inorder(root, ks);
}


template <typename K, typename V>
void RBTCollection<K,V>::sort(std::vector <K>& ks) const {
	// defer to the inorder (recursive) helper function
	ks.clear();
	inorder(root, ks);
	std::sort(ks.begin(), ks.end());
}


template <typename K, typename V>
int RBTCollection<K,V>::size() const
{
	return collection_size;
}


template <typename K, typename V>
int RBTCollection<K,V>::height(const Node* subtree_root) const {
	int left_height;
	int right_height;

	if (!subtree_root)
		return 0;

	left_height = height(subtree_root->left);
	right_height = height(subtree_root->right);

	if (left_height > right_height)
		return left_height + 1;
	else
		return right_height + 1;
}


template <typename K, typename V>
int RBTCollection<K,V>::height() const 
{
	// defer to the height (recursive) helper function
	return height(root);
}

#endif
