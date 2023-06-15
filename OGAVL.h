#include <memory>
#include "utilesWet2.h"

#ifndef OG_AVLTREE_H
#define OG_AVLTREE_H

using namespace std;


template <typename K, typename T>
class OGNode {
public:
    T data;
    K key;
	OGNode<K,T>* left;
	OGNode<K,T>* right;
	int height;
	OGNode(const T& data, const K& key) : data(data), key(key), left(nullptr), right(nullptr), height(0) {}
	~OGNode() = default;
	int getHeight (OGNode<K,T>* node);
	int getBF (OGNode<K,T>* node);
    void correctHeight(OGNode<K,T>* node);
    OGNode<K,T>* shiftRR(OGNode<K,T>* node);
    OGNode<K,T>* shiftLL(OGNode<K,T>* node);
    OGNode<K,T>* balance(OGNode<K,T>* node);
    OGNode<K,T>* removeAux(OGNode<K,T>* currentNode, const K& key);
    OGNode<K,T>* getMinNodeRightChild(OGNode<K,T>* node);
};

template <typename K, typename T>
class OGAVLTree {
private:
    OGNode<K,T>* root;
    int size;

public:
    OGAVLTree() : root(nullptr), size(0) {}
    ~OGAVLTree();
    int getSize() const;
    bool contains(const K &key);
    T find(const K &key);
    StatusType insert(const K &key, const T &data);
    StatusType remove(const K &key);
    void populateArrayReverseOrder(T* output) const;
    T* getHighestRankedNode() const;
    T* arrayAux(T* output, int& index, OGNode<K,T>* node) const;
    void eraseTree (OGNode<K,T> *node);
    OGNode<K,T>* insertAux(OGNode<K,T>* currentNode, const T &data, const K& key);

};

//returns size of tree
template <typename K, typename T>
int OGAVLTree<K,T>::getSize() const {
    return size;
}

//destructor for AVL tree
template <typename K, typename T>
OGAVLTree<K,T>::~OGAVLTree() {
    eraseTree(root);
}

//helper function for destroying AVL tree
template <typename K, typename T>
void OGAVLTree<K,T>::eraseTree(OGNode<K,T> *node) {
    if (!node) {
        return;
    }
    eraseTree(node->left);
    eraseTree(node->right);
    delete node;
}

//checks if a tree contains a node with given tree
template <typename K, typename T>
bool OGAVLTree<K,T>::contains(const K &key) {
	OGNode<K,T>* currentNode = root;
	while(currentNode) {
		if (key == currentNode->key)
			return true;
		else if (key < currentNode->key) 
			currentNode = currentNode->left;
		else
			currentNode = currentNode->right;
	}
	return false;
}

//returns data of node with given key
template <typename K, typename T>
T OGAVLTree<K,T>::find(const K &key) {
	OGNode<K,T>* currentNode = root;
	while(currentNode) {
		if (key == currentNode->key)
			return currentNode->data;
            
		else if (key < currentNode->key) 
			currentNode = currentNode->left;
		else
			currentNode = currentNode->right;
	}
    return nullptr;
}

