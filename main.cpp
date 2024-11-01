#include <stdio.h>
#include <assert.h>

int const SIZE = 10;
int const POISON = -1;

struct linkedList
{
    int data[SIZE];
    int next[SIZE];
    int prev[SIZE];

    size_t head;
    size_t tail;
    size_t free;
    size_t counter;
};

void ctor(linkedList* tmp);
void pushFront(linkedList* tmp, int value);
void pushBack(linkedList* tmp, int value);
void pushAfterNth(linkedList* tmp, size_t index, int value);
void pushBeforeNth(linkedList* tmp, size_t index, int value);
void deleteNth(linkedList* tmp, size_t index);

int main()
{
    linkedList tmp = {};

    ctor(&tmp);

    // pushFront(&tmp, 10); pushFront(&tmp, 20); pushFront(&tmp, 30); pushFront(&tmp, 40); pushFront(&tmp, 50);
    // pushBeforeNth(&tmp, 1, 60); pushAfterNth(&tmp, 5, 52);
    // pushFront(&tmp, 5); pushFront(&tmp, 2); pushFront(&tmp, 52);
    pushBack(&tmp, 10);
    pushBack(&tmp, 5); pushBack(&tmp, 2); pushBack(&tmp, 52);
    // pushAfterNth(&tmp, 1, 52); pushAfterNth(&tmp, 1, 53); // pushAfterNth(&tmp, 1, 152);
    // pushBeforeNth(&tmp, 1, 52); pushBeforeNth(&tmp, 1, 53); pushBeforeNth(&tmp, 3, 777);
    deleteNth(&tmp, 1);

    printf("HEAD = %lu\nTAIL = %lu\nCOUNTER = %lu\n", tmp.head, tmp.tail, tmp.counter);
    for (int i = 1; i < SIZE; i++) printf("%5d", i);
    printf("\n\n");
    for (int i = 1; i < SIZE; i++) printf("%5d", tmp.data[i]);
    printf("\n");
    for (int i = 1; i < SIZE; i++) printf("%5d", tmp.next[i]);
    printf("\n");
    for (int i = 1; i < SIZE; i++) printf("%5d", tmp.prev[i]);
    printf("\n");

    return 0;
}

void ctor(linkedList* tmp)
{
    tmp->head = 0;
    tmp->tail = 0;
    tmp->free = 1;
    tmp->next[0] = 0;
    tmp->prev[0] = 0;

    for (int i = 0; i < SIZE; i++) { tmp->next[i] = POISON; }
    for (int i = 0; i < SIZE; i++) { tmp->prev[i] = POISON; }
}

void pushFront(linkedList* tmp, int value)
{
    assert(tmp);
    assert(tmp->counter + 1 < SIZE);

    tmp->data[tmp->free] = value;
    if (tmp->counter == 0)
    {
        tmp->tail = 1;
    }
    tmp->next[tmp->free] = tmp->head;
    tmp->prev[tmp->head] = tmp->free;
    tmp->prev[tmp->free] = 0;
    tmp->head = tmp->free;
    tmp->counter++;
    tmp->free++;
}

void pushBack(linkedList* tmp, int value)
{
    assert(tmp);
    assert(tmp->counter + 1 < SIZE);

    tmp->data[tmp->free] = value;
    if (tmp->counter == 0)
    {
        tmp->head = 1;
    }
    tmp->next[tmp->tail] = tmp->free;
    tmp->next[tmp->free] = 0;
    tmp->prev[tmp->free] = tmp->tail;
    tmp->counter++;
    tmp->tail++;
    tmp->free++;
}

void pushAfterNth(linkedList* tmp, size_t index, int value)
{
    assert(tmp);
    assert(index != 0);
    assert(tmp->counter + 1 < SIZE);
    assert(index <= tmp->counter);

    tmp->data[tmp->free] = value;
    if (index == tmp->tail)
    {
        tmp->tail = tmp->free;
    }
    tmp->next[tmp->free] = tmp->next[index];
    tmp->next[index] = tmp->free;
    tmp->prev[tmp->next[tmp->free]] = tmp->free;
    tmp->prev[tmp->free] = index;
    tmp->counter++;
    tmp->free++;
}

void pushBeforeNth(linkedList* tmp, size_t index, int value)
{
    assert(tmp);
    assert(index != 0);
    assert(tmp->counter + 1 < SIZE);
    assert(index <= tmp->counter);

    tmp->data[tmp->free] = value;
    if (index == 1)
    {
        tmp->next[tmp->free] = tmp->head;
        tmp->prev[tmp->head] = tmp->free;
        tmp->prev[tmp->free] = 0;
        tmp->head = tmp->free;
        tmp->counter++;
        tmp->free++;
        return;
    }
    tmp->next[tmp->free] = tmp->next[index - 1];
    tmp->next[index - 1] = tmp->free;
    tmp->prev[tmp->next[tmp->free]] = tmp->free;
    tmp->prev[tmp->free] = index;
    tmp->counter++;
    tmp->free++;
}

void deleteNth(linkedList* tmp, size_t index)
{
    assert(tmp);
    assert(tmp->counter != 0);

    if (index == tmp->head) { tmp->head = tmp->next[index]; }
    if (index == tmp->tail) { tmp->tail = tmp->prev[index]; }

    tmp->next[tmp->prev[index]] = tmp->next[index];
    tmp->prev[tmp->next[index]] = tmp->prev[index];
    tmp->next[index] = POISON;
    tmp->prev[index] = POISON;

    tmp->counter--;
}
