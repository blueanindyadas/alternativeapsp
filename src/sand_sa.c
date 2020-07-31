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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DEBUGLEVEL 1
#include <sa.h>
#include <apsp_read_dir.h>

#include <trace.h>

extern Trace_t trace;

/**
 * @brief Print the interger array
 */
void printV(int* a, int n, const char *comment) {
    int i;
    for ( i = 0;  i < n;  i++) {
        trace( trace, TRACE_DEBUG, "At index = %d %s = %d ", i, comment, a[i] );
    }
}
/** printV() */

/**
 * @brief lexicographic order for pairs
*/
inline int leq2( int a1, int a2,   int b1, int b2 ) {
    return(( a1 < b1 ) || (( a1 == b1 ) && ( a2 <= b2 ))); 
}
/** leq2() */

/**
 * @brief lexicographic order for triples
*/
inline int leq( int a1, int a2, int a3,   int b1, int b2, int b3 ) {
    return(( a1 < b1 ) || (( a1 == b1 ) && leq2( a2, a3, b2, b3 ))); 
}
/** leq() */

/**
 * @brief Stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
*/
static void radixPass( int* a, int* b, int* r, int n, int K ) 
{
    /** count occurrences */
    int* c = ( int * )calloc( sizeof( int ), ( K + 1 )); /** counter array */
    int i, t, sum, temp1, temp2, temp3;

    /** reset counters */
    for( i = 0;  i <= K;  i++) c[i] = 0;

    /** count occurences */
    for( i = 0;  i < n;  i++ ) {
#if 1
        temp1 = a[i];
        temp2 = r[temp1];
        c[temp2]++;
#else
        c[r[a[i]]]++;
#endif
    }

    /** exclusive prefix sums */
    for( i = 0, sum = 0;  i <= K;  i++) {
         t = c[i];  c[i] = sum;  sum += t;
    }

    /** sort */
    for( i = 0;  i < n;  i++) {
#if 1 /* Added by Anindya */
            temp1 = a[i];
            temp2 = r[temp1];
            temp3 = c[temp2]++;
            b[temp3] = a[i];
#else
            b[c[r[a[i]]]++] = a[i];
#endif
    }

    free( c );
}
/** radixPass() */

/**
 * @brief Create suffix array from interger array of symbols
 *        Find the suffix array SA of s[0..n-1] in {1..K}^n
 * @param s  Integer array of symbols
 * @param SA Suffix array
 * @param n  Length of suffix array
 * @param K  Highest integer symbol
 * @return Nothing
 * @note Require s[n]=s[n+1]=s[n+2]=0, n>=2
*/
void sand_sa( int* s, int* SA, int n, int K ) {
    int n0 = ( n + 2 ) / 3, n1 = ( n + 1 ) / 3, n2 = n / 3, n02 = n0 + n2; 
    int* s12  = ( int * )calloc( sizeof( int ), ( n02 + 3 ));
    int* SA12 = ( int * )calloc( sizeof( int ), ( n02 + 3 ));
    int* s0   = ( int * )calloc( sizeof( int ), n0 );
    int* SA0  = ( int * )calloc( sizeof( int ), n0 );
    int i, name, p, j, t, c0, c1, c2, k;
 
#if 1
    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0; 
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
#else
    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = n02; 
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = n02;
#endif

    /** generate positions of mod 1 and mod  2 suffixes */
    /** the "+ ( n0 - n1 )" adds a dummy mod 1 suffix if n%3 == 1 */
    for( i = 0, j = 0;  i < n + ( n0 - n1 );  i++ ) if( i % 3 != 0 ) {
        s12[j++] = i;
    }

    /** lsb radix sort the mod 1 and mod 2 triples */
    radixPass( s12 , SA12, s + 2, n02, K );
    radixPass( SA12, s12 , s + 1, n02, K );  
    radixPass( s12 , SA12, s    , n02, K );

    /** find lexicographic names of triples */
    name = 0, c0 = -1, c1 = -1, c2 = -1;
    for( i = 0;  i < n02;  i++ ) {
        if( s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2 ) { 
            name++;  c0 = s[SA12[i]];  c1 = s[SA12[i]+1];  c2 = s[SA12[i]+2];
        }

        /** Left half */
        if( SA12[i] % 3 == 1 ) { s12[SA12[i] / 3]      = name; }

        /** Right half */
        else                  { s12[SA12[i]/3 + n0] = name; }
    }

    /** Recurse if names are not yet unique */
    if( name < n02 ) {
        sand_sa( s12, SA12, n02, name );

        /** store unique names in s12 using the suffix array */
        for( i = 0;  i < n02;  i++ ) s12[SA12[i]] = i + 1;
    } else {

        /** Generate the suffix array of s12 directly */
        for( i = 0;  i < n02;  i++ ) SA12[s12[i] - 1] = i;
    }

    /** Stably sort the mod 0 suffixes from SA12 by their first character */
    for( i = 0, j = 0;  i < n02;  i++ ) {
        if( SA12[i] < n0 ) s0[j++] = 3 * SA12[i];
    }
    radixPass( s0, SA0, s, n0, K );

    /** Merge sorted SA0 suffixes and sorted SA12 suffixes */
    for( p = 0,  t = n0 - n1,  k = 0;  k < n;  k++ ) {
#define GetI() ( SA12[t] < n0 ? SA12[t] * 3 + 1 : ( SA12[t] - n0 ) * 3 + 2 )

        /** Pos of current offset 12 suffix */
        i = GetI();

        /** Pos of current offset 0  suffix */
        j = SA0[p];
        if( SA12[t] < n0 ? 
                leq2( s[i], s12[SA12[t] + n0], s[j], s12[j/3] ) :
                leq( s[i], s[i + 1], s12[SA12[t] - n0 + 1], s[j],s[j + 1], s12[j / 3 + n0] )) {
            
            /** Suffix from SA12 is smaller */
            SA[k] = i;  t++;
            if( t == n02 ) {
                    
                /** Done --- only SA0 suffixes left */
                for( k++;  p < n0;  p++, k++) SA[k] = SA0[p];
            }
        } else { 
            SA[k] = j;  p++; 
            if( p == n0 )  {
                    
                /** Done --- only SA12 suffixes left */
                for( k++;  t < n02;  t++, k++) SA[k] = GetI(); 
            }
        }  
    } 
    free( s12 ); free( SA12 ); free( SA0 ); free( s0 ); 
}
/** sand_sa() */

