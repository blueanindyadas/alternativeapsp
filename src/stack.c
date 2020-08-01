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
#include <stack.h>

/************************** Procedure definitions ****************************/

/* *
 * @brief Initialize STACK
 * @param st STACK
 * @return Nothing
 * */
void
stackinit( stack_t *st )
{
    st->top = NULL;
    st->count = 0;
}
/* stackinit() */

/* *
 * @brief STACK.pop() implemented
 * @param top STACK.top
 * @return top element of stack, i.e. STACK.top.sp
 * */
void *
pop( stack_t *st )
{
    stackentry_t *top = st->top;
    void *obj = NULL;

    if( NULL != top ) {
        obj = top->obj;
        st->top = top->sp;
        st->count--;
        free( top );
    }

    return obj;
}
/* pop() */

/**
 @brief Remove from stack
 @param stack STACK
 @param pstack Present stack entry
 @param cstack Next stack entry
 @return Object pointer that it stores
 **/
void *
removest( stack_t *stack,
          stackentry_t *pstack,
          stackentry_t *cstack )
{
    void *obj = NULL;

    if( pstack == cstack ) {
        obj = pop( stack );
    } else {
        obj = cstack->obj;
        pstack->sp = cstack->sp;
        stack->count--;
        free( cstack );

    }

    return obj;
}
/* removest() */

/* *
 * @brief STACK.push( element ) implemented
 * @param st STACK
 * @param obj New stack element to push in the STACK
 * @return Nothing
 * */
void
push( stack_t *st, void *obj )
{
    stackentry_t *top;

    if( NULL == st ) {
        return;
    }

    top = ( stackentry_t * )calloc( 1, sizeof( stackentry_t ));
    if( NULL == top ) {
        fprintf( stderr, "Fail to allocate memory for statckentry" );
        return;
    }
    
    top->sp = st->top;
    top->obj = obj;
    st->top = top;
    st->count++;
}
/* push() */
