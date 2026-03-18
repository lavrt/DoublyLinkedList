#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "list.h"

int main()
{
    dblLinkedList list = {};
    ctor(&list);

    for (size_t i = 10; i <= 50; i += 10) {
        pushFront(&list, i);
    }

    dump(&list);

    dtor(&list);

    return 0;
}
