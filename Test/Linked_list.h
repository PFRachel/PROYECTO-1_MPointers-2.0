//
// Created by Rachel on 09/03/2025.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../Server/Mpointers_lib/MPointer.h"
template <typename T>// plantilla de tipos de datos
class Node {
    public:
    MPointer<T> data;
    MPointer<Node<T>> next;
    Node(T value);
};
template <typename T>// plantilla de tipos de datos
class Linked_list {
private:
    MPointer<Node<T>> head;
public:
    Linked_list();
    void insert(T value);
    void display()const;
};
//prevenir errores
template<typename T>
Node<T>::Node(T value) {
    data = MPointer<T>::New();
    *data = value;
    next = MPointer<Node<T>>::New();
    *next = nullptr;
}
template<typename T>
Linked_list<T>::Linked_list() {
    head = MPointer<Node<T>>::New();
    *head = nullptr;

}


#endif //LINKED_LIST_H
