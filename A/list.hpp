#include <cstdio>

template<class T> List<T>::List()
{
    first = nullptr;
    last = nullptr;
    current = nullptr;
    current_i = 0;
    length = 0;
}

template<class T> List<T>::List(T t)
{
    first = new Node(t);
    last = first;
    current = first;
    length = 1;
}

template<class T> List<T>::Node::Node(T t)
{
    in = t;
}

template<class T> void List<T>::push_back(T t)
{
    if(length == 0){
        first = new Node(t);
        last = first;
        current = first;
        length++;
        return;
    }
    last->next = new Node(t);
    last = last->next;
    length++;
}

template<class T> void List<T>::push_front(T t)
{
    Node* f = first;
    first = new Node(t);
    first->next = f;
    if(length == 0){
        current = first;
        last = first;
    }
    length++;
}

template<class T> T& List<T>::get(int i)
{
    int l = 0;
    Node* n = first;
    if(i>=current_i){
        l = current_i;
        n = current;
    }
    for(;l<i;l++){
        n = n->next;
    }
    return n->in;
}
template<class T> T& List<T>::operator[](int i)
{
    return get(i);
}
template<class T> T& List<T>::seek(int i)
{
    int l=0;
    Node* n = first;
    if(i>=current_i){
        l = current_i;
        n = current;
    }
    for(;l<i;l++){
        n = n->next;
    }
    current = n;
    current_i = i;
    return n->in;
}
template<class T> void List<T>::insert(T t)
{
    insert(current_i, t);
}
template<class T> void List<T>::insert(int i, T t)
{
    int l=0;
    Node* n = first;
    if(i>current_i){
        l = current_i;
        n = current;
    }
    for(;l<i-1;l++){
        n = n->next;
    }
    if(l == 0){
        push_front(t);
        //return;
    }
    else{
        length++;
    }
    Node* next = n->next;
    n->next = new Node(t);
    n->next->next = next;
}
template<class T> T& List<T>::next()
{
    T* c = &(current->next->in);
    current = current->next;
    current_i++;
    return *c;
}
template<class T> T& List<T>::recent()
{
    return current->in;
}
template<class T> int List<T>::recent_idx()
{
    return current_i;
}

template<class T> T List<T>::pop_back()
{
    int l;
    T t;
    Node* n;
    if(length == 1){
        delete first;
        t = first->in;
        n = first = nullptr;
    }
    else{
        n = first;
        for(l=0;l<length-1;l++){
            n = n->next;
        }
        t = n->in;
        delete n->next;
    }
    last = n;
    length--;
    return t;
}
template<class T> T List<T>::pop_front()
{
    int l;
    T t = first->in;
    Node* f = first->next;
    delete first;
    first = f;
    length--;
    return t;
}

template<class T> int List<T>::size()
{
    return length;
}
