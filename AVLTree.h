#include <memory>
#include "utilesWet2.h"
#include <iostream>

#ifndef AVLTREE_H
#define AVLTREE_H

using namespace std;


template<typename K, typename T>
class Node {
public:
    T data;
    K key;
    Node<K, T> *left;
    Node<K, T> *right;
    int height;
    double extra;

    Node(const T &data, const K &key) : data(data), key(key), left(nullptr), right(nullptr), height(0), extra(0) {}

    ~Node() = default;

    int getHeight(Node<K, T> *node);

    int getBF(Node<K, T> *node);

    void correctHeight(Node<K, T> *node);

    Node<K, T> *shiftRR(Node<K, T> *node);

    Node<K, T> *shiftLL(Node<K, T> *node);

    Node<K, T> *balance(Node<K, T> *node);

    Node<K, T> *removeAux(Node<K, T> *currentNode, const K &key);

    Node<K, T> *getMinNodeRightChild(Node<K, T> *node);
};

template<typename K, typename T>
class AVLTree {
private:
    Node<K, T> *root;
    int size;
    Node<K,T> *findNode(const K &key) const;

public:
    AVLTree() : root(nullptr), size(0) {}

    ~AVLTree();

    int getSize() const;

    bool contains(const K &key);

    T find(const K &key);

    StatusType insert(const K &key, const T &data);

    StatusType remove(const K &key);

    void populateArrayReverseOrder(T *output) const;

    T *getHighestRankedNode() const;

    T *arrayAux(T *output, int &index, Node<K, T> *node) const;

    void eraseTree(Node<K, T> *node);

    Node<K, T> *insertAux(Node<K, T> *currentNode, const T &data, const K &key);

    double getExtraSum(const K &key) const;

    void addExtra(const K &key, double extra);

    void deleteExtras();

    void deleteExtrasAux(Node<K,T> *node);

};

//returns size of tree
template<typename K, typename T>
int AVLTree<K, T>::getSize() const {
    return size;
}

//destructor for AVL tree
template<typename K, typename T>
AVLTree<K, T>::~AVLTree() {
    eraseTree(root);
}

//helper function for destroying AVL tree
template<typename K, typename T>
void AVLTree<K, T>::eraseTree(Node<K, T> *node) {
    if (!node) {
        return;
    }
    eraseTree(node->left);
    eraseTree(node->right);
    delete node;
}

