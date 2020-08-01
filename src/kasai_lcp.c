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

/**************************** Header files ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include <trace.h>
#include <kasai_lcp.h>

/***************************** Global Variable ********************************/
extern Trace_t trace;

char *str_loc;
char *str_pos;
char *str_inv_pos;
char *str_flag_bucket_pos;
char *str_bucket_num;
char *str_lcp;

#if 0
extern unsigned char *new_bucket_flag;
extern unsigned int  *bucket_num;
#endif
/**************************** Procedure Definition ****************************/

/* *
 * @brief Calculating and allocating memory buffer for printing
 * @param sa Suffix Array structure
 * @return return Nothing
 * */
void
kasai_sa_print_init( sa_t *sa )
{
     unsigned int tempLenSaStr = 0, size = 0;
     unsigned int maxPow = 1;
     unsigned int i;

     /* Calculating the size for ', ' */
     size = 2 * sa->len;

     tempLenSaStr = sa->len;
     for( i = 0; tempLenSaStr > 0; i++ ) {
             tempLenSaStr /= 10;
             maxPow *= 10;
     }
     tempLenSaStr = sa->len;
     while( maxPow ) {
             maxPow /= 10;
             size += (( tempLenSaStr - maxPow ) * i );
             i--;
             tempLenSaStr = maxPow;
     }
     size++;

     /* For string append at front of numbers to indicate the list of */
     size += 200;

     trace( trace, TRACE_DEBUG, "Calculated size for printing %u integers "
            "is %u", sa->len, size );

     str_loc = (char *)calloc( 1, size );
     str_pos = (char *)calloc( 1, size );
     str_inv_pos = (char *)calloc( 1, size );
     str_flag_bucket_pos = (char *)calloc( 1, size );
     str_bucket_num = (char *)calloc( 1, size );
     str_lcp = (char *)calloc( 1, size );
}
/* kasai_sa_print_init() */

/* *
 * @brief Print the Suffix Array
 * @param sa Suffix Array
 * @return Nothing
 * */
void
kasai_sa_print( sa_t *sa )
{
     unsigned int i, ret, ptr_loc_remain, ptr_pos_remain, ptr_inv_pos_remain;
#if 0
     unsigned int ptr_flag_bucket_pos_remain, ptr_bucket_num_remain;
#endif
     unsigned int ptr_lcp_remain;
     char *ptr_loc = str_loc;
     char *ptr_pos = str_pos;
     char *ptr_inv_pos = str_inv_pos;
#if 0
     char *ptr_flag_bucket_pos = str_flag_bucket_pos;
     char *ptr_bucket_num = str_bucket_num;
#endif
     char *ptr_lcp = str_lcp;

#if 0
     ptr_bucket_num_remain =
     ptr_flag_bucket_pos_remain =
#endif
     ptr_inv_pos_remain =
     ptr_pos_remain =
     ptr_loc_remain =
     ptr_lcp_remain =
         sa->len * 7;

     /* Getting location string */
     ret = snprintf( ptr_loc, ptr_loc_remain, "Location    : " );
     ptr_loc += ret;
     ptr_loc_remain -= ret;

     /* Getting sorted postion */
     ret = snprintf( ptr_pos, ptr_pos_remain, "Position idx: ");
     ptr_pos += ret;
     ptr_pos_remain -= ret;

     /* Getting longest common prefix */
     ret = snprintf( ptr_lcp, ptr_lcp_remain, "LCP         : ");
     ptr_lcp += ret;
     ptr_lcp_remain -= ret;

     /* Getting inverted sorted position */
     ret = snprintf( ptr_inv_pos, ptr_inv_pos_remain, "Inverse pos : ");
     ptr_inv_pos += ret;
     ptr_inv_pos_remain -= ret;

#if 0
     /* Getting new_bucket_flag */
     ret = snprintf( ptr_flag_bucket_pos,
                     ptr_flag_bucket_pos_remain, "New buck fl : ");
     ptr_flag_bucket_pos += ret;
     ptr_flag_bucket_pos_remain -= ret;

     /* Getting bucket number string */
     ret = snprintf( ptr_bucket_num, ptr_bucket_num_remain, "Bucket num  : ");
     ptr_bucket_num += ret;
     ptr_bucket_num_remain -= ret;
#endif

     for( i = 0; i < sa->len; i++ ) {

          /* Getting location string */
          ret = snprintf( ptr_loc, ptr_loc_remain,
                          ( i == ( sa->len - 1 )) ? "%2u\n" : "%2u, ", i );
          ptr_loc += ret;
          ptr_loc_remain -= ret;

          /* Getting sorted postion */
          ret = snprintf( ptr_pos,
                          ptr_pos_remain,
                          ( i == ( sa->len - 1 )) ? "%2u\n" : "%2u, ",
                          sa->pos[i].idx );
          ptr_pos += ret;
          ptr_pos_remain -= ret;

          /* Getting longest common prefix */
          ret = snprintf( ptr_lcp,
                          ptr_lcp_remain,
                          ( i == ( sa->len - 1 )) ? "%2u\n" : "%2u, ",
                          sa->pos[i].lcp );
          ptr_lcp += ret;
          ptr_lcp_remain -= ret;

          /* Getting inverted sorted position */
          ret = snprintf( ptr_inv_pos,
                          ptr_inv_pos_remain,
                          ( i == ( sa->len - 1 )) ? "%2u\n" : "%2u, ",
                          ( &sa->pos[i] )->inv_pos );
          ptr_inv_pos += ret;
          ptr_inv_pos_remain -= ret;

#if 0
          /* Getting new_bucket_flag */
          ret = snprintf( ptr_flag_bucket_pos,
                          ptr_flag_bucket_pos_remain,
                          ( i == ( sa->len - 1 )) ? "%2hu\n" : "%2hu, ",
                          new_bucket_flag[i] );
          ptr_flag_bucket_pos += ret;
          ptr_flag_bucket_pos_remain -= ret;

          /* Getting bucket number string */
          ret = snprintf( ptr_bucket_num,
                          ptr_bucket_num_remain, 
                          ( i == ( sa->len - 1 )) ? "%2u\n" : "%2u, ",
                          bucket_num[i] );
          ptr_bucket_num += ret;
          ptr_bucket_num_remain -= ret;
#endif
     }

     /* Printing all the arrays */
     trace_str( &trace, str_loc, 0, strlen( str_loc ));
     bzero( str_loc, strlen( str_loc ));

     trace_str( &trace, str_pos, 0, strlen( str_pos ));
     bzero( str_pos, strlen( str_pos ));

     trace_str( &trace, str_lcp, 0, strlen( str_lcp ));
     bzero( str_lcp, strlen( str_lcp ));

     trace_str( &trace, str_inv_pos, 0, strlen( str_inv_pos ));
     bzero( str_inv_pos, strlen( str_inv_pos ));

     trace_str( &trace, str_flag_bucket_pos, 0, strlen( str_flag_bucket_pos ));
     bzero( str_flag_bucket_pos, strlen( str_flag_bucket_pos ));

     trace_str( &trace, str_bucket_num, 0, strlen( str_bucket_num ));
     bzero( str_bucket_num, strlen( str_bucket_num ));

     return;
}
/* kasai_sa_print() */

