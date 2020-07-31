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
#ifndef __APSP_READ_STR_H
#define __APSP_READ_STR_H

/* Prefix sums of strings' indices */
extern unsigned int *istrEnd;

/* Number of strings */
extern unsigned int num_strings;

/**
 * @brief Read string file
 * @param filename Input file to read
 * @param str_c The read string is kept in this string
 * @return Total length of read string
 **/
extern unsigned int
apsp_read_str( char *file_name, char **str_c );

/* __APSP_READ_STR_H */
#endif