//checks if a tree contains a node with given tree
template<typename K, typename T>
bool AVLTree<K, T>::contains(const K &key) {
    Node<K, T> *currentNode = root;
    while (currentNode) {
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
template<typename K, typename T>
T AVLTree<K, T>::find(const K &key) {
    Node<K, T> *currentNode = root;
    while (currentNode) {
        if (key == currentNode->key)
            return currentNode->data;

        else if (key < currentNode->key)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }
    return nullptr;
}

template<typename K, typename T>
Node<K,T> *AVLTree<K, T>::findNode(const K &key) const {
    Node<K, T> *currentNode = root;
    while (currentNode) {
        if (key == currentNode->key)
            return currentNode;

        else if (key < currentNode->key)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }
    return nullptr;
}

//inserts new node in tree with given key and data 
template<typename K, typename T>
StatusType AVLTree<K, T>::insert(const K &key, const T &data) {
    if (contains(key))
        return StatusType::FAILURE;

    try {
        this->root = insertAux(this->root, data, key);
       
    }
    catch (const bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    this->size++;

    return StatusType::SUCCESS;
}

//helper function for inserting new node to tree
template<typename K, typename T>
Node<K, T> *AVLTree<K, T>::insertAux(Node<K, T> *currentNode, const T &data, const K &key) {
    try {
        if (currentNode == nullptr) {
            Node<K, T> *newNode = new Node<K, T>(data, key);
            currentNode = newNode;
            currentNode->extra = -getExtraSum(key);
            return currentNode;
        }
        if (key < currentNode->key)
            currentNode->left = insertAux(currentNode->left, data, key);
        else
            currentNode->right = insertAux(currentNode->right, data, key);

        return currentNode->balance(currentNode);
    }
    catch (const bad_alloc &e) {
        throw;
    }
}

//returns height of given node
template<typename K, typename T>
int Node<K, T>::getHeight(Node<K, T> *node) {
    return node ? node->height : 0;
}

//returns balance factor of given node
template<typename K, typename T>
int Node<K, T>::getBF(Node<K, T> *node) {
    int leftHeight = node->left ? (getHeight(node->left) + 1) : 0;
    int rightHeight = node->right ? (getHeight(node->right) + 1) : 0;
    return leftHeight - rightHeight;
}

//corrects the height of given node
template<typename K, typename T>
void Node<K, T>::correctHeight(Node<K, T> *node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    if (!(node->left) && !(node->right))
        node->height = 0;
    else
        node->height = (rightHeight > leftHeight) ? (rightHeight + 1) : (leftHeight + 1);
}

//performs the LL shift on given node
template<typename K, typename T>
Node<K, T> *Node<K, T>::shiftLL(Node<K, T> *node) {
    Node<K, T> *newParent = node->left;
    node->left = newParent->right;
    newParent->right = node;
    correctHeight(node);
    correctHeight(newParent);

    //update extras
    double oldExtraNewParent = newParent->extra;
    double oldExtraNode = node->extra;

    newParent->extra += oldExtraNode;
    node->extra = -oldExtraNewParent;
    if (node->left) {
        node->left->extra += oldExtraNewParent;

    }

    return newParent;
}

//performs the RR shift on given node
template<typename K, typename T>
Node<K, T> *Node<K, T>::shiftRR(Node<K, T> *node) {
    Node<K, T> *newParent = node->right;
    node->right = newParent->left;
    newParent->left = node;
    correctHeight(node);
    correctHeight(newParent);

    //update extras
    double oldExtraNewParent = newParent->extra;
    double oldExtraNode = node->extra;

    newParent->extra += oldExtraNode;
    node->extra = -oldExtraNewParent;
    if (node->right) {
        node->right->extra += oldExtraNewParent;

    }

    return newParent;
}

//if given node has invalid balance factor, performs relevant shifts according to layout of the tree
template<typename K, typename T>
Node<K, T> *Node<K, T>::balance(Node<K, T> *node) {
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
template<typename K, typename T>
Node<K, T> *findMin(Node<K, T> *node) {
    return node->left ? findMin(node->left) : node;
}

//returns minimum child of given node
template<typename K, typename T>
Node<K, T> *Node<K, T>::getMinNodeRightChild(Node<K, T> *node) {
    if (!node->left) {
        return node->right;
    }
    node->left = getMinNodeRightChild(node->left);
    return balance(node);
}


//helper function to remove a node with given key from tree
template<typename K, typename T>
Node<K, T> *Node<K, T>::removeAux(Node<K, T> *currentNode, const K &key) {
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
            Node<K, T> *leftChild = currentNode->left;
            Node<K, T> *rightChild = currentNode->right;
            delete currentNode;
            if (!rightChild) {
                return leftChild;
            }
            Node<K, T> *minNode = findMin(rightChild);
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
template<typename K, typename T>
StatusType AVLTree<K, T>::remove(const K &key) {
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
template<typename K, typename T>
void AVLTree<K, T>::populateArrayReverseOrder(T *output) const {
    int index = 0;
    arrayAux(output, index, root);
    return;
}

//helper function for populateArrayReverseOrder
template<typename K, typename T>
T *AVLTree<K, T>::arrayAux(T *output, int &index, Node<K, T> *node) const {
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
template<typename K, typename T>
T *AVLTree<K, T>::getHighestRankedNode() const {
    Node<K, T> *currentNode = root;
    if (!root) {
        return nullptr;
    }
    while (currentNode->right)
        currentNode = currentNode->right;
    return &currentNode->data;
}

template<typename K, typename T>
double AVLTree<K, T>::getExtraSum(const K &key) const {
    Node<K, T> *currentNode = root;
    double sum = 0;

    while (currentNode) {
        sum += currentNode->extra;

        if (currentNode->key == key)
            break;
        else if (key < currentNode->key)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }

    return sum;
}

template<typename K, typename T>
void AVLTree<K, T>::addExtra(const K &key, double extra) {
    bool isRightSequence = false;

    Node<K, T> *currentNode = root;

    while (currentNode) {
        if (key == currentNode->key) {
            if (!isRightSequence) {
                currentNode->extra += extra;
            }
            if (currentNode->right) {
                currentNode->right->extra -= extra;
            }
            break;
        } else if (key < currentNode->key) {
            if (isRightSequence) {
                currentNode->extra -= extra;
            }
            isRightSequence = false;
            currentNode = currentNode->left;
        } else {
            if (!isRightSequence) {
                currentNode->extra += extra;
                isRightSequence = true;
            }
            currentNode = currentNode->right;
        }
    }
}
template<typename K, typename T>
void AVLTree<K,T>::deleteExtras(){
    deleteExtrasAux(root);
}

template<typename K, typename T>
void AVLTree<K,T>::deleteExtrasAux(Node<K,T> *node){
    if (!node)
        return;

    if (node->right)
        deleteExtrasAux(node->right);

    node->extra = 0;

    if (node->left)
        deleteExtrasAux(node->left);

    return;
}

#endif