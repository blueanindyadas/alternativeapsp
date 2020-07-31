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
#ifndef __APSP_READ_DIR_H__
#define __APSP_READ_DIR_H__

/***************************** Global Variable ********************************/

/* Number of strings */
extern unsigned int num_strings;

/* Prefix sums of strings' indices */
extern unsigned int *ps_str_idx;

/************************* Procedure Declaration ******************************/
/**
 * @brief Read all files content in a string for given directory
 * @param dirname Directory name
 * @param str_c Output string
 * @return Length of final string
 **/
extern unsigned int
apsp_read_dir( char *dirname, char **str_c );

#endif
/* __APSP_READ_DIR_H__ */
