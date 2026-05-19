#include "../include/VigiFile.h"

void queue_init(Queue *q)
{
    q->capacity = INITIAL_CAPACITY;
    q->front = 0;
    q->back = 0;

    q->data = (char **)malloc(
        sizeof(char *) * q->capacity
    );
}

void queue_destroy(Queue *q)
{
    while (q->front < q->back)
    {
        free(q->data[q->front++]);
    }

    free(q->data);
}

int queue_empty(Queue *q)
{
    return q->front == q->back;
}

void enqueue(Queue *q, const char *path)
{
    if (q->back >= q->capacity)
    {
        q->capacity *= 2;

        q->data = (char **)realloc(
            q->data,
            sizeof(char *) * q->capacity
        );
    }

    q->data[q->back++] = strdup(path);
}

char *dequeue(Queue *q)
{
    if (queue_empty(q))
        return NULL;

    return q->data[q->front++];
}