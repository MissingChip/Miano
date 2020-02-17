#pragma once

#include <vector>
#include <string>

using namespace std;

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
    
    T* to_array();
    void to_array(T*);
    vector<T> to_vector();
    string to_string();
//private:
    class Node{
    public:
        T in;
        Node* next;
        Node(T);
    };
    int length;
    Node* first;
    int current_i;
    int wait;
    int threshold;
    Node* current;
    Node* last;
};

#if __has_include("list.hpp")
#include "list.hpp"
#endif
#if __has_include("List/list.hpp")
#include "List/list.hpp"
#endif
