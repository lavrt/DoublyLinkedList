#include <stdio.h>
#include <assert.h>
#include <string.h>

const size_t kSize = 10;
const int POISON = -333;
const int ptrPOISON = -1;
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

enum ErrorCodes
{
    Success                = 0,
    ListOverflow           = 1,
    ListUnderflow          = 2,
    OutOfBoundsListAccess  = 3,
    DumpFileOpenningError  = 4,
    NullPointerDereference = 5,
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
int getNth(const dblLinkedList* const list, const size_t index);
int search(const dblLinkedList* const list, int value);

void deleteNth(dblLinkedList* const list, const size_t index);
void clear(dblLinkedList* const list);

int main()
{
    dblLinkedList list = {};

    ctor(&list);

    pushFront(&list, 10); pushFront(&list, 20); pushFront(&list, 30); pushFront(&list, 40); pushFront(&list, 50);
    // pushBeforeNth(&list, 1, 60); pushAfterNth(&list, 5, 52);
    // pushFront(&list, 5); pushFront(&list, 2); pushFront(&list, 52);
    // pushBack(&list, 10); pushBack(&list, 20); pushBack(&list, 30); pushBack(&list, 40);
    // pushBack(&list, 5); pushBack(&list, 2); pushBack(&list, 52);
    pushAfterNth(&list, 1, 52); pushAfterNth(&list, 1, 53);
    // pushAfterNth(&list, 1, 152);
    // pushBeforeNth(&list, 1, 52); pushBeforeNth(&list, 1, 53);
    // pushBeforeNth(&list, 1, 777);
    deleteNth(&list, 3);
    pushBack(&list, 20);

    dump(&list);

    dtor(&list);

    return 0;
}

void ctor(dblLinkedList* const list)
{
    assert(list);

    list->free = 1;
    list->counter = 0;
    list->data[0] = POISON;

    for (size_t i = 1; i < kSize; i++)
    {
        list->data[i] = POISON;
        list->next[i] = (int)i + 1;
        list->prev[i] = ptrPOISON;
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

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = list->next[0];
    list->prev[list->next[0]] = list->free;
    list->next[0] = list->free;
    list->prev[list->free] = 0;

    list->free = free;
}

void pushBack(dblLinkedList* const list, const int value)
{
    assert(list);
    assert(++list->counter < kSize);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->prev[0]] = list->free;
    list->prev[list->free] = list->prev[0];
    list->next[list->free] = 0;
    list->prev[0] = list->free;

    list->free = free;
}

void pushAfterNth(dblLinkedList* const list, const size_t index, const int value)
{
    assert(list);
    assert(++list->counter < kSize);
    assert(list->prev[index] != ptrPOISON);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = list->next[index];
    list->prev[list->next[index]] = list->free;
    list->next[index] = list->free;
    list->prev[list->free] = (int)index;

    list->free = free;
}

void pushBeforeNth(dblLinkedList* const list, const size_t index, const int value)
{
    assert(list);
    assert(++list->counter < kSize);
    assert(list->prev[index] != ptrPOISON);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = (int)index;
    list->prev[list->free] = list->prev[index];
    list->next[list->prev[index]] = list->free;
    list->prev[index] = list->free;

    list->free = free;
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
    assert(list->prev[index] != ptrPOISON);

    return list->next[index];
}

int prev(const dblLinkedList* const list, const size_t index)
{
    assert(list);
    assert(list->prev[index] != ptrPOISON);

    return list->prev[index];
}

int getNth(const dblLinkedList* const list, const size_t index)
{
    assert(list);
    assert(list->prev[index] != ptrPOISON);

    return list->data[index];
}

int search(const dblLinkedList* const list, int value)
{
    assert(list);

    for (int i = list->next[0]; i != list->prev[0]; i = list->next[i])
    {
        if (list->data[i] == value)
        {
            return i;
        }
    }

    return -1;
}

void deleteNth(dblLinkedList* const list, const size_t index) // FIXME
{
    assert(list);
    assert(list->counter--);
    assert(list->prev[index] != ptrPOISON);

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    list->next[index] = ptrPOISON;
    list->prev[index] = ptrPOISON;

    list->next[index] = list->free;
    list->free = index;
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
