#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#define QUEUE_SIZE 100

typedef struct
{
  char queue[QUEUE_SIZE];
  uint32_t read_index;
  uint32_t write_index;
  bool read_and_write_on_same_lap;
} circular_queue;

void circular_queue_construct(volatile circular_queue *q);
bool circular_queue_is_empty(volatile circular_queue *q);
char circular_queue_read_next_char(volatile circular_queue *q);
void circular_queue_remove_next_char(volatile circular_queue *q);
void circular_queue_write_char(volatile circular_queue *q, char c);

#endif //CIRCULAR_QUEUE_H
