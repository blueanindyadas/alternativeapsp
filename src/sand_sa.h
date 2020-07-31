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
#ifndef __SAND_H__
#define __SAND_H__

struct sa_t;
typedef struct sa_t sa_t;

/**
 * @brief Prepare integer array of given string
 * @param SA Suffix array to be created
 * @param str File name containg string of which suffix array to be created
 * @return Suffix array
 */
extern sa_t *
sand_str_create( int  **str,
                 char *farg,
                 char **string );

/**
 * @brief Prepare integer array of given string
 * @param str File name containg string of which suffix array to be created
 * @return Suffix array
 */
extern sa_t *
sand_sa_create( char *str );

/**
 @brief Free allocations in suffix array
 @param sa Suffix array
 @return Nothing
 */
extern void
sand_sa_destroy( sa_t *sa );

#endif
/** __SAND_H__ */
