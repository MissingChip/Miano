#pragma once

#include <cstdio>
#include <vector>

template<class T> List<T>::List()
{
    first = nullptr;
    last = nullptr;
    current = nullptr;
    current_i = 0;
    length = 0;
    wait = 0;
    threshold = 16;
}

template<class T> List<T>::List(T t)
{
    first = new Node(t);
    last = first;
    current = first;
    length = 1;
    wait = 0;
    threshold = 16;
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
    else{
        current_i++;
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
template<class T> T& List<T>::operator[](int i)
{
    if(i >= current_i){
        wait += min(threshold/2, i-current_i);
    }else{
        wait -= min(threshold/2, i);
    }
    if(abs(wait)>threshold){
        return seek(i);
        wait = 0;
    }
    return get(i);
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
        return;
    }
    if(l == length){
        push_back(t);
        return;
    }
    if(i<=current_i){
        current_i++;
    }
    length++;
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

template<class T> std::vector<T> List<T>::to_vector()
{
    int b = current_i;
    std::vector<T> out;
    for(int i=0;i<length;i++){
        out.push_back(seek(i));
    }
    seek(b);
    return out;
}
template<class T> string List<T>::to_string()
{
    int b = current_i;
    string out = "[";
    if(length>0){
        for(int i=0;i<length-1;i++){
            out += std::to_string(seek(i));
            out += ", ";
        }
        out += std::to_string(seek(length-1));
    }
    out += "]";
    seek(b);
    return out;
}

template<class T> string to_string(List<T> l){
    string out = "[";
    if(l.size()>0){
        for(int i=0;i<l.size()-1;i++){
            out += std::to_string(l[i]);
            out += ", ";
        }
        out += std::to_string(l[l.size()-1]);
    }
    out += "]";
    return out;
}
template<class T> string to_string(vector<T> v){
    string out = "[";
    if(v.size()>0){
        for(int i=0;i<v.size()-1;i++){
            out += std::to_string(v[i]);
            out += ", ";
        }
        out += std::to_string(v[v.size()-1]);
    }
    out += "]";
    return out;
}
