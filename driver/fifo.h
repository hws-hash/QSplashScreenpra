#ifndef FIFO_H
#define FIFO_H

struct fifo {
  unsigned char *buffer;     /* the buffer holding the data */
  unsigned int size;         /* the size of the allocated buffer */
  unsigned int in;           /* data is added at offset (in % size) */
  unsigned int out;          /* data is extracted from off. (out % size) */
};

struct fifo *fifo_alloc(unsigned int size);
struct fifo *fifo_init(unsigned char *buffer, unsigned int size);
void fifo_free(struct fifo *fifo);
unsigned int fifo_put(struct fifo *fifo,
                       const unsigned char *buffer, unsigned int len);

unsigned int fifo_get(struct fifo *fifo,
                       unsigned char *buffer, unsigned int len);
unsigned int fifo_len(struct fifo *fifo);

#endif // FIFO_H
