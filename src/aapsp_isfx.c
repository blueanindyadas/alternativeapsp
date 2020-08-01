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
#include <stdio.h>
#include <stdlib.h>

#include <apsp_read_str.h>
#include <sa.h>
#include <trace.h>
/***************************** Global Variable ********************************/

unsigned int *isfx;
unsigned int *istr_no;
extern Trace_t trace;

/************************* Procedure Definitions ******************************/

/**
 * @brief Create isfx array
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
int
apsp_compute_isfxlen( sa_t *sa )
{
    unsigned int i, j, isfx_ovlap;
    unsigned int *str_lengths = NULL;
    unsigned int *isa = sa->inv;

    /* Allocate memory for required LCP */
    isfx = ( unsigned int * )calloc( sa->len, sizeof( unsigned int ));
    if( NULL == isfx ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for required lcp" );
        return -1;
    }

    istr_no = ( unsigned int * )calloc( sa->len, sizeof( unsigned int ));
    if( NULL == istr_no ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for required lcp" );
        return -1;
    }


    str_lengths = ( unsigned int * )calloc( num_strings,
                                            sizeof( unsigned int ));
    if( NULL == str_lengths ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for"
                " temporary prefix sums of strings indices" );
        return -1;
    }

    /* create a copy of all istrEnd */
    str_lengths[0] = istrEnd[0];
    for( i = 1; i < num_strings; i++ )
    {
        str_lengths[i] = istrEnd[i] - istrEnd[i - 1];
    }

    i = num_strings - 1;
    isfx_ovlap = 1;
    isfx[sa->len - 1] = isfx_ovlap;
    j = sa->len - 1;
    while(((int)str_lengths[0]) > 0 ) {
        isfx[isa[j]] = isfx_ovlap;
        istr_no[isa[j]] = i + 1;
        j--;
        (str_lengths[i])--;
        if(( str_lengths[i] == 0 ) && ( i != 0 )) {
            i--;
            isfx_ovlap = 1;
        } else {
            isfx_ovlap++;
        }
    }

    free( str_lengths );

    return 0;
}
/* apsp_compute_isfxlen() */
