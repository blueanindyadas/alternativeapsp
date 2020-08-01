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

/************************* Header files ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <trace.h>
#include <sa.h>
#include <apsp_read_str.h>
#include <apsp_isfx.h>
#include <apsp_prefix.h>
#include <stack.h>
#include <queue.h>

#define OVLAP_FILE_NAME "apsp.out"
/***************************** Global Variable ********************************/

unsigned int *pfl;
#ifdef OVLAP_FILE
FILE *ovlap_file;
#elif OVLAP_MEM
unsigned int *ovlap;
#endif
stack_t stack;
queue_t queue;
extern Trace_t trace;
unsigned int queue_ovlen_max;
unsigned int stack_max_isfx;
unsigned long int ovlapcnt;
int threshold;

/************************* Procedure Definitions ******************************/

/**
 * @brief Mark prefixes in a seperate array
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
int
apsp_mark_prefix( sa_t *sa )
{
    unsigned int i;
    unsigned int *pos = sa->inv;

    pfl = ( unsigned int * )calloc( sa->len, sizeof( unsigned int ));
    if( NULL == pfl ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for pfl" );
        return 0;
    }

    pfl[pos[0]] = 1;
    for( i = 0; i < (num_strings - 1); i++ ) {
        pfl[pos[istrEnd[i]]] = 1;
    }

    return 0;
}
/* apsp_mark_prefix() */

/**
 * @brief Initialize ovlap matrix
 * @return 0 on success, -1 otherwise
 */
int
apsp_init_ovlap()
{

    /* If threshold is not supplied, set it to 1 */
    if( threshold == 0 ) threshold = 1;
#ifdef OVLAP_FILE
    ovlap_file = fopen( OVLAP_FILE_NAME, "w" );
    if( NULL == ovlap_file ) {
        trace( trace, TRACE_ERROR, "Fail to open %s file", OVLAP_FILE_NAME );
        return 0;
    }
#elif OVLAP_MEM
    unsigned int size;

    size = num_strings * num_strings;
    trace( trace, TRACE_INFO, "Allocating %u elements of "
           "size 4 bytes\n", size );
    ovlap = ( unsigned int * )calloc( size, sizeof( unsigned int ));
    if( NULL == ovlap ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for ovlap matrix" );
        return -1;
    }
#endif
    return 0;
}
/* apsp_init_ovlap() */

/**
 * @brief Finds overlaps with prefix
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
int
apsp_mark_suffix( sa_t *sa )
{
    unsigned int i = 0;
    unsigned int *lcp = sa->lcp;

    for( i = 0; i < sa->len; i++ ) {
        if(( lcp[i] >= threshold ) && ( lcp[i] >= isfx[i] )) {
            pfl[i] |= 0x00000002;
        }
        if(( lcp[i+1] >= isfx[i] ) && ( isfx[i] >= threshold )) {
            pfl[i] |= 0x00000004;
        }
    }
    return 0;
}
/* apsp_mark_suffix() */

/**
 * @brief Add an SA element to queue
 * @param sa Suffix array
 * @param id Index of SA which have to add in queue
 * @return 0 on success, -1 otherwise
 */
int
apsp_queue_add( sa_t *sa, unsigned int i )
{
    prefix_t *prefix;

    prefix = ( prefix_t * )calloc( 1, sizeof( prefix_t ));
    if( NULL == prefix ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for prefix" );
        return -1;
    }
    prefix->i = i;
    prefix->said = sa->idx[i];
    prefix->isa =  sa->inv[i];
    prefix->ovlen = sa->lcp[i + 1];

    ENQUEUE( queue, prefix );

#if 1 /* Check later and modify to later: TODO */
    if( sa->lcp[i] > sa->lcp[i + 1] ) {
        queue_ovlen_max = sa->lcp[i];
    } else {
        queue_ovlen_max = sa->lcp[i + 1];
    }
#else
    if( sa->lcp[i + 1] > queue_ovlen_max ) {
        queue_ovlen_max = sa->lcp[i + 1];
    }
#endif

    return 0;
}
/* apsp_queue_add() */

/**
 * @brief Push an SA element to stack
 * @param sa Suffix array
 * @param id SA index to push
 * @return 0 on success, -1 otherwise
 */
int
apsp_stack_push( sa_t *sa, unsigned int i )
{
    suffix_t *suffix;

    suffix = ( suffix_t * )calloc( 1, sizeof( suffix_t ));
    if( NULL == suffix ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for suffix" );
        return -1;
    }
    suffix->i = i;
    suffix->said = sa->idx[i];
    suffix->isa =  sa->inv[i];
    suffix->lcp = sa->lcp[i+1];
    suffix->ovlen = isfx[i];

    PUSH( stack, suffix );

    if( isfx[i] > stack_max_isfx ) {
        stack_max_isfx = isfx[i];
    }

    return 0;
}
/* apsp_stack_push() */

