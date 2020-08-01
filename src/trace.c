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

/******************************* Headers *************************************/
#define _SVID_SOURCE
#include <dirent.h>

/* va_arg example */
#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <time.h>       /* time related function */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>

#include "trace.h"

/**************************** Constants **************************************/

/************************** Static variables *********************************/
static char backuptrace_fname[MAX_FILE_NAME_LEN];

/************************** Procedure definitions ****************************/

/*
 * @brief Select a file it is named as trace.txt.[0-1]*
 * @usage This API is static called from trace_roll procedure
 * @param entry Dirctory entry
 * @return 1 if match; or 0 otherwise
 * */
static int
file_select( const struct direct *entry )
{
    const char *ptr = entry->d_name;

    if ((strcmp(entry->d_name, ".")== 0) ||
        (strcmp(entry->d_name, "..") == 0))
        return (0);

    /* Check for filename extensions */
#ifdef TEST_TRACE
    fprintf( stdout, "Checking file %s with %s\n",
             entry->d_name, backuptrace_fname );
#endif
    if (( ptr != NULL ) && ( strncmp( ptr,
                                      backuptrace_fname,
                                      strlen( backuptrace_fname )) == 0 )) {
        return (1);
    } else {
        return(0);
    }
}
/* file_select() */

/*
 * @brief Initialize a trace
 * @param trace Trace instance
 * @param fname Trace file name
 * @param max_sz Maximum trace file size
 * @param backup_dir Trace file rollback directory
 * @param level Trace level, Only these levels are being trace using trace macro
 * @return
 * */
int
trace_open( Trace_t    *trace,
            const char *fname,
            uint64_t   max_sz,
            const char *backup_dir,
            int        level )
{
    if( NULL == fname ) {
        fprintf( stdout, "Trace file is NULL\n" );
        return -1;
    }

    /* Open trace file if not already open */
    if( NULL == trace->trace_fd ) {

        trace->trace_fd = fopen( fname, "a+" );
        if( NULL == trace->trace_fd ) {
            fprintf( stdout, "Fail to open trace file \"%s\"\n", fname );
            return -1;
        }
        strncpy( trace->fname, fname, MAX_FILE_NAME_LEN - 1 );
        trace->level = level;
        trace->sz_left = max_sz;
        trace->max_sz = max_sz;
        trace->path_name = ( char * )calloc( 1, strlen( backup_dir ) + 1);
        strncpy( trace->path_name, backup_dir, strlen( backup_dir ));
    } else {
        fprintf( stdout, "Trace file already opened\n" );
    }

    return 0;
}
/* trace_open() */

/*
 * @brief  Trace without variables
 *
 * @param trace      [In] Trace variable
 * @param str        [In] Trace string
 * @param start_pos  [In] Starting position of the string to be print
 * @param len        [In] Length of the string to print
 *
 * @return None
 * */
void
trace_str( Trace_t *trace,
           char    *str,
           int     start_pos,
           int     len )
{
    int i;

    len += start_pos;
    for( i = start_pos; i < len; i++ ) {
        fputc( str[i], trace->trace_fd );
        trace->sz_left--;
    }
}
/* trace_str() */

/*
 * @brief  Initialize a trace
 *
 * @param trace      [In] Trace variable
 * @param fname      [In] Caller file name
 * @param line       [In] Line number of callee file
 * @param level_str  [In] Trace level string
 * @param format,... [In] Variable length arguments
 *
 * @return None
 * */
void
trace_do( Trace_t    *trace,
          char       *fname,
          int        line,
          char       *level_str,
          const char *format, ... )
{
    va_list valist;
    char trace_str[TRACE_STR_LEN] = { '\0' };
    int ret = 0;
    time_t tm;
    struct tm *local_tm;

    /* Initialize valist for num_args number of arguments */
    va_start( valist, format );

    /* Access all the arguments assigned to valist */
    ret = vsnprintf( trace_str, ( TRACE_STR_LEN - 2 ), format, valist );
    if( ret < 0 ) {
        fprintf( stderr, "Fail to trace line \"%s\"\n", format );
    } else {
        trace->sz_left -= ret;
        ret = snprintf( trace_str + ret, ( TRACE_STR_LEN - ret - 1 ), "\n" );
        trace->sz_left -= ret;
    }

    va_end( valist );

    tm = time( &tm );
    local_tm = localtime( &tm );

    fprintf( trace->trace_fd, "%04d-%02d-%02d %02d:%02d:%02d : (%s:%d) : %s - "
             "%s", ( 1900 + local_tm->tm_year ), ( 1 + local_tm->tm_mon ),
             local_tm->tm_mday, local_tm->tm_hour, local_tm->tm_min,
             local_tm->tm_sec, fname, line, level_str, trace_str );
    fflush( trace->trace_fd );
}
/* trace_do */

