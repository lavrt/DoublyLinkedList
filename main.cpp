#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "list.h"

int main()
{
    dblLinkedList list = {};

    ctor(&list);

    pushFront(&list, 10); pushFront(&list, 20); pushFront(&list, 30); pushFront(&list, 40); pushFront(&list, 50);
    // pushBeforeNth(&list, 1, 60); pushAfterNth(&list, 5, 52);
    // pushFront(&list, 5); pushFront(&list, 2); pushFront(&list, 52);
    // pushBack(&list, 10); pushBack(&list, 20); pushBack(&list, 30); pushBack(&list, 40);
    // pushBack(&list, 5); pushBack(&list, 2); pushBack(&list, 52);
    // pushAfterNth(&list, 1, 52); pushAfterNth(&list, 1, 53);
    // pushAfterNth(&list, 1, 152);
    // pushBeforeNth(&list, 1, 52); pushBeforeNth(&list, 1, 53);
    // pushBeforeNth(&list, 1, 777);
    // deleteNth(&list, 3);
    // pushBack(&list, 20);

    dump(&list);

    dtor(&list);

    return 0;
}