/* *
 * @brief Create LCP of SA (Suffix Arrays)
 * @param sa Suffix Array structure
 * @return 0 on success; -1 otherwise
 * */
int
kasai_sa_lcp_create( sa_t *sa )
{
    unsigned int i, j, h;
    unsigned char *a = sa->str;

    if(( trace.level & TRACE_DEBUG ) == TRACE_DEBUG ) {
        trace( trace, TRACE_DEBUG, "String is given below - " );
        trace_str( &trace, ( char * )sa->str, 0, sa->len );
        trace_str( &trace, "\n", 0, 1 );
    }

    sa->lcp = ( unsigned int * )calloc( sizeof( unsigned int ), sa->len );
    h = 0;
    for( i = 0; i < sa->len; i++ ) {
        if( sa->inv[i] > 0 ) {
            j = sa->idx[sa->inv[i] - 1];
            while( a[i + h] == a[j + h] ) {
                h = h + 1;
            }
            sa->lcp[sa->inv[i]] = h;
            if( h > 0 ) {
                h = h - 1;
            }
        }
    }

    if(( trace.level & TRACE_DEBUG ) == TRACE_DEBUG ) {
        trace( trace, TRACE_DEBUG, "After LCP computation - " );
#define MAX_DEBUG_STR_LEN 1024
        char tempStr[MAX_DEBUG_STR_LEN] = {'\0'};
        snprintf (tempStr, MAX_DEBUG_STR_LEN - 1,
                  "i\t\tSA[i]\tINV[i]\tLCP[i]\n");
        trace_str( &trace, tempStr, 0, strlen(tempStr) );
        for( i = 0; i < sa->len; i++ ) {
            memset (tempStr, 0, MAX_DEBUG_STR_LEN);
            snprintf (tempStr, MAX_DEBUG_STR_LEN - 1, "%d\t\t%d\t\t%d\t\t%d\n",
                      i, sa->idx[i], sa->inv[i], sa->lcp[i]);
            trace_str( &trace, tempStr, 0, strlen(tempStr) );
        }
    }

    return 0;
}
/* kasai_sa_lcp_create() */
