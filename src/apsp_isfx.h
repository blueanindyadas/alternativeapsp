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
#ifndef __APSP_LSFX_H__
#define __APSP_LSFX_H__

/***************************** Global Variable ********************************/

extern unsigned int *isfx;
extern unsigned int *istr_no;

/************************* Procedure Definitions ******************************/

/**
 * @brief Create isfx array
 * @param sa Suffix array
 * @return 0 on success, -1 otherwise
 */
extern int
apsp_compute_isfxlen( sa_t *sa );

#endif
/* __APSP_LSFX_H__ */