/**
 * @brief Flush all element from queue
 * @return None
 */
void apsp_queue_flush()
{
    prefix_t *prefix;

    /* Delete all from queue */
    while( NULL != queue.front ) {
        prefix = ( prefix_t * )DEQUEUE( queue );
        free( prefix );
    }

}
/* apsp_queue_flush() */

/**
 * @brief Flush all element from stack
 * @return None
 */
void apsp_stack_flush()
{
    suffix_t *suffix;

    /* Pop all from stack */
    while( NULL != stack.top ) {
        suffix = ( suffix_t * )POP( stack );
        free( suffix );
    }
}
/* apsp_stack_flush() */

#if 0
/**
 * @brief Set new lcp value of ovlap possible withthat
 * @return None
 */
void
apsp_stack_reset( unsigned int lcp )
{
    stackentry_t *pstack, *cstack, *nstack;
    suffix_t *suffix = NULL;

    if( stack_max_lcp > lcp ) {
        pstack = nstack = stack.top;
        //if( NULL != nstack ) {
        while( NULL != nstack ) {
            suffix = ( suffix_t * )nstack->obj;

            //while( lcp < suffix->ovlen ) {
            /* pop not up to lesser ovlen; removal is
             * required for all lesser ovlen */
            if( lcp < suffix->ovlen ) {
                //POP( stack );
                cstack = nstack->sp;
                removest( &stack, pstack, nstack );
                free( suffix );

                if( pstack == nstack ) pstack = cstack;
                nstack = cstack;
                //nstack = stack.top;
                //if( NULL == nstack ) break;
                //suffix = ( suffix_t * )nstack->obj;
                if( NULL != nstack ) {
                    suffix = ( suffix_t * )nstack->obj;
                }
            } else {
                pstack = nstack;
                nstack = nstack->sp;
            }
        }
        stack_max_lcp = lcp;
    }
}
/* apsp_stack_reset() */
#else
/**
 * @brief Set new lcp value of ovlap possible withthat
 * @return None
 */
void
apsp_stack_reset( unsigned int lcp )
{

    /* Previous, current, next stack */
    stackentry_t *pstack, *cstack, *nstack;
    suffix_t *suffix = NULL;

    if( stack_max_isfx > lcp ) {
        pstack = cstack = stack.top;
        if( NULL != cstack ) {
            suffix = ( suffix_t * )cstack->obj;

            while( lcp < suffix->lcp ) { /* TODO: '<' instead of '<=' */
                /* Pop all lesser ovlen; Check all suffix with at
                 * least of lcp of current suffix's lcp */
                if( lcp < suffix->ovlen ) {
                    nstack = cstack->sp;
                    removest( &stack, pstack, cstack );
                    free( suffix );

                    if( pstack == cstack ) pstack = nstack;
                    cstack = nstack;
                } else {
                    pstack = cstack;
                    cstack = cstack->sp;
                }
                if( NULL != cstack ) {
                    suffix = ( suffix_t * )cstack->obj;
                } else break;
            }
        }
        stack_max_isfx = lcp;
    }
}
/* apsp_stack_reset() */
#endif

/**
 * @brief Set new lcp value of ovlap possible with that
 * @return None
 */
void
apsp_queue_lcp_reset( unsigned int lcp )
{
    queueentry_t *nqueue;
    prefix_t *prefix = NULL;

    /* At least, queue have some prefixes with possible-overlap lengths
     * greater than current lcp; Those need to be reduced to current lcp */
    if( queue_ovlen_max > lcp ) {
        nqueue = queue.rear;
        if( nqueue != NULL ) {
            prefix = ( prefix_t * )nqueue->obj;
            while( prefix->ovlen > lcp ) {
                prefix->ovlen = lcp;
                nqueue = nqueue->prev;
                if( nqueue != NULL ) prefix = ( prefix_t * )nqueue->obj;
                else break;
            }
        }
        queue_ovlen_max = lcp;
    }
}
/* apsp_queue_lcp_reset() */

/**
 * @brief Fill ovlap matrix for all suffix in stack
 *        with newly added prefix in que
 * @param lcp Maximum lcp that can be ovlap
 * @return None
 */
