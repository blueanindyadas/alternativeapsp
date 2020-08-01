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
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <trace.h>
/***************************** MACRO Definitions ******************************/

#ifdef UNIT_TEST

/* For testing */
#define SRCDIR "strings32"
#endif

/***************************** Global Variable ********************************/

/* Trace object; other file must extern this object */
extern Trace_t trace;

/* Number of strings */
unsigned int num_strings;

/* Prefix sums of strings' indices */
unsigned int *istrEnd;

/************************* Procedure Definitions ******************************/

/**
 * @brief Read all files content in a string for given directory
 * @param dirname Directory name
 * @param str_c Output string
 * @return Length of final string
 **/
unsigned int
apsp_read_dir( char *dirname, char **str_c )
{
    unsigned int len = 0;
    struct dirent *drnt;
    DIR *dir;
    struct stat st;
    char fl_name[512] = { '\0' };
    unsigned int i, idx_strings;
    FILE *fp = NULL;
    int ret;

    if( NULL == dirname ) {
        trace( trace, TRACE_ERROR, "Invalid directory name %p", dirname );
        return 0;
    }

    dir = opendir( dirname );
    if( NULL == dir ) {
        trace( trace, TRACE_ERROR, "Fail to open directory %s", dirname );
        return 0;
    }
    while( NULL != ( drnt = readdir( dir ))) {
        if( DT_REG == drnt->d_type ) {
            fl_name[0] = '\0';
            strcat( fl_name, dirname );
            strcat( fl_name, "/" );
            strcat( fl_name, drnt->d_name );
            stat( fl_name, &st );
            len += st.st_size;
            num_strings++;
            trace( trace, TRACE_DEBUG, "Getting the file (\"%s\")  "
                    "size (%ld)\n", fl_name, (long int)st.st_size );
        }
    }
    closedir( dir );
    num_strings++;
    len++;

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
    

    i = 0;
    dir = opendir( dirname );
    if( NULL == dir ) {
        return 0;
    }
    idx_strings = 0;
    while( NULL != ( drnt = readdir( dir ))) {
        if( DT_REG == drnt->d_type ) {
            fl_name[0] = '\0';
            strcat( fl_name, dirname );
            strcat( fl_name, "/" );
            strcat( fl_name, drnt->d_name );
            stat( fl_name, &st );
            trace( trace, TRACE_DEBUG, "Reading file \"%s\" of size %ld in "
                    "str_c[%u]\n", fl_name, (long int)st.st_size, i );
            if( NULL == ( fp = fopen( fl_name, "rb" ))) {
                return 0;
            }
            if (0 == (ret = fread( &( (*str_c )[i] ), 1, st.st_size, fp ))) {
                trace( trace, TRACE_DEBUG, "File \"%s\" is empty", fl_name);
            }
            fclose( fp );
            i += st.st_size;
            istrEnd[idx_strings++] = i;
        }
    }
    i++;
    istrEnd[idx_strings] = i;
    closedir( dir );

    /* Trailing '\0' character is manipulated */
    return ( len - 1 );
}
/* apsp_read_dir() */

#ifdef UNIT_TEST
int
main()
{
    unsigned int tot_len = 0;
    char *str_concatenated = NULL;

    tot_len = apsp_read_dir( SRCDIR, &str_concatenated );
    printf( "The final string of lenght %u is given below\n%s\n",
            tot_len, str_concatenated );

    return 0;
}
/* main() */
#endif
