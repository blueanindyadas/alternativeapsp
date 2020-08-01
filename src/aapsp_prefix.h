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
#ifndef __APSP_PREFIX_H__
#define __APSP_PREFIX_H__

/******************************** Typedefs ************************************/

/* Prefix structure used to store SA element in queue */
typedef struct prefix_t {
    unsigned int i;
    unsigned int said;
    unsigned int isa;
    unsigned int ovlen;
} prefix_t;

/* Suffix structure used to store SA element in stack */
typedef struct suffix_t {
    unsigned int i;
    unsigned int said;
    unsigned int isa;
    unsigned int ovlen;
    unsigned int lcp;
} suffix_t;
/***************************** Global Variable ********************************/

extern unsigned int *pfl;
extern unsigned long int ovlapcnt;
extern int threshold;

/************************* Procedure Definitions ******************************/

/**
 * @brief Mark prefixes in a seperate array
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
extern int
apsp_mark_prefix( sa_t *sa );

/**
 * @brief Initialize ovlap matrix
 * @return 0 on success, -1 otherwise
 */
extern int
apsp_init_ovlap();

/**
 * @brief Finds overlaps with prefix
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
extern int
apsp_mark_suffix( sa_t *sa );

/**
 * @brief Determine overlap matrix
 * @return 0 on success, -1 otherwise
 */
extern int
apsp_compute_ovlap( sa_t *sa );

/**
 * @brief Print ovlap matrix in latex format
 * @return None
 */
extern void
apsp_lprint_ovlap();

/**
 * @brief Close file for output matrix
 * @return Nothing
 **/
extern void
apsp_ovlap_close();

#endif
/* __APSP_PREFIX_H__ */

