#include <stdio.h>
#include <assert.h>
#include <string.h>

const size_t kSize = 10;
const int POISON_1 = -333;
const int POISON_2 = -1;
const char* const kDumpFileName = "dump.gv";

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while(0)

struct dblLinkedList
{
    int data[kSize];
    int next[kSize];
    int prev[kSize];

    int free;
    size_t counter;
};

void ctor(dblLinkedList* const list);
void dtor(dblLinkedList* const list);
void dump(const dblLinkedList* const list);
void pushFront(dblLinkedList* const list, const int value);
void pushBack (dblLinkedList* const list, const int value);
void pushBeforeNth(dblLinkedList* const list, const size_t index, const int value);
void pushAfterNth (dblLinkedList* const list, const size_t index, const int value);

int front(const dblLinkedList* const list);
int back(const dblLinkedList* const list);
int next(const dblLinkedList* const list, const size_t index);
int prev(const dblLinkedList* const list, const size_t index);

void deleteNth(dblLinkedList* const list, const size_t index);
void clear(dblLinkedList* const list);

int main()
{
    dblLinkedList list = {};

    ctor(&list);

    // pushFront(&list, 10); pushFront(&list, 20); pushFront(&list, 30); pushFront(&list, 40); pushFront(&list, 50);
    // pushBeforeNth(&list, 1, 60); pushAfterNth(&list, 5, 52);
    // pushFront(&list, 5); pushFront(&list, 2); pushFront(&list, 52);
    // pushBack(&list, 10); pushBack(&list, 20); pushBack(&list, 30); pushBack(&list, 40);
    // pushBack(&list, 5); pushBack(&list, 2); pushBack(&list, 52);
    // pushAfterNth(&list, 1, 52); pushAfterNth(&list, 1, 53);
    // pushAfterNth(&list, 1, 152);
    // pushBeforeNth(&list, 1, 52); pushBeforeNth(&list, 1, 53);
    pushBeforeNth(&list, 1, 777);
    // deleteNth(&list, 5);

    dump(&list);

    dtor(&list);

    return 0;
}

void ctor(dblLinkedList* const list)
{
    assert(list);

    list->free = 1;
    list->counter = 0;
    list->data[0] = POISON_1;

    for (size_t i = 1; i < kSize; i++)
    {
        list->data[i] = POISON_1;
        list->next[i] = i + 1;
        list->prev[i] = POISON_2;
    }
    list->next[kSize - 1] = 0;
}

void dtor(dblLinkedList* const list)
{
    assert(list);

    memset(list->data, 0, kSize * sizeof(int));
    memset(list->next, 0, kSize * sizeof(int));
    memset(list->prev, 0, kSize * sizeof(int));

    list->free = 0;
    list->counter = 0;
}

void pushFront(dblLinkedList* const list, const int value)
{
    assert(list);
    assert(++list->counter < kSize);

    list->data[list->free] = value;
    list->next[list->free] = list->next[0];
    list->prev[list->next[0]] = list->free;
    list->next[0] = list->free;
    list->prev[list->free] = 0;

    list->free++;
}

void pushBack(dblLinkedList* const list, const int value)
{
    assert(list);
    assert(++list->counter < kSize);

    list->data[list->free] = value;
    list->next[list->prev[0]] = list->free;
    list->prev[list->free] = list->prev[0];
    list->next[list->free] = 0;
    list->prev[0] = list->free;

    list->free++;
}

void pushAfterNth(dblLinkedList* const list, const size_t index, const int value)
{
    assert(list);
    assert(++list->counter < kSize);
    assert(list->prev[index] != POISON_2);

    list->data[list->free] = value;
    list->next[list->free] = list->next[index];
    list->prev[list->next[index]] = list->free;
    list->next[index] = list->free;
    list->prev[list->free] = (int)index;

    list->free++;
}

void pushBeforeNth(dblLinkedList* const list, const size_t index, const int value)
{
    assert(list);
    assert(++list->counter < kSize);
    assert(list->prev[index] != POISON_2);

    list->data[list->free] = value;
    list->next[list->free] = (int)index;
    list->prev[list->free] = list->prev[index];
    list->next[list->prev[index]] = list->free;
    list->prev[index] = list->free;

    list->free++;
}

int front(const dblLinkedList* const list)
{
    assert(list);

    return list->next[0];
}

int back(const dblLinkedList* const list)
{
    assert(list);

    return list->prev[0];
}

int next(const dblLinkedList* const list, const size_t index)
{
    assert(list);

    return list->next[index];
}

int prev(const dblLinkedList* const list, const size_t index)
{
    assert(list);

    return list->prev[index];
}

void deleteNth(dblLinkedList* const list, const size_t index)
{
    assert(list);
    assert(list->counter--);

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    list->next[index] = POISON_2;
    list->prev[index] = POISON_2;
}

void clear(dblLinkedList* const list)
{

}

void dump(const dblLinkedList* const list)
{
    assert(list);

    FILE* dumpFile = fopen(kDumpFileName, "wb");
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n");
    fprintf(dumpFile, "{\n    ");
    fprintf(dumpFile, "rankdir = LR;\n    ");
    fprintf(dumpFile, "node [ style = rounded, color = \"#252A34\", penwidth = 2.5 ];\n    ");
    fprintf(dumpFile, "bgcolor = \"#E7ECEF\";\n\n");

    for (size_t i = 0; i < kSize; i++)
    {
        fprintf(dumpFile, "    node_%lu [shape=record,label=\" ip: %lu | data: %d | next: %d | prev: %d \"];\n",
                i, i, list->data[i], list->next[i], list->prev[i]);
    }

    fprintf(dumpFile, "\n    edge [ style = invis, weight = 100 ];\n    ");
    for (size_t i = 0; i < kSize - 1; i++)
    {
        fprintf(dumpFile, "node_%lu -> ", i);
    }
    fprintf(dumpFile, "node_%lu;\n", kSize - 1);

    fprintf(dumpFile, "\n    edge [ color = \"#048A81\", style = filled, "
                      "penwidth = 2, weight = 0, headport = n, tailport = n ];\n    ");
    for (int i = list->next[0]; i != list->prev[0]; i = list->next[i])
    {
        fprintf(dumpFile, "node_%d -> node_%d; ", i, list->next[i]);
    }

    fprintf(dumpFile, "\n\n    edge [ color = \"#FF2E63\", style = filled, "
                      "penwidth = 2, weight = 0, headport = s, tailport = s ];\n    ");
    for (int i = list->prev[0]; i != list->next[0]; i = list->prev[i])
    {
        fprintf(dumpFile, "node_%d -> node_%d; ", i, list->prev[i]);
    }
    fprintf(dumpFile, "\n}\n");

    FCLOSE(dumpFile);
}
