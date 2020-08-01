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
#include <time.h>

#include <trace.h>
#include <sa.h>

#include <sand_sa.h>
#include <kasai_lcp.h>
#include <aapsp_isfx.h>
#include <aapsp_prefix.h>
#include <aapsp_read_str.h>

/***************************** MACRO Definitions ******************************/
#define MAX_TEXT_LEN 1024
#define MAX_PATTERN_LEN 1024

/***************************** Global Variable ********************************/

/* Trace object; other file must extern this object */
Trace_t trace;

/************************* Procedure Definitions ******************************/

/**
 * @brief Implenting usage
 * @return Nothing
 **/
void
apsp_usage( void )
{
    printf( "./apsp -[-f TEXT_FILE] [-t THRESHOLD]\n" );
    return;
}
/* apsp_usage() */

/**
 * @brief Print given suffix array
 * @param sa Suffix array
 * @return Nothing
 **/
void
apsp_sa_print( sa_t *sa )
{
    unsigned int i;

    for( i = 0; i < ( sa->len - 1 ); i++ ) {
#if 0
        trace( trace, TRACE_DEBUG, "i = %3u, pos = %3u, inv = %3u, lcp = %3u, "
               "isfx = %3u, str = \"%.*s...\"", i, sa->pos[i].idx,
               sa->pos[i].inv_pos,
               sa->pos[i].lcp, isfx[i],
               ( sa->pos[i].lcp > sa->pos[i + 1].lcp ) ?
               ( sa->pos[i].lcp + 1 ): ( sa->pos[i + 1].lcp + 1 ),
               &sa->str[sa->pos[i].idx] );
#else
        trace( trace, TRACE_DEBUG, "i = %3u, pos = %3u, inv = %3u, lcp = %3u, "
               "isfx = %3u, pflag = %3u, str = \"%.*s...\"", i, sa->idx[i],
               sa->inv[i],
               sa->lcp[i], isfx[i], pfl[i],
               ( sa->lcp[i] > sa->lcp[i + 1] ) ?
               ( sa->lcp[i] + 1 ): ( sa->lcp[i + 1] + 1 ),
               &sa->str[sa->idx[i]] );
#endif
    }
#if 0
    trace( trace, TRACE_DEBUG, "i = %3u, pos = %3u, inv = %3u, lcp = %3u, "
           "isfx = %3u, str = \"%.*s...\"", i, sa->pos[i].idx,
           sa->pos[i].inv_pos, sa->pos[i].lcp, isfx[i],
           ( sa->pos[i].lcp + 1 ), &sa->str[sa->pos[i].idx] );
#else
    trace( trace, TRACE_DEBUG, "i = %3u, pos = %3u, inv = %3u, lcp = %3u, "
           "isfx = %3u, pflag = %3u, str = \"%.*s...\"", i, sa->idx[i],
           sa->inv[i], sa->lcp[i], isfx[i], pfl[i],
           ( sa->lcp[i] + 1 ), &sa->str[sa->idx[i]] );
#endif
}
/* apsp_sa_print() */

/**
 * @brief Print given suffix array in latex tabular format
 * @param sa Suffix array
 * @return Nothing
 **/
void
apsp_sa_lprint( sa_t *sa )
{
    unsigned int i;
#define TRACE_STR_LEN 1024
    char str_trace[TRACE_STR_LEN];
    char *ptr_str_trace;
    unsigned int ret = 0;

    ptr_str_trace = str_trace;
    ret = snprintf( ptr_str_trace, TRACE_STR_LEN, "\\hline\n$i$ & $SA$ & $ISA$"
            " & $LCP$ & $LSFX$ & $LSTR$ & $PFlag$ & "
            "$SA_{SA[i]}$\\\\\n\\hline\n" );
    trace_str( &trace, str_trace, 0, ret );
    bzero( str_trace, TRACE_STR_LEN );
    for( i = 0; i < ( sa->len - 1 ); i++ ) {
#if 0
        ret = snprintf( ptr_str_trace, TRACE_STR_LEN, "%3u & %3u & %3u & %3u & "
               "%3u & ``$%.*s\\cdots$\"\\\\\n\\hline\n", i, sa->pos[i].idx,
               sa->pos[i].inv_pos, sa->pos[i].lcp, isfx[i],
               ( sa->pos[i].lcp > sa->pos[i + 1].lcp ) ?
               ( sa->pos[i].lcp + 1 ): ( sa->pos[i + 1].lcp + 1 ),
               &sa->str[sa->pos[i].idx] );
#else
        ret = snprintf( ptr_str_trace, TRACE_STR_LEN, "%3u & %3u & %3u & %3u & "
               "%3u & %3u & %3u & ``$%.*s\\cdots$\"\\\\\n\\hline\n",
               i, sa->idx[i], sa->inv[i], sa->lcp[i], isfx[i], istr_no[i],
               pfl[i], ( sa->lcp[i] > sa->lcp[i + 1] ) ?
               ( sa->lcp[i] + 1 ): ( sa->lcp[i + 1] + 1 ),
               &sa->str[sa->idx[i]] );
#endif
        trace_str( &trace, str_trace, 0, ret );
        bzero( str_trace, TRACE_STR_LEN );
    }
#if 0
    ret = snprintf( ptr_str_trace, TRACE_STR_LEN, "%3u & %3u & %3u & %3u & "
            " %3u & ``$%.*s\\cdots$\"\\\\\n\\hline\n",
            i, sa->pos[i].idx, sa->pos[i].inv_pos,
           sa->pos[i].lcp, isfx[i], ( sa->pos[i].lcp + 1 ),
           &sa->str[sa->pos[i].idx] );
#else
    ret = snprintf( ptr_str_trace, TRACE_STR_LEN, "%3u & %3u & %3u & %3u & "
            " %3u & %3u & %3u & ``$%.*s\\cdots$\"\\\\\n\\hline\n",
            i, sa->idx[i], sa->inv[i], sa->lcp[i], isfx[i], istr_no[i], pfl[i],
            ( sa->lcp[i] + 1 ), &sa->str[sa->idx[i]] );
#endif
    trace_str( &trace, str_trace, 0, ret );
    bzero( str_trace, TRACE_STR_LEN );
    fflush( trace.trace_fd );
}
/* apsp_sa_print() */

