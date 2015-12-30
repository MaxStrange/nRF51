#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "circular_queue.h"

void circular_queue_construct(volatile circular_queue *q)
{
  q->read_index = 0;
  q->write_index = 0;
  q->read_and_write_on_same_lap = true;
}

bool circular_queue_is_empty(volatile circular_queue *q)
{
  return ((q->read_and_write_on_same_lap) && (q->read_index >= q->write_index));
}

char circular_queue_read_next_char(volatile circular_queue *q)
{
  char c = q->queue[q->read_index];
  q->queue[q->read_index] = '\0';//clear out that character
  q->read_index += 1;

  if (q->read_index >= QUEUE_SIZE)
  {
    q->read_index = 0;
    q->read_and_write_on_same_lap = true;
  }

  return c;
}

void circular_queue_remove_next_char(volatile circular_queue *q)
{
  q->queue[q->read_index] = '\0';//clear out that character
  q->read_index += 1;

  if (q->read_index >= QUEUE_SIZE)
  {
    q->read_index = 0;
    q->read_and_write_on_same_lap = true;
  }
}

void circular_queue_write_char(volatile circular_queue *q, char c)
{
  q->queue[q->write_index] = c;
  q->write_index += 1;

  if (q->write_index >= QUEUE_SIZE)
  {
    q->write_index = 0;
    q->read_and_write_on_same_lap = false;
  }
}
