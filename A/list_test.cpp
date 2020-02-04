#include <stdio.h>
#include "list.h"


int main(void){
    List<int> l;
    l.push_back(3);
    printf("pushed back 3\n");
    printf("pop_back %d\n", l.pop_back());
    l.push_front(4);
    printf("pushed front 4\n");
    printf("pop_front %d\n", l.pop_front());
    l.push_back(6);
    printf("push 6");
    l.push_back(4);
    printf(" 4");
    l.push_back(18);
    printf(" 18\n");
    printf("get 0,1 %d,%d\n", l.get(0), l.get(1));
    printf("seek(0), next %d ", l.seek(0));
    printf("%d\n", l.next());
    l[2] = 7;
    printf("seek(0), %d\n", l.seek(0));
    printf("l[0], %d\n", l[0]);
    l.next() = 17;
    printf("seek(1), %d\n", l.seek(1));
    printf("l[0], %d\n", l[0]);
    printf("l[1], %d\n", l[1]);
    printf("l[2], %d\n", l[2]);
    l.insert(0, 19);
    l.insert(2, 20);
    l.insert(4, 21);
    printf("l[0], %d\n", l[0]);
    printf("l[1], %d\n", l[1]);
    printf("l[2], %d\n", l[2]);
    printf("l[3], %d\n", l[3]);
    printf("l[4], %d\n", l[4]);
}