/**
 * @brief Create interger array from a file containing character symbol
 * @param str Integer array to be outputted
 * @param farg File name
 * @return Number of element outputted in array
 */
int sand_str_create( int **str, char *farg, unsigned char **string )
{
    unsigned int flen;
    unsigned int i = 0;
#if 0
    int ch;
#endif
    char *fname;
    char *str_concat = NULL;

    fname = ( char * )calloc( sizeof( char ), ( strlen( farg ) + 1 ));
    strncpy( fname, farg, strlen( farg ));
    fname[strlen( farg )] = '\0';

#if 0
    if( NULL == ( fp = fopen( fname, "r" ))) {
        fprintf( stdout, "Fail to open file %s", fname );
        return 0;
    }

    fseek( fp, 0L, SEEK_END );
    flen = ftell( fp );
    *str = ( int * )calloc( sizeof( int ), ( flen + 4 ));
    *string = ( unsigned char * )calloc( sizeof( unsigned char ),
                                         ( flen + 2 ));

    fseek( fp, 0L, SEEK_SET );
    while( EOF != ( ch = fgetc( fp ))) {
        ( *str )[i]    = (( unsigned char ) ch );
        ( *string )[i] = (( unsigned char ) ch );
        i++;
    }
#endif
    flen = apsp_read_dir( fname, &str_concat );
    //flen = apsp_read_str( fname, &str_concat );

    /* For last '~' character */
    flen++;
    *str = ( int * )calloc( sizeof( int ), ( flen + 3 ));
    *string = ( unsigned char * )calloc( sizeof( unsigned char ),
                                         ( flen + 1 ));
    for( i = 0; i < ( flen - 1 ); i++ ) {
        ( *str )[i] = ( unsigned char )str_concat[i];
        ( *string )[i] = ( unsigned char )str_concat[i];
    }
    ( *str )[flen - 1] = '~';
    ( *str )[flen] = '~';
    ( *str )[flen + 1] = '~';
    ( *string )[flen - 1]  = '~';

    free( str_concat );
    free( fname );

    return flen;
}
/** sand_str_create() */

/**
 * @brief Prepare integer array of given string
 * @param str File name containg string of which suffix array to be created
 * @return Suffix array
 */
sa_t *
sand_sa_create( char *str )
{
    int *s;
    int i, n;
    sa_t *sa = NULL;
    int *SA = NULL;

    sa = ( sa_t * )calloc( sizeof( sa_t ), 1 );

    n = sand_str_create( &s, str, &sa->str );
    SA = ( int * )calloc( sizeof( int ), ( n + 3 ));
    for ( i = 0;  i < n;  i++) SA[i] = 1;
    SA[n] = SA[n + 1] = SA[n + 2] = n;
    //printV(s, n, "s"); /* For debug */
    //sand_sa( s, SA, n + 1, 255 ); /* For input of total length less than 32 */
    sand_sa( s, SA, n, 255 ); /* For input of total length greater than 31 */

    //sa->pos = ( pos_t * )calloc( sizeof( pos_t ), n + 1 ); /* For old usage */
    sa->idx = ( unsigned int * )calloc( sizeof( unsigned int ), n + 1 );
    sa->inv = ( unsigned int * )calloc( sizeof( unsigned int ), n + 1 );
    for( i = 0; i < n - 1; i++ ) {
        //sa->pos[i].idx = SA[i];
        //sa->pos[SA[i]].inv_pos = i; /* For old usage */
        sa->idx[i] = SA[i];
        sa->inv[SA[i]] = i;
    }
    /* Last four line was patch badly */
    //sa->pos[n - 1].idx = n - 1; /* For old usage */
    //sa->pos[n - 1].inv_pos = n - 1; /* For old usage */
    sa->idx[n -1] = n -1;
    sa->inv[n - 1] = n -1;
    /* Suffix array length increased by one for last sentinel character */
    sa->len = n;

    //printV(SA, sa->len, "SA"); /* For debug */

    free( SA );
    free( s );
    return sa;
}
/** sand_sa_create() */

/**
 @brief Free allocations in suffix array
 @param sa Suffix array
 @return Nothing
 */
void
sand_sa_destroy( sa_t *sa )
{

    /* Freeing memory to match valgrind memory check pass */
    if( NULL != sa ) {
        if( NULL != sa->pos ) {
            free( sa->pos );
            sa->pos = NULL;
        }
        if( NULL != sa->str ) {
            free( sa->str );
            sa->str = NULL;
        }
        free( sa );
        sa = NULL;
    }
}
/* sand_sa_destroy() */