/*
 * @brief Roll trace
 * @param Trace Trace instance
 * @return
 * */
int
trace_roll( Trace_t *trace )
{
    //char path_name[] = "/home/anindya/work/phd/trace_backup/";
    struct direct **files;
    int count, lst_file_num = 0;
    char *ptr, *fptr;
    char new_file[MAX_FILE_NAME_LEN] = { '\0' };

    fptr = rindex( trace->fname, '/' );
    if( fptr == NULL ) {
        fptr = trace->fname;
    } else {
        fptr += 1;
    }
    strncpy( backuptrace_fname, fptr, strlen( fptr ));
    backuptrace_fname[strlen( fptr )] = '.';
    count = scandir( trace->path_name, &files, &file_select, alphasort );
#ifdef TEST_TRACE
    printf( "path_name = %s, count = %d\n", trace->path_name, count );
#endif
    if( 0 < count ) {
        ptr = rindex( files[count - 1]->d_name, '.' );
        ptr += 1; /* skipping last "." */
        lst_file_num = atoi( ptr );
#ifdef TEST_TRACE
        printf( "Total trace file found %d and last file number is %d\n",
                count, lst_file_num );
#endif
    } else if( 0 > count ) {
#ifdef TEST_TRACE
        printf( "Total trace file found %d in %s: %s\n",
                count, trace->path_name, strerror( errno ));
#endif
    }
#ifdef TEST_TRACE
    printf( "Total trace file found %d in %s\n",
            count, trace->path_name );
#endif

    /* When no 1*, @* etc... comes before 9 */
    if( lst_file_num < count ) lst_file_num = count;

    sprintf( new_file, "%s%s.%d", trace->path_name,
             ( const char * )fptr,
             ( lst_file_num + 1 ));
#ifdef TEST_TRACE
    printf( "Generating %s\n", new_file );
#endif

    if( 0 > trace_close( trace )) {
        fprintf( stderr, "Unable to close trace file\n" );
    }

    if( 0 > mkdir( trace->path_name, 0777 )) {
#ifdef TEST_TRACE
        fprintf( stderr, "Fail to create backup directory: %s\n", strerror( errno ));
#endif
    }
#ifdef TEST_TRACE
     printf("Renaming done\n");
#endif

    if( 0 > rename( trace->fname, new_file )) {
        fprintf( stderr, "Fail to move trace file \"%s\" in \"%s\" (%s)\n",
                 trace->fname, trace->path_name, strerror( errno ));
    }

    trace->trace_fd = fopen( trace->fname, "a+" );
    if( NULL == trace->trace_fd ) {
        fprintf( stdout, "Fail to open trace file \"%s\"\n", trace->fname );
        return -1;
    }
    trace->sz_left = trace->max_sz;

    return 0;
}
/* trace_roll() */

/*
 * @brief
 * @param Trace Trace instance
 * @return
 * */
int
trace_close( Trace_t *trace )
{
    int ret;

    /* Close trace file if not already close */
    if( trace->trace_fd != NULL ) {

        ret = fclose( trace->trace_fd );
        if( 0 != ret ) {
            fprintf( stdout, "Unable to close trace file\n" );
            return -1;
        }
        fprintf( stdout, "Trace file successfully closed\n" );

        trace->trace_fd = NULL;
    } else {
        fprintf( stdout, "Trace file already closed\n" );
    }

    return 0;
}
/* trace_close() */

/*
 * @brief
 * @param
 * @return
 * */
