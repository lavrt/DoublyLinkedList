#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

// static --------------------------------------------------------------------------------------------------------------

static const int POISON = -333;
static const int ptrPOISON = -1;
static const char* const kDumpFileExtension = ".gv";
static const char* const kGraphExtension = ".png";
static const char* const kHtmlFileName = "dump.html";
static const char* const kGraphNameFile = "namesOfGraphs.txt";
static const size_t kSizeOfBuffer = 20;

static void dumpHtml(dblLinkedList* const list);
static ErrorCodes listError(const dblLinkedList* const list);
static ErrorCodes listAssertFunc(dblLinkedList* const list, const char * file, const int line, const char * const func);

// global --------------------------------------------------------------------------------------------------------------

ErrorCodes ctor(dblLinkedList* const list)
{
    LISTASSERT(list);

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

    list->error = Success;
    return Success;
}

ErrorCodes dtor(dblLinkedList* const list)
{
    assert(list);
    assert(list->data);
    assert(list->next);
    assert(list->prev);

    memset(list->data, 0, kSize * sizeof(int));
    memset(list->next, 0, kSize * sizeof(int));
    memset(list->prev, 0, kSize * sizeof(int));

    list->free = 0;
    list->counter = 0;

    return Success;
}

ErrorCodes pushFront(dblLinkedList* const list, const int value)
{
    LISTASSERT(list);
    list->counter++;
    LISTASSERT(list);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = list->next[0];
    list->prev[list->next[0]] = list->free;
    list->next[0] = list->free;
    list->prev[list->free] = 0;

    list->free = free;

    return Success;
}

ErrorCodes pushBack(dblLinkedList* const list, const int value)
{
    LISTASSERT(list);
    list->counter++;
    LISTASSERT(list);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->prev[0]] = list->free;
    list->prev[list->free] = list->prev[0];
    list->next[list->free] = 0;
    list->prev[0] = list->free;

    list->free = free;

    return Success;
}

ErrorCodes pushAfterNth(dblLinkedList* const list, const size_t index, const int value)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    list->counter++;
    LISTASSERT(list);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = list->next[index];
    list->prev[list->next[index]] = list->free;
    list->next[index] = list->free;
    list->prev[list->free] = (int)index;

    list->free = free;

    return Success;
}

ErrorCodes pushBeforeNth(dblLinkedList* const list, const size_t index, const int value)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    list->counter++;
    LISTASSERT(list);

    int free = list->next[list->free];
    list->data[list->free] = value;
    list->next[list->free] = (int)index;
    list->prev[list->free] = list->prev[index];
    list->next[list->prev[index]] = list->free;
    list->prev[index] = list->free;

    list->free = free;

    return Success;
}

int front(dblLinkedList* const list)
{
    LISTASSERT(list);

    return list->next[0];
}

int back(dblLinkedList* const list)
{
    LISTASSERT(list);

    return list->prev[0];
}

int next(dblLinkedList* const list, const size_t index)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    LISTASSERT(list);

    return list->next[index];
}

int prev(dblLinkedList* const list, const size_t index)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    LISTASSERT(list);

    return list->prev[index];
}

int getNth(dblLinkedList* const list, const size_t index)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    LISTASSERT(list);

    return list->data[index];
}

int search(dblLinkedList* const list, int value)
{
    LISTASSERT(list);

    for (int i = list->next[0]; i != list->prev[0]; i = list->next[i])
    {
        if (list->data[i] == value)
        {
            return i;
        }
    }

    return ElementNotFound;
}

ErrorCodes deleteNth(dblLinkedList* const list, const size_t index)
{
    LISTASSERT(list);
    if (list->prev[index] == ptrPOISON) { list->error = OutOfBoundsListAccess ; }
    list->counter--;
    LISTASSERT(list);

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    list->next[index] = ptrPOISON;
    list->prev[index] = ptrPOISON;

    list->next[index] = list->free;
    list->free = (int)index;

    return Success;
}

ErrorCodes dump(dblLinkedList* const list)
{
    assert(list);
    assert(list->data);
    assert(list->next);
    assert(list->prev);

    time_t currentTime = 0;
    time(&currentTime);
    struct tm* timeInfo = localtime(&currentTime);
    char timeBuffer[kSizeOfBuffer] = {};
    strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d%H%M%S", timeInfo);

    FILE* dumpFile = fopen(strcat(timeBuffer, kDumpFileExtension), "w");
    if (!dumpFile) { list->error = DumpFileOpenningError; assert(0); }

    FILE* namesOfGraphs = fopen(kGraphNameFile, "a");
    if (!namesOfGraphs) { list->error = DumpFileOpenningError; assert(0); }
    fprintf(namesOfGraphs, "%s\n", timeBuffer);
    FCLOSE(namesOfGraphs);

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

    dumpHtml(list);

    return Success;
}

// static --------------------------------------------------------------------------------------------------------------

static void dumpHtml(dblLinkedList* const list)
{
    FILE* htmlFile = fopen(kHtmlFileName, "w");
    if (!htmlFile) { list->error = DumpFileOpenningError; assert(0); }

    fprintf(htmlFile, "<!DOCTYPE html>\n");
    fprintf(htmlFile, "<html lang=\"en\">\n");
    fprintf(htmlFile, "  <head>\n");
    fprintf(htmlFile, "    <meta charset=\"utf-8\">\n");
    fprintf(htmlFile, "    <title>dump</title>\n");
    fprintf(htmlFile, "    <link rel=\"stylesheet\" href=\"style.css\">\n");
    fprintf(htmlFile, "    <script src=\"script.js\"></script>\n");
    fprintf(htmlFile, "  </head>\n");
    fprintf(htmlFile, "  <body>\n");

    FILE* namesOfGraphs = fopen(kGraphNameFile, "r");
    if (!namesOfGraphs) { list->error = DumpFileOpenningError; assert(0); }
    char buffer[kSizeOfBuffer] = {};
    while (fscanf(namesOfGraphs, "%s", buffer) != EOF)
    {
        buffer[strlen(buffer) - strlen(kDumpFileExtension)] = '\0';
        fprintf(htmlFile, "    <img src=\"%s%s\"/>\n", buffer, kGraphExtension);
        fprintf(stderr, "dot %s%s -Tpng -o %s%s\n", buffer, kDumpFileExtension, buffer, kGraphExtension);
    }
    FCLOSE(namesOfGraphs);

    fprintf(htmlFile, "  </body>\n");
    fprintf(htmlFile, "</html>\n");

    FCLOSE(htmlFile);
}

static ErrorCodes listError(const dblLinkedList* const list)
{
    if (!list)                           { return NullPointerDereference; }
    if (!list->data)                     { return NullPointerDereference; }
    if (!list->next)                     { return NullPointerDereference; }
    if (!list->prev)                     { return NullPointerDereference; }
    if (!list->counter && list->next[0]) { return ListUnderflow         ; }
    if (list->counter >= kSize)          { return ListOverflow          ; }
    if (list->error)                     { return list->error           ; }
                                         { return Success               ; }
}

static ErrorCodes listAssertFunc(dblLinkedList* const list, const char * file, const int line, const char * const func)
{
    if (!listError(list)) { return Success; }

    fprintf(stderr, "%s:%d in function: %s\n", file, line, func);
    dump(list);
    dtor(list);

    assert(0);
}
