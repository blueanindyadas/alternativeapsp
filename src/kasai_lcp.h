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
#ifndef __KASAI_LCP_H__
#define __KASAI_LCP_H__

#include <sa.h>

/******************************* Procedure Declaration ************************/

/**
 * @fn int kasai_sa_lcp_create( sa_t *sa );
 * @brief Create LCP of SA (Suffix Arrays)
 * @param sa Suffix Array structure
 * @return 0 on success; -1 otherwise
 **/
extern int
kasai_sa_lcp_create( sa_t *sa );

#endif
/* __KASAI_LCP_H__ */
