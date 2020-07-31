/* *****************************************************************************
 * Copyright 2020 Anindya Das
 *
 * This file is part of apsp.
 *
 * apsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * apsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with apsp.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__

/******************************* Types ***************************************/

/* STACK link to next element */
typedef struct queueentry_t {

    struct queueentry_t *next;
    struct queueentry_t *prev;

    void *obj;

} queueentry_t;

/* Stack implementation */
typedef struct queue_t {

    /* front element of queue */
    queueentry_t *front;

    /* rear element of queue */
    queueentry_t *rear;

    /* Number of element int queue */
    unsigned int count;

} queue_t;

#define QINIT( QUEUE ) do { \
    queueinit( &QUEUE ); \
} while( 0 )

#define DEQUEUE( QUEUE ) dequeue( &QUEUE )

#define ENQUEUE( QUEUE, obj ) do { \
    enqueue( &QUEUE, ( void * )obj ); \
} while( 0 )
/************************** Procedure declaration ****************************/

/**
 * @brief Initialize QUEUE
 * @param queue Pointer to queue
 * @return Nothing
 **/
extern void
queueinit( queue_t *queue );

/**
 * @brief QUEUE.deeque() implemented
 * @param queue
 * @return front element from stack, NULL for empty stack
 * @note Caller should frees front
 **/
extern void *
dequeue( queue_t *queue );

/**
 * @brief QUEUE.enqueue( element ) implemented
 * @param queue QUEUE
 * @param rear Rear element to add
 * @return Nothing
 * @note Caller should allocate queueentry_t
 **/
extern void
enqueue( queue_t *queue, void *obj );

#endif
/* __QUEUE_H__ */
