
#ifndef __SDRAM_CBUF_
#define __SDRAM_CBUF_


typedef struct { char value; } ElemType;
 
/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    ElemType   * elems;  /* vector of elements                   */
} CircularBuffer;

void cbInit(CircularBuffer *cb, void* addr, int size);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, ElemType *elem);
void cbRead(CircularBuffer *cb, ElemType *elem);

#endif