void
apsp_stack_ovlap_detect( sa_t *sa, unsigned int i )
{
    stackentry_t *nstack;
    suffix_t *suffix;

    if( stack_max_isfx <= sa->lcp[i] ) {
        nstack = stack.top; 
        while( nstack != NULL ) {
            suffix = ( suffix_t * )nstack->obj;

            /* Ensure prefix string is longer than overlap */
            if( isfx[i] >= suffix->ovlen ) {
                if( suffix->ovlen < sa->lcp[i] ) {
                    ovlapcnt++;
#ifdef OVLAP_FILE
                    fprintf( ovlap_file, "[%6u, %6u, %6u]\n",
                            ( istr_no[i] - 1 ), ( istr_no[suffix->i] - 1 ),
                            suffix->ovlen );
#elif OVLAP_MEM
                    if( ovlap[( num_strings * ( istr_no[i] -1 )) +
                            ( istr_no[suffix->i] - 1 )] < suffix->ovlen ) {
        
                        ovlap[( num_strings * ( istr_no[i] -1 )) +
                                ( istr_no[suffix->i] - 1 )] = suffix->ovlen;
                    }
#endif
                } else { /* i.e. suffix->ovlen == sa->lcp[i] */
                    ovlapcnt++;
                    //trace( trace, TRACE_INFO, "Suffix(%u)->ovlen (%u)>= sa->lcp[%u] (%u)\n",
                            //suffix->i,suffix->ovlen, i, sa->lcp[i] );
#ifdef OVLAP_FILE
                    fprintf( ovlap_file, "[%6u, %6u, %6u]\n",
                            ( istr_no[i] - 1 ), ( istr_no[suffix->i] - 1 ),
                            sa->lcp[i] );
#elif OVLAP_MEM
                    if( ovlap[( num_strings * ( istr_no[i] - 1 )) +
                            ( istr_no[suffix->i] - 1 )] < sa->lcp[i] ) {
                        ovlap[( num_strings * ( istr_no[i] - 1 )) +
                            ( istr_no[suffix->i] - 1 )] = sa->lcp[i];
                    }
#endif
                }
            }
            nstack = nstack->sp;
        }
    }
}
/* apsp_stack_ovlap_detect() */

/**
 * @brief Fill ovlap matrix for all prefix in queue
 *        with newly added suffix in stack
 * @param isfx element of isfx which is the maximum ovlap length
 * @return None
 * @note If we precess the queue from rear to front we dont have to process all
 *       prefix. Only last prefixes with ovlen => isfx[i] can ovlap with the
 *       current suffix.Prior to that prefix with ovlen < isfx[i] are not
 *       sufficient length to overlap with current suffix. So, queue would be
 *       double linked queue, so that it cab be processed from rear to front
 *       direction. (TODO)
 */
void
apsp_queue_ovlap_detect( sa_t *sa, unsigned int i )
{
    queueentry_t *nqueue;
    prefix_t *prefix;

    if( queue_ovlen_max >= isfx[i] ) {
        nqueue = queue.rear;
        if( NULL != nqueue ) {
            prefix = ( prefix_t * )nqueue->obj;
            while( prefix->ovlen >= isfx[i] ) {
                ovlapcnt++;
#ifdef OVLAP_FILE
                fprintf( ovlap_file, "[%6u, %6u, %6u]\n",
                        ( istr_no[prefix->i] - 1 ), ( istr_no[i] - 1 ),
                        isfx[i] );
#elif OVLAP_MEM
                if( ovlap[( num_strings * ( istr_no[prefix->i] - 1 )) +
                        ( istr_no[i] - 1 )] < isfx[i] ) {
                    ovlap[( num_strings * ( istr_no[prefix->i] - 1 )) +
                        ( istr_no[i] - 1 )] = isfx[i];
                }
#endif
                nqueue = nqueue->prev;
                if( NULL != nqueue ) prefix = ( prefix_t * )nqueue->obj;
                else break;
            }
        }
        if( queue_ovlen_max > sa->lcp[i] ) {
            queue_ovlen_max = sa->lcp[i];
        }
    }
}
/* apsp_queue_ovlap_detect() */

/**
 * @brief Determine overlap matrix
 * @return 0 on success, -1 otherwise
 * @note Prefixes are always kept in queue and stack keeps the suffixes 
 */
int
apsp_compute_ovlap( sa_t *sa )
{
    unsigned int i;
    unsigned int *lcp = sa->lcp;

    QINIT( queue );
    STKINIT( stack );
    for( i = 0; i < ( sa->len - 1 ); i++ ) {

        if(( i % 1000000 ) == 0 ) {
            trace( trace, TRACE_INFO, "i = %u, Count(queue) = %u, "
                    "Count(stack) = %u, overlap = %lu", i,
                    queue.count, stack.count, ovlapcnt );
        }

        /* If a suffix with lcp value 0 found */
        if( sa->lcp[i] < threshold ) {

            apsp_queue_flush();
            apsp_stack_flush();
        } else {

            /* Reset elements of stack */
            apsp_stack_reset( lcp[i] );
            apsp_queue_lcp_reset( lcp[i] );
        }

        if(( pfl[i] & 0x00000002 ) == 0x00000002 ) {

            /* Fill all ( queue's prefix /\ stack's suffixes) ovlap matrix with
             * value min( queue.max_ovlap_len, stack.isfx) */
            apsp_queue_ovlap_detect( sa, i );
        }

        if(( pfl[i] & 0x00000001 ) == 0x00000001 ) {

            /* Push the suffix with lcp and isfx; Fill all
             * ( queue's prefix /\ stack's suffixes) ovlap matrix with
             * value min( queue.max_ovlap_len, stack.isfx) */
            if( lcp[i] >= threshold ) {
                apsp_stack_ovlap_detect( sa, i );
            }

            /* Add at the rear of queue with the
             * queue[qidx].prefix.max_ovlap_len = isfx[i] */
            if( lcp[i + 1] >= threshold ) {
                if( 0 != apsp_queue_add( sa, i )) {
                    return -1;
                }
            }
        }

        if(( pfl[i] & 0x00000004 ) == 0x00000004 ) {

            /* Push the suffix with lcp and isfx; */
            if( 0 != apsp_stack_push( sa, i )) {
                return -1;
            }
        }
    }
    return 0;
}
/* apsp_compute_ovlap() */

