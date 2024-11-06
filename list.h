#ifndef LIST_H
#define LIST_H

#include <string.h>

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while(0)
#define LISTASSERT(list_) \
    do { listAssertFunc(list_, __FILE__, __LINE__, __func__); } while(0)

const size_t kSize = 10;

enum ErrorCodes
{
    ElementNotFound        = -1,
    Success                =  0,
    ListOverflow           =  1,
    ListUnderflow          =  2,
    OutOfBoundsListAccess  =  3,
    DumpFileOpenningError  =  4,
    NullPointerDereference =  5,
};

struct dblLinkedList
{
    int data[kSize];
    int next[kSize];
    int prev[kSize];

    int free;
    size_t counter;
    ErrorCodes error;
};

ErrorCodes ctor(dblLinkedList* const list);
ErrorCodes dtor(dblLinkedList* const list);
ErrorCodes dump(dblLinkedList* const list);
ErrorCodes pushFront(dblLinkedList* const list, const int value);
ErrorCodes pushBack (dblLinkedList* const list, const int value);
ErrorCodes pushBeforeNth(dblLinkedList* const list, const size_t index, const int value);
ErrorCodes pushAfterNth (dblLinkedList* const list, const size_t index, const int value);
ErrorCodes deleteNth(dblLinkedList* const list, const size_t index);
int front(dblLinkedList* const list);
int back(dblLinkedList* const list);
int next(dblLinkedList* const list, const size_t index);
int prev(dblLinkedList* const list, const size_t index);
int getNth(dblLinkedList* const list, const size_t index);
int search(dblLinkedList* const list, int value);

#endif // LIST_H
