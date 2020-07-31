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
#ifndef __SA_H__
#define __SA_H__

/******************************** Types **************************************/

/** Sorted positions of suffix array */
typedef struct pos_t {

    /** Index of original string */
    unsigned int idx;

    /** Position of index */
    unsigned int inv_pos;

    /** Longest common prefix with previous postion index */
    unsigned int lcp;

} pos_t;

/** Suffix array structure */
typedef struct sa_t {

    /** Main string */
    unsigned char *str;

    /** Length of main string */
    unsigned int  len;

    /** Index of original string */
    unsigned int *idx;

    /** Position of index */
    unsigned int *inv;

    /** Position of index */
    unsigned int *lcp;

    /** Sorted array element */
    pos_t         *pos;

} sa_t;

#endif
/* __SA_H__ */
