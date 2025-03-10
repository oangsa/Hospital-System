#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Node.h"
#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class Map {
    private:
        int _size;
        vector<Node<T>*> dataMap;

    public:

        /*
            Constructor:
            Initialize the size of the map and create a vector of Node pointers
        */
        Map(int size): _size(size) {
            dataMap = vector<Node<T>*>(size, nullptr);
        }

        /*
            Destructor:
            Delete all the nodes in the map
        */
        ~Map() {
            for (int i = 0; i < _size; i++) {
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
            int hash = key % _size;

            Node<T>* newNode = new Node<T>(value, key);

            if (dataMap[hash] == nullptr) {
                dataMap[hash] = newNode;
            }
            else {
                Node<T>* current = dataMap[hash];

                while (current->next() != nullptr) {
                    current = current->next();
                }

                current->setNext(newNode);
            }
        }

        /*
            Param: int key
            Return the value of the given key
            if the key is not found, return nullptr
        */
        T* get(int key) {
            int hash = key % _size;

            Node<T>* current = dataMap[hash];

            while (current != nullptr) {
                if (current->key() == key) {
                    return current->data();
                }

                current = current->next();
            }

            return nullptr;
        }

        /*
            Param: int key
            Remove the value of the given key
        */
        void remove(int key) {
            int hash = key % _size;

            Node<T>* current = dataMap[hash];
            Node<T>* previous = nullptr;

            while (current != nullptr) {
                if (current->key() == key) {
                    if (previous == nullptr) {
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

        /*
            Return the size of the map
        */
        int size() const {
            return _size;
        }

        /*
            Param: string& fileName
            Write the map to a file
            AKA: Save the map to a file
        */
        void writeToFile(string& fileName) {
            // Implement file writing logic here
            cout << "This method is not implemented yet." << "\n";
        }
};

#endif
