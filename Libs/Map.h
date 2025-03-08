// #pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Node.h"
#include <vector>

using namespace std;

template<typename T>
class Map {
    private:
        int size;
        vector<Node<T>*> dataMap;

    public:
        /*
            Constructor:
            Initialize the size of the map and create a vector of Node pointers
        */
        Map(int size): size(size) {
            dataMap = vector<Node<T>*>(size, NULL);
        }
        /*
            Destructor:
            Delete all the nodes in the map
        */
        ~Map() {
            for (int i = 0; i < size; i++) {
                Node<T>* current = dataMap[i];
                while (current != nullptr) {
                    Node<T>* temp = current;
                    current = current->next();
                    delete temp;
                }
            }
        }

        /*
            Param: int key, T value
            Put the value to the map with the key
        */
        void put(int key, T value) {
            int hash = key % size;

            Node<T>* newNode = new Node<T>(value, key);

            if (dataMap[hash] == NULL) {
                dataMap[hash] = newNode;
            }
            else {
                Node<T>* current = dataMap[hash];

                while (current->next() != NULL) {
                    current = current->next();
                }

                current->setNext(newNode);
            }
        }

        /*
            Param: int key
            Return the value of the given key
            if the key is not found, return NULL
        */
        T* get(int key) {
            int hash = key % size;

            Node<T>* current = dataMap[hash];

            while (current != NULL) {
                if (current->key() == key) {
                    return current->data();
                }

                current = current->next();
            }

            return NULL;
        }

        /*
            Param: int key
            Remove the value of the given key
        */
        void remove(int key) {
            int hash = key % size;

            Node<T>* current = dataMap[hash];
            Node<T>* previous = NULL;

            while (current != NULL) {
                if (current->key() == key) {
                    if (previous == NULL) {
                        dataMap[hash] = current->next();
                    }
                    else {
                        previous->setNext(current->next());
                    }

                    delete current;
                    return;
                }

                previous = current;
                current = current->next();
            }
        }
};

#endif
