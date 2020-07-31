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
#ifndef __STACK_H__
#define __STACK_H__

/******************************* Types ***************************************/

/* STACK link to next element */
typedef struct stackentry_t {

    struct stackentry_t *sp;

    void *obj;

} stackentry_t;

/* Stack implementation */
typedef struct stack_t {

    struct stackentry_t *top;

    unsigned int count;

} stack_t;

#define STKINIT( STACK ) do { \
    stackinit( &STACK ); \
} while( 0 )

#define POP( STACK ) pop( &STACK )

#define PUSH( STACK, obj ) do { \
    push( &STACK, ( void * )obj ); \
} while( 0 )
/************************** Procedure declaration ****************************/

/**
 * @brief Initialize STACK
 * @param top STACK top
 * @return Nothing
 **/
extern void
stackinit( stack_t *top );

/**
 * @brief STACK.pop() implemented
 * @param top STACK.top
 * @return top element of stack, i.e. STACK.top.sp
 **/
extern void *
pop( stack_t *top );

/**
 * @brief STACK.push( element ) implemented
 * @param top STACK top
 * @param sp New stack element to push in the STACK
 * @return Nothing
 **/
extern void
push( stack_t *top, void *obj );

/**
 @brief Remove from stack
 @param stack STACK
 @param pstack Present stack entry
 @param nstack Next stack entry
 @return Object pointer that it stores
 **/
extern void *
removest( stack_t *stack,
          stackentry_t *pstack,
          stackentry_t *nstack );

#endif
/* __STACK_H__ */
