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

int main()
{
    linkedList tmp = {};

    ctor(&tmp);

     pushBack(&tmp, 10); pushBack(&tmp, 20); pushBack(&tmp, 30); pushBack(&tmp, 40); pushBack(&tmp, 50);
    // pushBeforeNth(&tmp, 1, 60); pushAfterNth(&tmp, 5, 52);
    pushFront(&tmp, 5); pushFront(&tmp, 2); pushFront(&tmp, 52);

    printf("HEAD = %lu\nTAIL = %lu\nCOUNTER = %lu\n", tmp.head, tmp.tail, tmp.counter);
    for (int i = 1; i < SIZE; i++) printf("%5d", i);
    printf("\n\n");
    for (int i = 1; i < SIZE; i++) printf("%5d", tmp.data[i]);
    printf("\n");
    for (int i = 1; i < SIZE; i++) printf("%5d", tmp.next[i]);
    printf("\n");

    return 0;
}

void ctor(linkedList* tmp)
{
    tmp->head = 0;
    tmp->tail = 0;
    tmp->free = 1;
    tmp->next[0] = 0;

    for (int i = 0; i < SIZE; i++) { tmp->next[i] = POISON; }
}

void pushFront(linkedList* tmp, int value)
{
    assert(tmp);
    assert(tmp->counter + 1 < SIZE);

    tmp->data[tmp->free] = value;
    tmp->next[tmp->free] = tmp->head;
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
        tmp->next[tmp->free] = index;
        tmp->head = tmp->free;
        tmp->counter++;
        tmp->free++;
        return;
    }
    tmp->next[tmp->free] = tmp->next[index - 1];
    tmp->next[index - 1] = tmp->free;
    tmp->counter++;
    tmp->free++;
}

