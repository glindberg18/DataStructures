/*
Greeley Lindberg
12/11/19, hw11
Description: Implementation of Collection using a red black tree.
*/

#ifndef DBL_RBT_COLLECTION_H
#define DBL_RBT_COLLECTION_H

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

	// insert a key-value pair into the collection
	void insert(const K& key, const V& val);

	// remove a key-value pair from the collection
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

	// print for testing
	void print() const;

private:

	// binary search tree node structure
	struct Node {
		K key;
		V value;
		Node* left;
		Node* right;
		bool is_black;
		bool is_dbl_black_left;
		bool is_dbl_black_right;
	};

	// root node of the search tree
	Node* root;

	// number of k-v pairs in the collection
	int collection_size;

	// helper to recursively empty search tree
	void make_empty(Node* subtree_root);

	// recursive helper to remove node with given key
	Node* remove(const K& key, Node* parent, Node* subtree_root, bool& found);

	// helper to perform a single rebalance step on a red-black tree on remove
	Node* remove_color_adjust(Node* parent);

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

	// helper to recursively print
	void print(Node* subtree_root) const;

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
	ptr->is_dbl_black_left = false;
	ptr->is_dbl_black_right = false;
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
		   ((subtree_root->left->left && !subtree_root->left->left->is_black) ||
		    (subtree_root->left->right && !subtree_root->left->right->is_black))) {
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
			subtree_root->is_black = true;
			subtree_root->right->is_black = false;
			subtree_root->left->is_black = false;
		}
		else if (subtree_root->right == parent && parent->left == child) {
			subtree_root->right = rotate_right(subtree_root->right);
			subtree_root = rotate_left(subtree_root);
			subtree_root->is_black = true;
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
void RBTCollection <K,V>::remove(const K& key) {
	// check if anything to remove
	if (root == nullptr)
		return;
	// create a "fake" root to pass in as parent of root
	Node* root_parent = new Node;
	root_parent->key = root->key;
	root_parent->left = nullptr;
	root_parent->right = root;
	root_parent->is_black = true;
	root_parent->is_dbl_black_left = false;
	root_parent->is_dbl_black_right = false;
	// call remove
	bool found = false;
	root_parent = remove(key, root_parent, root, found);
	// update results
	if (found) {
		collection_size--;
		root = root_parent->right;

		if (root) {
			root->is_black = true;
			root->is_dbl_black_right = false;
			root->is_dbl_black_left = false;
		}
	}
	delete root_parent;
}


template <typename K, typename V>
typename RBTCollection <K,V>::Node*
RBTCollection <K,V>::remove(const K& key, Node* parent, Node* subtree_root, bool& found) {
	if (subtree_root && key < subtree_root->key)
		subtree_root = remove(key, subtree_root, subtree_root->left, found);
	else if (subtree_root && key > subtree_root->key)
		subtree_root = remove(key, subtree_root, subtree_root->right, found);
	else if (subtree_root && key == subtree_root->key) {
		found = true;
		// leaf node
		if (!subtree_root->left && !subtree_root->right) {
			// if node is black then set double-black, adjust parent,
			// and delete subtree root ...
			if (parent->left && parent->left->key == subtree_root->key && subtree_root->is_black) {
				parent->is_dbl_black_left = true;
				parent->left = nullptr;
			}
			else if (parent->right && parent->right->key == subtree_root->key && subtree_root->is_black) {
				parent->is_dbl_black_right = true;
				parent->right = nullptr;
			}
			//if node is red
			else {	
				if (parent->right && parent->right->key == subtree_root->key)
					parent->right = nullptr;
				else
					parent->left = nullptr;
			}
			delete subtree_root;
		}
		// left non-empty but right empty
		else if (subtree_root->left && !subtree_root->right) {
			// similar to above
			subtree_root->left->is_black = true;
			if (parent->left && parent->left->key == subtree_root->key) {
				if (subtree_root->is_black)
					parent->is_dbl_black_left = true;
				parent->left = subtree_root->left;
			}
			else if (parent->right && parent->right->key == subtree_root->key) {
				if (subtree_root->is_black)
					parent->is_dbl_black_right = true;
				parent->right = subtree_root->left;
			}
			delete subtree_root;
			subtree_root = nullptr;
		}
		// left empty but right non-empty
		else if (!subtree_root->left && subtree_root->right) {
			// similar to above
			subtree_root->right->is_black = true;
			if (parent->left && parent->left->key == subtree_root->key) {
				if (subtree_root->is_black)
					parent->is_dbl_black_left = true;
				parent->left = subtree_root->left;
			}
			else if (parent->right && parent->right->key == subtree_root->key) {
				if (subtree_root->is_black)
					parent->is_dbl_black_right = true;
				parent->right = subtree_root->right;
			}
			delete subtree_root;
			subtree_root = nullptr;
		}
		// left and right non empty
		else {
			// find inorder successor (right, then iterate left)
			Node* successor = subtree_root->right;
			while (successor->left)
				successor = successor->left;
			subtree_root->key = successor->key;
			subtree_root->value = successor->value;
			// then call remove again on inorder successor key and subtree 
			// root’s right child once the key and value copy is complete
			subtree_root = remove(successor->key,subtree_root, subtree_root->right, found);
		}
	}

	if (!found)
		return parent;

	// backtracking, adjust color at parent
	return remove_color_adjust(parent);
}


template <typename K, typename V>
typename RBTCollection <K,V>::Node*
RBTCollection <K,V>::remove_color_adjust(Node* subtree_root) {
	// subtree root is "grandparent" g, with left child gl and right child gr
	Node* g = subtree_root;
	Node* gl = g->left;
	Node* gr = g->right;
	// parent p is either gl or gr
	Node* p = nullptr;
	bool left_parent = false;
	if (gl && (gl->is_dbl_black_left || gl->is_dbl_black_right)) {
		p = gl;
		left_parent = true;
	}
	else if (gr && (gr->is_dbl_black_left || gr->is_dbl_black_right))
		p = gr;
	else
		return subtree_root;
	// parent’s left child is a double black node
	if (p->is_dbl_black_left) {
		// do the following cases
		// case 1: red sibling
		if (p->right && !p->right->is_black) {
			p = rotate_left(p);
			p->left->is_black = false;
			p->is_black = true;
			p->left = remove_color_adjust(p->left);
		}
		// case 2-a: black sibling with red child (outside)
		else if (p->right && p->right->is_black && (p->right->right && !p->right->right->is_black)) {
			p = rotate_left(p);
			p->is_black = p->left->is_black;
			p->left->is_black = true;
			p->left->is_dbl_black_left = false;
			p->right->is_black = true;
		}
		// case 2-b: black sibling with red child (inside)
		else if (p->right && p->right->is_black && (p->right->left && !p->right->left->is_black)) {
			p->right = rotate_right(p->right);
			p = rotate_left(p);
			p->is_black = p->left->is_black;
			p->left->is_black = true;
			p->left->is_dbl_black_left = false;
		}
		// case 3-a: black sibling with black children, red parent
		else if (p->right && p->right->is_black && !p->is_black) {
			p->is_black = true;
			p->is_dbl_black_left = false;
			p->right->is_black = false;
		}
		// case 3-b: black sibling with black children, black parent
		else if (p->right && p->right->is_black && p->is_black) {
			p->is_dbl_black_left = false;
			if (left_parent)
				g->is_dbl_black_left = true;
			else
				g->is_dbl_black_right = true;
			p->right->is_black = false;
		}
	}

	// parent’s right child is a double black node
	if (p->is_dbl_black_right) {
		// do the following cases
		// case 1: red sibling
		if (p->left && !p->left->is_black) {
			p = rotate_right(p);
			p->right->is_black = false;
			p->is_black = true;
			p->right = remove_color_adjust(p->right);
		}
		// case 2-a: black sibling with red child (outside)
		else if (p->left && p->left->is_black && (p->left->left && !p->left->left->is_black)){
			p = rotate_right(p);
			p->is_black = p->right->is_black;
			p->right->is_black = true;
			p->right->is_dbl_black_right = false;
			p->left->is_black = true;
		}
		// case 2-b: black sibling with red child (inside)
		else if (p->left && p->left->is_black && (p->left->right && !p->left->right->is_black)){
			p->left = rotate_left(p->left);
			p = rotate_right(p);
			p->is_black = p->right->is_black;
			p->right->is_black = true;
			p->left->is_dbl_black_right = false;
		}
		// case 3-a: black sibling with black children, red parent
		else if (p->left && p->left->is_black && !p->is_black) {
			p->is_black = true;
			p->is_dbl_black_right = false;
			p->left->is_black = false;
		}
		// case 3-b: black sibling with black children, black parent
		else if (p->left && p->left->is_black && p->is_black) {
			p->is_dbl_black_right = false;
			if (left_parent)
				g->is_dbl_black_left = true;
			else
				g->is_dbl_black_right = true;
			p->left->is_black = false;
		}
	}

	// connect up the subtree_root to the parent
	if (left_parent)
		subtree_root->left = p;
	else
		subtree_root->right = p;

	return subtree_root;
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
void RBTCollection<K,V>::print(Node* subtree_root) const {
	if (!subtree_root)
		return;
	std::cout<<subtree_root->key<<" "<<subtree_root->is_black<<"\n";
	print(subtree_root->left);
	print(subtree_root->right);
}

template <typename K, typename V>
void RBTCollection<K,V>::print() const {
	print(root);
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
int RBTCollection<K,V>::size() const {
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
int RBTCollection<K,V>::height() const {
	// defer to the height (recursive) helper function
	return height(root);
}

#endif
