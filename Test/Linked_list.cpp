//
// Created by Rachel on 09/03/2025.
//

#include "Linked_list.h"
#include <iostream>
//hacemos el nodo
template<typename T>
void ::Linked_list<T>::insert(T value) {
    //se crea un nuevo nodo
    MPointer<Node<T>> newNodo = MPointer<Node<T>>::New();
    *newNodo = Node<T>(value);
    if (*head == nullptr) {
        head = newNodo;
        return;
    }
    MPointer<Node<T>> current = *head;
    while (*(*current).next != nullptr) {
        current = *(*current).next;
    }
    *(*current).next = *newNodo;
}

//lista  cabeza
template<typename T>
void Linked_list<T>::display() const {
    MPointer<Node<T>> current = head;
    while (*current != nullptr) {
        std::cout << *(*current).data << " ";
        current= *(*current).next;
    }
    std::cout << std::endl;
}

// Explicit instantiation
template class Linked_list<int>;
template class Node<int>;
template class Linked_list<std::string>;
template class Node<std::string>;