//inserts new node in tree with given key and data 
template <typename K, typename T>
StatusType OGAVLTree<K,T>::insert(const K &key, const T &data) {
    if (contains(key))
        return StatusType::FAILURE;

    try {
        this->root = insertAux(this->root, data, key);
    }
    catch (const bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    this->size++;

    return StatusType::SUCCESS;
}

//helper function for inserting new node to tree
template <typename K, typename T>
OGNode<K,T>* OGAVLTree<K,T>::insertAux(OGNode<K,T>* currentNode, const T &data, const K& key) {
    try {
        if (currentNode == nullptr) {            
            OGNode<K,T>* newNode = new OGNode<K,T>(data, key);
            currentNode = newNode;
            return currentNode;
        }
        if (key < currentNode->key)
            currentNode->left = insertAux(currentNode->left, data, key);
        else
            currentNode->right = insertAux(currentNode->right, data, key);
        
        return currentNode->balance(currentNode);
    }
    catch (const bad_alloc& e) {
        throw;
    } 
}

//returns height of given node
template <typename K, typename T>
int OGNode<K,T>::getHeight(OGNode<K,T>* node) {
	return node ? node->height : 0;
}

//returns balance factor of given node
template <typename K, typename T>
int OGNode<K,T>::getBF(OGNode<K,T>* node) {
    int leftHeight = node->left ? (getHeight(node->left) + 1) : 0;
    int rightHeight = node->right ? (getHeight(node->right) + 1) : 0;
	return leftHeight - rightHeight;
}

//corrects the height of given node
template <typename K, typename T>
void OGNode<K,T>::correctHeight(OGNode<K,T>* node) {
  int leftHeight = getHeight(node->left);
  int rightHeight = getHeight(node->right);
  if (!(node->left) && !(node->right))
    node->height = 0;
  else
    node->height = (rightHeight > leftHeight) ? (rightHeight + 1) : (leftHeight + 1);
 }

//performs the LL shift on given node
template <typename K, typename T>
OGNode<K,T>* OGNode<K,T>::shiftLL(OGNode<K,T>* node) {
    OGNode<K,T>* newParent = node->left;
    node->left = newParent->right;
    newParent->right = node;
    correctHeight(node);
    correctHeight(newParent);
    return newParent;
}

//performs the RR shift on given node
template <typename K, typename T>
OGNode<K,T>* OGNode<K,T>::shiftRR(OGNode<K,T>* node) {
    OGNode<K,T>* newParent = node->right;
    node->right = newParent->left;
    newParent->left = node;
    correctHeight(node);
    correctHeight(newParent);
    return newParent;
}

//if given node has invalid balance factor, performs relevant shifts according to layout of the tree
template <typename K, typename T>
OGNode<K,T>* OGNode<K,T>::balance(OGNode<K,T>* node) {
    correctHeight(node);
    int balanceFactor = getBF(node);
    if (balanceFactor == 2) {
        if (node->left && (getBF(node->left) < 0)) {
            //requires shift LR
            node->left = shiftRR(node->left);
        }
        return shiftLL(node);
    } else if (balanceFactor == -2) {
        if (node->right && (getBF(node->right) > 0)) {
            //requires shift RL
            node->right = shiftLL(node->right);
        }
        return shiftRR(node);
    }
    return node;
}

//returns node's left child if one exits, otherwise returns node
template <typename K, typename T>
OGNode<K,T>* findMin(OGNode<K,T>* node) {
  return node->left ? findMin(node->left) : node;
}

//returns minimum child of given node
template <typename K, typename T>
OGNode<K,T>* OGNode<K,T>::getMinNodeRightChild(OGNode<K,T>* node) {
  if (!node->left) {
      return node->right;
  }
  node->left = getMinNodeRightChild(node->left);
  return balance(node);
}


//helper function to remove a node with given key from tree
template <typename K, typename T>
OGNode<K,T>* OGNode<K,T>::removeAux(OGNode<K,T>* currentNode, const K& key) {
    try {
        if (currentNode == nullptr) {
            return nullptr;
        }
        if (key < currentNode->key) {
            currentNode->left = removeAux(currentNode->left, key);
        } else if (key > currentNode->key) {
            currentNode->right = removeAux(currentNode->right, key);
        } else {
            //currentNode is the node with the given key that we want to delete
            OGNode<K,T>* leftChild = currentNode->left;
            OGNode<K,T>* rightChild = currentNode->right;
            delete currentNode;
            if (!rightChild) {
                return leftChild;
            }
            OGNode<K,T>* minNode = findMin(rightChild);
            minNode->right = getMinNodeRightChild(rightChild);
            minNode->left = leftChild;
            
            return balance(minNode);
        }
        return balance(currentNode);
    } 
    catch (...) {
        throw;
    }
}

//removes a node with the given key from the tree
template <typename K, typename T>
StatusType OGAVLTree<K,T>::remove(const K &key) {
    try {
        if (!contains(key)) {
            //if there is no node with specified key
            return StatusType::FAILURE;
        }
        root = root->removeAux(root, key);
        this->size--;
        return StatusType::SUCCESS;
    } catch (...) {
        this->size++;
        return StatusType::ALLOCATION_ERROR;
    }
}

//fills output array with data objects from the tree in descending order
template <typename K, typename T>
void OGAVLTree<K,T>::populateArrayReverseOrder(T* output) const {
    int index = 0;
    arrayAux(output, index, root);
    return;
}

//helper function for populateArrayReverseOrder
template <typename K, typename T>
T* OGAVLTree<K,T>::arrayAux(T* output, int& index, OGNode<K,T>* node) const {
    if (!node) {
        return output;
    } 
    if (node->right) 
        arrayAux(output, index, node->right);

    output[index++] = node->data;

    if (node->left) 
        arrayAux(output, index, node->left);

    return output;
}

//returns node with heighest ranking on the tree
template <typename K, typename T>
T* OGAVLTree<K,T>::getHighestRankedNode() const {
	OGNode<K,T>* currentNode = root;
    if (!root) {
        return nullptr;
    }
	while(currentNode->right)
		currentNode = currentNode->right;
	return &currentNode->data;
}

#endif