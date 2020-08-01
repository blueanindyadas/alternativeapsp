/* *****************************************************************************
 * Copyright 2020 Anindya Das
 *
 * This file is part of aapsp.
 *
 * aapsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aapsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aapsp.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/

/********************************* Headers ***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>

/************************** Procedure definitions ****************************/

/**
 * @brief Initialize QUEUE
 * @param queue Pointer to queue
 * @return Nothing
 **/
void
queueinit( queue_t *queue )
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
}
/* queueinit() */

/**
 * @brief QUEUE.deeque() implemented
 * @param queue
 * @return front element from stack, NULL for empty stack
 **/
void *
dequeue( queue_t *queue )
{
    void *obj = NULL;
    queueentry_t *front = queue->front;

    if( NULL != queue->front ) {
        obj = front->obj;
        if( queue->front == queue->rear ) {
            queue->front = queue->rear = NULL;
        } else {
            queue->front = queue->front->next;
            queue->front->prev = NULL;
        }
        free( front );
        queue->count--;
    }

    return obj;
}
/* dequeue() */

/**
 * @brief QUEUE.enqueue( element ) implemented
 * @param queue QUEUE
 * @param rear Rear element to add
 * @return Nothing
 * @note Caller should allocate queueentry_t
 **/
void
enqueue( queue_t *queue, void *obj )
{
    queueentry_t *qentry = NULL;

    if(( NULL == obj ) || ( NULL == queue )) {
        return;
    }

    qentry = ( queueentry_t * )calloc( 1, sizeof( queueentry_t ));
    if( NULL == qentry ) {
        return;
    }

    qentry->prev = queue->rear;
    qentry->next = NULL;
    qentry->obj = obj;

    if( NULL == queue->rear ) {
        queue->front = queue->rear = qentry;
    } else {
        queue->rear->next = qentry;
        queue->rear = qentry;
    }
    queue->count++;
}
/* enqueue() */
