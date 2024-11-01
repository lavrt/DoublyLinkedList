#include <stdio.h>
#include <assert.h>

int const SIZE = 10;
int const POISON = -1;
const char* const kDumpFileName = "dump.gv";

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while(0)

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
void dump(linkedList* tmp);

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

    dump(&tmp);

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

void dump(linkedList* tmp)
{
    FILE* dumpFile = fopen(kDumpFileName, "wb");
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n");
    fprintf(dumpFile, "{\n    ");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    for (int i = 0; i < SIZE; i++)
    {
        fprintf(dumpFile, "    node_%d [shape=record,label=\" ip: %d | data: %d | next: %d | prev: %d \"];\n",
                i, i, tmp->data[i], tmp->next[i], tmp->prev[i]);
    }

    fprintf(dumpFile, "\n    edge [ style = invis, weight = 100 ];\n    ");
    for (int i = 0; i < SIZE - 1; i++)
    {
        fprintf(dumpFile, "node_%d -> ", i);
    }
    fprintf(dumpFile, "node_%d;\n", SIZE - 1);

    fprintf(dumpFile, "\n    edge [ color = green, style = filled, weight = 99, headport = n, tailport = n ];\n    ");

    FCLOSE(dumpFile);
}

// digraph
// {
//   rankdir=LR;
//   node_1 [shape=record,label=" ip: 1 | data: 10 | next: -1 | prev: -1"];
//   node_2 [shape=record,label=" ip: 2 | data: 5 | next: 3 | prev: 0"];
//   node_3 [shape=record,label=" ip: 3 | data: 2 | next: 4 | prev: 2"];
//   node_4 [shape=record,label=" ip: 4 | data: 52 | next: 0 | prev: 3"];
//   node_1 -> node_2 -> node_3 -> node_4;
// }


// digraph
// {
//     rankdir = LR;
//
//     node_0 [shape=record,label=" ip: 0 | data: 0 | next: 2 | prev: -1 "];
//     node_1 [shape=record,label=" ip: 1 | data: 10 | next: -1 | prev: -1 "];
//     node_2 [shape=record,label=" ip: 2 | data: 5 | next: 3 | prev: 0 "];
//     node_3 [shape=record,label=" ip: 3 | data: 2 | next: 4 | prev: 2 "];
//     node_4 [shape=record,label=" ip: 4 | data: 52 | next: 0 | prev: 3 "];
//     node_5 [shape=record,label=" ip: 5 | data: 0 | next: -1 | prev: -1 "];
//     node_6 [shape=record,label=" ip: 6 | data: 0 | next: -1 | prev: -1 "];
//     node_7 [shape=record,label=" ip: 7 | data: 0 | next: -1 | prev: -1 "];
//     node_8 [shape=record,label=" ip: 8 | data: 0 | next: -1 | prev: -1 "];
//     node_9 [shape=record,label=" ip: 9 | data: 0 | next: -1 | prev: -1 "];
//
//     edge [ style = invis, weight = 100 ];
//     node_0 -> node_1 -> node_2 -> node_3 -> node_4 -> node_5 -> node_6 -> node_7 -> node_8 -> node_9;
//
//     edge [ color = green, style = filled, weight = 99, headport = n, tailport = n ];
//     node_0 -> node_2;
//     node_2 -> node_3;
//     node_3 -> node_4;
//
//     edge [ color = red, style = filled, weight = 0, headport = s, tailport = s ];
//     node_2 -> node_0;
//     node_3 -> node_2;
//     node_4 -> node_3;
// }