/**
 * @brief Print ovlap matrix in latex format
 * @return None
 */
void
apsp_lprint_ovlap()
{
#ifdef OVLAP_MEM
    unsigned int i, j;
#define TRACE_STR_LEN 1024
    char str_trace[TRACE_STR_LEN];
    char *ptr_str_trace;
    int ret, remain_str = TRACE_STR_LEN, size = 0;

    ptr_str_trace = str_trace;

    ret = snprintf( ptr_str_trace, remain_str,
            "\n\\begin{table}[H]\n\\begin{center}\n\\begin{tabular}{c|" );
    remain_str -= ret;
    ptr_str_trace += ret;
    size += ret;
    for( i = 0; i < num_strings; i++ ) {

        ret = snprintf( ptr_str_trace, remain_str, "c|" );
        remain_str -= ret;
        ptr_str_trace += ret;
        size += ret;
    }
    ret = snprintf( ptr_str_trace, remain_str, "}\n" );
    remain_str -= ret;
    ptr_str_trace += ret;
    size += ret;
    trace_str( &trace, str_trace, 0, size );
    bzero( str_trace, TRACE_STR_LEN );
    remain_str = TRACE_STR_LEN;
    ptr_str_trace = str_trace;
    size = 0;

    ret = snprintf( ptr_str_trace, remain_str,
            "\\multicolumn{1}{c}{}" );
    remain_str -= ret;
    ptr_str_trace += ret;
    size += ret;
    for( i = 0; i < num_strings; i++ ) {

        ret = snprintf( ptr_str_trace, remain_str,
                " & \\multicolumn{1}{c}{%d}", i + 1 );
        remain_str -= ret;
        ptr_str_trace += ret;
        size += ret;
    }
    ret = snprintf( ptr_str_trace, remain_str, "\\\\\n" );
    remain_str -= ret;
    ptr_str_trace += ret;
    size += ret;
    trace_str( &trace, str_trace, 0, size );
    bzero( str_trace, TRACE_STR_LEN );
    ptr_str_trace = str_trace;
    remain_str = TRACE_STR_LEN;
    size = 0;

    for( i = 0; i < num_strings; i++ ) {

        ret = snprintf( ptr_str_trace, remain_str, "\\cline{2-%u}\n %d",
                ( num_strings + 1 ), ( i + 1 ));
        remain_str -= ret;
        ptr_str_trace += ret;
        size += ret;
        for( j = 0; j < num_strings; j++ ) {
            ret = snprintf( ptr_str_trace, remain_str, " & %u",
                    *( ovlap + ( num_strings * i ) + j ));
            remain_str -= ret;
            ptr_str_trace += ret;
            size += ret;
        }
        ret = snprintf( ptr_str_trace, remain_str, "\\\\\n" );
        remain_str -= ret;
        ptr_str_trace += ret;
        size += ret;
        trace_str( &trace, str_trace, 0, size );
        bzero( str_trace, TRACE_STR_LEN );
        ptr_str_trace = str_trace;
        remain_str = TRACE_STR_LEN;
        size = 0;
    }
    ret = snprintf( ptr_str_trace, remain_str,
            "\\cline{2-%u}\n\\end{tabular}\n\\end{center}\n\\end{table}\n", 
            ( num_strings + 1 ));
    remain_str -= ret;
    ptr_str_trace += ret;
    size += ret;
    trace_str( &trace, str_trace, 0, size );
    bzero( str_trace, TRACE_STR_LEN );
    ptr_str_trace = str_trace;
    remain_str = TRACE_STR_LEN;
    size = 0;
#endif
}
/* apsp_lprint_ovlap() */

/**
 * @brief Close file for output matrix
 * @return Nothing
 **/
void
apsp_ovlap_close()
{
#ifdef OVLAP_FILE
    fclose( ovlap_file );
#endif
}
/* apsp_ovlap_close() */
