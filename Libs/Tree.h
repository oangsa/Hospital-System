#pragma once
#ifndef TREE
#define TREE

#include <iostream>
#include "Node.h"
#include "Define.h"

template<typename T>
class Tree {
    private:
        T* _data;
        u_int64 _key;
        Tree<T>* _left;
        Tree<T>* _right;

    public:
        /*
            Constructor of the class
            Params: T* data, u_int64 key
        */
        Tree(T* data, u_int64 key) {
            this->_data = data;
            this->_key = key;
            this->_left = nullptr;
            this->_right = nullptr;
        }
        
        /*
            Alternate Constructor of the class
            Params: T* data, u_int64 key, Tree<T*>* left, Tree<T*>* right
        */
        Tree(T* data, u_int64 key, Tree<T*>* left, Tree<T*>* right) {
            this->_data = data;
            this->_key = key;
            this->_left = left;
            this->_right = right;
        }
        
        /*
            Find data in the BST by given key
            Params: u_int64 key
            return pointer of data if the given key exist, else NULL
        */
        T* find(u_int64 key) {
            Tree<T>* current = this;

            while (current && current->_key != key) {
                if (key < current->_key) {
                    current = current->_left;
                }
                else {
                    current = current->_right;
                }
            }

            return current->_data;
        }
        
        /*
            Insert the node in BST
            Params: u_int64 key, T* data
        */
        void insert(u_int64 key, T* data) {
            Tree<T>* current = this;

            while (current) {
                if (key < current->_key) {
                    if (!current->_left) {
                        current->_left = new Tree(data, key);
                        return;
                    }
                    current = current->_left;
                }
                else {
                    if (!current->_right) {
                        current->_right = new Tree(data, key);
                        return;
                    }
                    current = current->_right;
                }
            }

            current = new Tree(data, key);
        }

        /*
            Performs an in-order traversal of the binary tree.

            Params: callback A function that takes a reference to a `T` type
                object (the data stored in the node) and returns void.
                This function will be called for each node's data in in-order sequence.
        */
        void inOrder(void (*callback)(T& data)) {
            if (_left) {
                _left->inOrder(callback);
            }

            callback(*_data);

            if (_right) {
                _right->inOrder(callback);
            }
        }
};

#endif
