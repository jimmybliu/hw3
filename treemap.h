#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>
#include <map>

template <typename K, typename V>
class Treemap {
public:
    //
    // @@@ The class's public API below should _not_ be modified @@@
    //

    // * Capacity
    // Returns number of key-value mappings in map --O(1)
    size_t Size();
    // Returns true if map is empty --O(1)
    bool Empty();

    // * Modifiers
    // Insert @key in map --O(log N) on average
    void Insert(const K &key, const V &value);
    // Remove @key from map --O(log N) on average
    void Remove(const K &key);

    // * Lookup
    // Return value corresponding to @key --O(log N) on average
    const V& Get(const K &key);

    // Return greatest key less than or equal to @key --O(log N) on average
    const K& FloorKey(const K &key);
    // Return least key greater than or equal to @key --O(log N) on average
    const K& CeilKey(const K &key);

    // Return whether @key is found in map --O(log N) on average
    bool ContainsKey(const K& key);
    // Return whether @value is found in map --O(N)
    bool ContainsValue(const V& value);

    // Return max key in map --O(log N) on average
    const K& MaxKey();
    // Return min key in map --O(log N) on average
    const K& MinKey();

    void Print();

private:
    //
    // @@@ The class's internal members below can be modified @@@
    //

    // Private member variables
    // ...To be completed...
    struct Node{
        K key;
        V value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };
    std::unique_ptr<Node> root;
    size_t size = 0;
    // Private constants
    // ...To be completed (if any)...

    // Private methods
    // ...To be completed (if any)...
    void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
    void Remove(std::unique_ptr<Node> &n, const K &key);
    bool ContainsValue(std::unique_ptr<Node> &n, const V& value);
    const V& Get(std::unique_ptr<Node> &n, const K &key);

    void Print(Node *n, int level);
};

//
// Your implementation of the class should be located below
//
template <typename K, typename V>
size_t Treemap<K, V>::Size(){
    return size;
}

template<typename K, typename V>
bool Treemap<K, V>::Empty() {
    return size==0;
}

template<typename K, typename V>
void Treemap<K, V>::Insert(const K &key, const V &value) {
    Insert(root, key, value);
    size++;
}

template<typename K, typename V>
void Treemap<K, V>::Insert(std::unique_ptr<Node> &n, const K &key, const V &value) {
    if (!n){
        n = std::unique_ptr<Node>(new Node{key, value});
    }
    else if(key < n->key){
        Insert(n->left, key, value);
    }
    else if(key > n->key){
        Insert(n->right, key, value);
    }
    else{
        std::cerr<<"Duplicate key " << key << "\n";
    }
}

template<typename K, typename V>
void Treemap<K, V>::Remove(const K &key) {
    if (Empty()){
        std::cerr<<"Empty tree\n";
    }
    else{
        Remove(root, key);
        size--;
    }
}
template<typename K, typename V>
void Treemap<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
    if (!n){
        std::cerr<<"Invalid Key\n";
    }
    if (key < n->key){
        Remove(n->left, key);
    }
    else if (key > n->key){
        Remove(n->right, key);
    }
    else{
        if (n->left && n->right){
            n->key = MinKey();
            Remove(n->right, key);
        }
        else{
            n = std::move((n->left) ? n->left : n->right);
        }
    }
}
template<typename K, typename V>
const V& Treemap<K, V>::Get(const K &key) {
    if (Empty()){
        std::cerr<<"Empty tree\n";
    }
    else{
        Get(root, key);
    }
}

template<typename K, typename V>
const V& Treemap<K, V>::Get(std::unique_ptr<Node> &n, const K &key) {
    if (!n){
        std::cerr<<"Invalid key\n";
    }
    Get(n->left, key);
    if (n->key == key){
        return n->value;
    }
    Get(n->right, key);
}

template <typename K, typename V>
const K& Treemap<K,V>::FloorKey(const K &key){

    Node *n = root.get();
    while (n) {
        //when key is found, if left branch exists, search for max in left branch
        if (key == n->key){
            if(n->right) {
                n = n->left.get();
                while (n->right) {
                    n = n->right.get();
                }
                return n->key;
            }else{
                throw std::out_of_range("No floor!\n");
            }
        }
        if (key < n->key){
            n = n->left.get();
        }
        else {
            n = n->right.get();
        }
    }

    throw std::out_of_range("Key not found!\n");
}

template <typename K, typename V>
const K& Treemap<K,V>::CeilKey(const K &key) {
    Node *n = root.get();
    while (n) {
        //when key is found, if right branch exists, search for min in right branch
        if (key == n->key){
            if(n->right) {
                return MinKey();
            } else{
                throw std::out_of_range("No ceiling!\n");
            }
        }
        if (key < n->key){
            n = n->left.get();
        }
        else {
            n = n->right.get();
        }
    }
    throw std::out_of_range("Key not found!\n");
}

template<typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K &key) {
    Node *n = root.get();
    while(n){
        if (key == n->key){
            return true;
        }
        if(key < n->key){
            n = n->left.get();
        }
        else{
            n = n->right.get();
        }
    }

    return false;
}

template<typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V &value) {
    ContainsValue(root, value);
}

template<typename K, typename V>
bool Treemap<K, V>::ContainsValue(std::unique_ptr<Node> &n, const V &value) {
    if (!n){
        return false;
    }
    ContainsValue(n->left, value);
    if (n->value == value){
        return true;
    }
    ContainsValue(n->right, value);
}

template<typename K, typename V>
const K &Treemap<K, V>::MaxKey() {
    Node *n = root.get();
    while (n->right)
        n = n->right.get();return n->key;
}

template<typename K, typename V>
const K &Treemap<K, V>::MinKey() {
    Node *n = root.get();
    while (n->left)
        n = n->left.get();
    return n->key;
}


template <typename K, typename V>
void Treemap<K, V>::Print() {
    Print(root.get(), 1);
    std::cout << std::endl;
}
template <typename K, typename V>
void Treemap<K, V>::Print(Node *n, int level) {
    if (!n) return;
    Print(n->left.get(), level + 1);
    std::cout << n->key
              << " [" << n->value << "] ";
    Print(n->right.get(), level + 1);
}
// ...To be completed...

#endif  // TREEMAP_H_