/**
 * @brief Destroy all allocated data structures
 * @param sa Suffix array
 * Return None
 **/
void
apsp_destroy( sa_t *sa )
{
    free( isfx );
    free( istr_no );
    free( pfl );
    free( istrEnd );

    free( sa->lcp );
    free( sa->inv );
    free( sa->idx );
    free( sa->str );
    free( sa );
}
/* apsp_destroy() */

/**
 * @brief Main procedure
 **/
int
main( int argc, char *argv[] )
{
    int  option;
    sa_t *sa = NULL;
    char in_file[MAX_FILE_NAME_LEN] = { '\0' };
    int ret = 0;
	time_t t_start;
	clock_t c_start;

    /* Parsing options */
    while(( option = getopt( argc, argv, "f:t:" )) != -1 ) {
        switch( option ) {
            case 'f': strncpy( in_file, optarg, strlen( optarg ));
                      break;
            case 't': threshold = atoi( optarg );
                      break;
            default:
                      printf( "Invalid option !!!\n");
                      apsp_usage();
        }
    }

    /* Open & initialize texts */
    trace_open( &trace, "trace_apsp.txt",
                5000000, "trace_backup/", TRACE_ALL );

    printf( "You will find current log at \"%s\"\n", trace.fname );

    if( 0 == strcmp( "", in_file )) {
        trace( trace, TRACE_FATAL, "No text file" );
        exit( 0 );
    }

    trace( trace, TRACE_INFO, "APSP started with threshold = %u, for input "
           "file %s", threshold, in_file );

    sa = sand_sa_create( in_file );
    if( NULL == sa ) {
        trace( trace, TRACE_ERROR, "Fail to create SA" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully SA created" );

    if( 0 != kasai_sa_lcp_create( sa )) {
        trace( trace, TRACE_ERROR, "Fail to create lcp" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully LCP created" );

    t_start = time(NULL);
    c_start = clock();
    if( 0 != ( ret = apsp_compute_isfxlen( sa ))) {
        trace( trace, TRACE_ERROR, "Fail to create LSFX" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully LSFX created" );

    if( 0 != ( ret = apsp_init_ovlap())) {
        trace( trace, TRACE_ERROR, "Fail to initialize ovlap matrix" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully initialize ovlap matrix" );

    if( 0 != ( ret = apsp_mark_prefix( sa ))) {
        trace( trace, TRACE_ERROR, "Fail to mark start of strings" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully marking start of strings" );

    if( 0 != ( ret = apsp_mark_suffix( sa ))) {
        trace( trace, TRACE_ERROR, "Fail to mark end of strings" );
        return 0;
    }
    trace( trace, TRACE_INFO, "Successfully mark end of strings" );

    if( 0 != ( ret = apsp_compute_ovlap( sa ))) {
        trace( trace, TRACE_ERROR, "Fail to determine suffix prefix overlap" );
        return 0;
    }
    trace( trace, TRACE_INFO, "File = %s, threshold = %u, CLOCK = %lf "
           "TIME = %lf (in seconds)\n", in_file, threshold,
           (clock() - c_start) / (double)(CLOCKS_PER_SEC),
           difftime (time(NULL),t_start));
    trace( trace, TRACE_INFO, "Successfully determine suffix prefix "
           "overlap %lu", ovlapcnt );

    apsp_ovlap_close();

#ifdef OVLAP_MEM
    apsp_lprint_ovlap();

    apsp_sa_print( sa );
    apsp_sa_lprint( sa );
#endif

    apsp_destroy( sa );

    /* Close trace */
    trace_close( &trace );

    return 0;
}
/* main() */
