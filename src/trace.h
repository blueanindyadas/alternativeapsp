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

/******************************* Headers *************************************/
#include <stdio.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <stdint.h>

/******************************* Constants ***********************************/
#define MAX_FILE_NAME_LEN 1024

/* Trace flags */
#define TRACE_ALWAYS  0x0000
#define TRACE_DEBUG   0x0001
#define TRACE_INFO    0x0002
#define TRACE_WARNING 0x0004
#define TRACE_ERROR   0x0008
#define TRACE_FATAL   0x0010

#define TRACE_ALL ( TRACE_DEBUG   | \
                    TRACE_INFO    | \
                    TRACE_WARNING | \
                    TRACE_ERROR   | \
                    TRACE_FATAL )

#define TRACE_MAXFILESZ 5242880
#define TRACE_STR_LEN   1024

/* Default values of configuration parameters */
#define TRACE_PATH_DEFAULT      "/tmp/trace_bwt.txt"
#define TRACE_BACKUPDIR_DEFAULT "trace_backupdir/"
#define TRACE_SIZE_DEFAULT      TRACE_MAXFILESZ
/******************************* Types ***************************************/

/* Trace structures */
typedef struct Trace_t {
    FILE *trace_fd;
    char fname[MAX_FILE_NAME_LEN];

    /* Default log will be WARNING, ERROR & FATAL;
     * Can be change for INFO & DEBUG */
    int  level;

    /* Initially 0; Will change dynamically with each trace done */
    int  sz_left;
    int  max_sz;
    char *path_name;
} Trace_t;

/******************************* Macros **************************************/
#ifdef WITHOUT_TRACE
#define trace( trce, lvl, fmt, ... ) do{} while( 0 )
#else
#define trace( trce, lvl, fmt, ... ) \
    if( trce.sz_left < TRACE_STR_LEN ) trace_roll(&trce); \
    if( lvl == ( trce.level & lvl )) \
        trace_do( &trce, __FILE__, __LINE__, #lvl, fmt, ##__VA_ARGS__ )
#endif

/************************** Procedure declaration ****************************/

/*
 * @brief Initialize a trace
 * @param trace Trace instance
 * @param fname Trace file name
 * @param max_sz Maximum trace file size
 * @param backup_dir Trace file rollback directory
 * @param level Trace level, Only these levels are being trace using trace macro
 * @return
 * */
extern int
trace_open( Trace_t    *trace,
            const char *fname,
            uint64_t   max_sz,
            const char *backup_dir,
            int        level );

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
extern void
trace_str( Trace_t *trace,
           char    *str,
           int     start_pos,
           int     len );

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
extern void
trace_do( Trace_t    *trace,
          char       *fname,
          int        line,
          char       *level_str,
          const char *format, ... );

/*
 * @brief Roll trace
 * @param Trace Trace instance
 * @return
 * */
int
trace_roll( Trace_t *trace );

/*
 * @brief
 * @param Trace Trace instance
 * @return
 * */
int
trace_close( Trace_t *trace );
