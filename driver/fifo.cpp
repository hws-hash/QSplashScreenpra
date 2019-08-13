#include "fifo.h"
#include <stdlib.h>
#include <string.h>

#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
//取a和b中最小值
#define min(a, b) (((a) < (b)) ? (a) : (b))

struct fifo *fifo_init(unsigned char *buffer, unsigned int size)
{
  struct fifo *fifo;
  /* size must be a power of 2 */
  //BUG_ON(!is_power_of_2(size));
  fifo = (struct fifo *)malloc(sizeof(struct fifo));
  if (!fifo){
    return 0;
  }

  fifo->buffer = buffer;
  fifo->size = size;
  fifo->in = fifo->out = 0;

  return fifo;
}

static unsigned int roundup_pow_of_two(unsigned int size){

  if(is_power_of_2(size)){//已经是2的n次幂
    return size;
  }

  int value=0;
  const int bit=sizeof(unsigned int)*8-1;
  for(int i=0; i<bit; ++i){
    if( size&(1<<i) ){
      value=i;
    }
  }
  return 1<<(value+1);
}

struct fifo *fifo_alloc(unsigned int size)
{
  unsigned char *buffer;
  struct fifo *ret;

  //size不应该大于0x80000000
  size = roundup_pow_of_two(size);
  buffer = (unsigned char *)malloc(size);
  if (!buffer){
    return 0;
  }
  ret = fifo_init(buffer, size);

  if (ret == 0)
    free(buffer);
  return ret;
}


//释放缓冲区
void fifo_free(struct fifo *fifo){
  if (fifo) {
    free(fifo->buffer);
    free(fifo);
  }
}



unsigned int fifo_put(struct fifo *fifo,
                       const unsigned char *buffer, unsigned int len)
{
  unsigned int l;
  //buffer中空的长度
  len = min(len, fifo->size - fifo->in + fifo->out);

  /* first put the data starting from fifo->in to buffer end */
  l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
  memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
  /* then put the rest (if any) at the beginning of the buffer */
  memcpy(fifo->buffer, buffer + l, len - l);


  fifo->in += len;  //每次累加，到达最大值后溢出，自动转为0
  return len;
}

unsigned int fifo_get(struct fifo *fifo,
                       unsigned char *buffer, unsigned int len)
{
  unsigned int l;
  //有数据的缓冲区的长度
  len = min(len, fifo->in - fifo->out);

  /* first get the data from fifo->out until the end of the buffer */
  l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
  memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
  /* then get the rest (if any) from the beginning of the buffer */
  memcpy(buffer + l, fifo->buffer, len - l);

  fifo->out += len; //每次累加，到达最大值后溢出，自动转为0
  return len;
}


unsigned int fifo_len(struct fifo *fifo)
{
  return (fifo->in - fifo->out);
}
