#pragma once

template<class T>
class List{
public:
    List();
    List(T);
    void push_front(T);
    void push_back(T);
    T pop_front();
    T pop_back();
    T& get(int i);
    T& operator[](int i);
    T& seek(int i);
    void insert(T t);
    void insert(int i, T t);
    T& next();
    T& recent();
    int recent_idx();
    int size();
private:
    class Node{
    public:
        T in;
        Node* next;
        Node(T);
    };
    int length;
    Node* first;
    int current_i;
    Node* current;
    Node* last;
};

#include "list.hpp"
