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

/************************* Header files ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

#include <trace.h>
#include <aapsp_read_str.h>

/***************************** MACRO Definitions ******************************/

#define INPUT_FILE "../../../corpus/all_pairs_suffix_prefix_problem/all_ests.fasta"
//#define INPUT_FILE "../../../corpus/all_pairs_suffix_prefix_problem/c_elegans_ests.fasta"
#define MAX_STR_LEN (1000000 * 300)

/***************************** Global Variable ********************************/

/* Trace object; other file must extern this object */
Trace_t trace;

/* Number of strings */
unsigned int num_strings;

/* Prefix sums of strings' indices */
unsigned int *istrEnd;

/************************* Procedure Definitions ******************************/

#if 0
/**
 * @brief For testing
 **/
int
main()
{
    char *str = NULL;
    unsigned int str_len = 0;

    str_len = apsp_read_str( INPUT_FILE, &str );

    fprintf( stdout, "Total %u bytes read from %s file\n",
             str_len, INPUT_FILE );

    return 0;
}
/* main() */
#endif

/**
 * @brief Read files containg strings
 * @param filename Input file to read
 * @param str_c The read string is kept in this string
 * @return Total length of read string
 **/
unsigned int
apsp_read_str( char *filename, char **str_c )
{
    FILE *fp = NULL;
    unsigned int i = 0, len = 0;
    unsigned int prev_i = 0;
    int idx_strings = 0;
    char ch;

    if( NULL == filename ) {
        fprintf( stdout, "No file name!!!\n" );
        return 0;
    }

    if( NULL == ( fp = fopen( filename, "r" ))) {
        fprintf( stdout, "Error to open %s:%s\n",
                 filename, strerror( errno ));
        return 0;
    }
    ch = fgetc( fp );
    while( EOF != ch ) {
        if( '>' == ch ) {
            while( '\n' != ( ch = fgetc( fp )));
        }

        num_strings++;

        while( '\n' == ch ) {
            ch = fgetc( fp );
            while(( 'A' == ch ) ||
                  ( 'C' == ch ) ||
                  ( 'G' == ch ) ||
                  ( 'T' == ch ) ||
                  ( 'H' == ch ) ||
                  ( 'Y' == ch ) ||
                  ( 'S' == ch ) ||
                  ( 'R' == ch ) ||
                  ( 'B' == ch ) ||
                  ( 'V' == ch ) ||
                  ( 'K' == ch ) ||
                  ( 'W' == ch ) ||
                  ( 'M' == ch ) ||
                  ( 'N' == ch )) {
                len++;
                ch = fgetc( fp );
            }
        }
    }
    num_strings++;
    len++;
    fclose( fp );

    trace( trace, TRACE_INFO, "Total %u bytes and %d "
           "strings read\n", len, num_strings );

    /* Allocate memory to hold all strings in one */
    *str_c = ( char * )calloc( len, sizeof( char ));
    if( NULL == *str_c ) {
        return 0;
    }

    /* Allocate memory for prefix sums all strings' indices */
    istrEnd = ( unsigned int * )calloc( num_strings, sizeof( unsigned int ));
    if( NULL == istrEnd ) {
        trace( trace, TRACE_ERROR, "Fail to allocate memory for prefix "
               "sums of strings indices" );
        return 0;
    }

    if( NULL == ( fp = fopen( filename, "r" ))) {
        trace( trace, TRACE_ERROR, "Error to open %s:%s\n",
               INPUT_FILE, strerror( errno ));
        return 0;
    }
    i = 0;
    ch = fgetc( fp );
    while( EOF != ch ) {
        if( '>' == ch ) {
            while( '\n' != ( ch = fgetc( fp )));
        }

        while( '\n' == ch ) {
            ch = fgetc( fp );
            while(( 'A' == ch ) ||
                  ( 'C' == ch ) ||
                  ( 'G' == ch ) ||
                  ( 'T' == ch ) ||
                  ( 'H' == ch ) ||
                  ( 'Y' == ch ) ||
                  ( 'S' == ch ) ||
                  ( 'R' == ch ) ||
                  ( 'B' == ch ) ||
                  ( 'V' == ch ) ||
                  ( 'K' == ch ) ||
                  ( 'W' == ch ) ||
                  ( 'M' == ch ) ||
                  ( 'N' == ch )) {
                (*str_c)[i] = ch;
                i++;
                ch = fgetc( fp );
            }
        }
        if( i > prev_i ) {
            istrEnd[idx_strings++] = i;
            prev_i = i;
        }
    }
    i++;
    istrEnd[idx_strings] = i;
    fclose( fp );

    trace( trace, TRACE_INFO, "%u bytes and %d "
           "strings read\n", i, idx_strings );

    return ( len - 1 );
}
/* apsp_read_str() */
