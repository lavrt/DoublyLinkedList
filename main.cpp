#include <stdio.h>
#include <assert.h>
#include <string.h>

const size_t kSize = 10;
const int POISON_1 = -333;
const int POISON_2 = -1;
const char* const kDumpFileName = "dump.gv";

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while(0)

struct linkedList
{
    int data[kSize];
    int next[kSize];
    int prev[kSize];

    int free;
    size_t counter;
};

void ctor(linkedList* const tmp);
void dtor(linkedList* const tmp);
void dump(const linkedList* const tmp);
void pushFront(linkedList* const tmp, const int value);
void pushBack (linkedList* const tmp, const int value);
void pushBeforeNth(linkedList* const tmp, const size_t index, const int value);
void pushAfterNth (linkedList* const tmp, const size_t index, const int value);
void deleteNth(linkedList* const tmp, const size_t index);

int main()
{
    linkedList tmp = {};

    ctor(&tmp);

    // pushFront(&tmp, 10); pushFront(&tmp, 20); pushFront(&tmp, 30); pushFront(&tmp, 40); pushFront(&tmp, 50);
    // pushBeforeNth(&tmp, 1, 60); pushAfterNth(&tmp, 5, 52);
    // pushFront(&tmp, 5); pushFront(&tmp, 2); pushFront(&tmp, 52);
    pushBack(&tmp, 10); pushBack(&tmp, 20); pushBack(&tmp, 30); pushBack(&tmp, 40);
    // pushBack(&tmp, 5); pushBack(&tmp, 2); pushBack(&tmp, 52);
    // pushAfterNth(&tmp, 1, 52); pushAfterNth(&tmp, 1, 53);
    // pushAfterNth(&tmp, 1, 152);
    // pushBeforeNth(&tmp, 1, 52); pushBeforeNth(&tmp, 1, 53);
    pushBeforeNth(&tmp, 1, 777);
    // deleteNth(&tmp, 5);

    dump(&tmp);

    dtor(&tmp);

    return 0;
}

void ctor(linkedList* const tmp)
{
    assert(tmp);

    tmp->free = 1;
    tmp->counter = 0;
    tmp->data[0] = POISON_1;

    for (size_t i = 1; i < kSize; i++)
    {
        tmp->data[i] = POISON_1;
        tmp->next[i] = POISON_2;
        tmp->prev[i] = POISON_2;
    }
}

void dtor(linkedList* const tmp)
{
    assert(tmp);

    memset(tmp->data, 0, kSize * sizeof(int));
    memset(tmp->next, 0, kSize * sizeof(int));
    memset(tmp->prev, 0, kSize * sizeof(int));

    tmp->free = 0;
    tmp->counter = 0;
}

void pushFront(linkedList* const tmp, const int value)
{
    assert(tmp);
    assert(++tmp->counter < kSize);

    tmp->data[tmp->free] = value;
    tmp->next[tmp->free] = tmp->next[0];
    tmp->prev[tmp->next[0]] = tmp->free;
    tmp->next[0] = tmp->free;
    tmp->prev[tmp->free] = 0;

    tmp->free++;
}

void pushBack(linkedList* const tmp, const int value)
{
    assert(tmp);
    assert(++tmp->counter < kSize);

    tmp->data[tmp->free] = value;
    tmp->next[tmp->prev[0]] = tmp->free;
    tmp->prev[tmp->free] = tmp->prev[0];
    tmp->next[tmp->free] = 0;
    tmp->prev[0] = tmp->free;

    tmp->free++;
}

void pushAfterNth(linkedList* const tmp, const size_t index, const int value)
{
    assert(tmp);
    assert(index);
    assert(index <= tmp->counter);
    assert(++tmp->counter < kSize);

    tmp->data[tmp->free] = value;
    tmp->next[tmp->free] = tmp->next[index];
    tmp->prev[tmp->next[index]] = tmp->free;
    tmp->next[index] = tmp->free;
    tmp->prev[tmp->free] = (int)index;

    tmp->free++;
}

void pushBeforeNth(linkedList* const tmp, const size_t index, const int value)
{
    assert(tmp);
    assert(index);
    assert(index <= tmp->counter);
    assert(++tmp->counter < kSize);

    tmp->data[tmp->free] = value;
    tmp->next[tmp->free] = (int)index;
    tmp->prev[tmp->free] = tmp->prev[index];
    tmp->next[tmp->prev[index]] = tmp->free;
    tmp->prev[index] = tmp->free;

    tmp->free++;
}

void deleteNth(linkedList* const tmp, const size_t index)
{
    assert(tmp);
    assert(tmp->counter--);

    tmp->next[tmp->prev[index]] = tmp->next[index];
    tmp->prev[tmp->next[index]] = tmp->prev[index];

    tmp->next[index] = POISON_2;
    tmp->prev[index] = POISON_2;
}

void dump(const linkedList* const tmp)
{
    assert(tmp);

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
                i, i, tmp->data[i], tmp->next[i], tmp->prev[i]);
    }

    fprintf(dumpFile, "\n    edge [ style = invis, weight = 100 ];\n    ");
    for (size_t i = 0; i < kSize - 1; i++)
    {
        fprintf(dumpFile, "node_%lu -> ", i);
    }
    fprintf(dumpFile, "node_%lu;\n", kSize - 1);

    fprintf(dumpFile, "\n    edge [ color = \"#048A81\", style = filled, "
                      "penwidth = 2, weight = 0, headport = n, tailport = n ];\n    ");
    for (int i = tmp->next[0]; i != tmp->prev[0]; i = tmp->next[i])
    {
        fprintf(dumpFile, "node_%d -> node_%d; ", i, tmp->next[i]);
    }

    fprintf(dumpFile, "\n\n    edge [ color = \"#FF2E63\", style = filled, "
                      "penwidth = 2, weight = 0, headport = s, tailport = s ];\n    ");
    for (int i = tmp->prev[0]; i != tmp->next[0]; i = tmp->prev[i])
    {
        fprintf(dumpFile, "node_%d -> node_%d; ", i, tmp->prev[i]);
    }
    fprintf(dumpFile, "\n}\n");

    FCLOSE(dumpFile);
}
