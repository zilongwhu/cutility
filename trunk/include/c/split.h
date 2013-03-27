/*
 * =====================================================================================
 *
 *       Filename:  split.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/26/2013 10:51:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zilong.Zhu (), zilong.whu@gmail.com
 *   Organization:  edu.whu
 *
 * =====================================================================================
 */
#ifndef __C_UTILITY_SPLIT_H__
#define __C_UTILITY_SPLIT_H__

#include <stddef.h>

int split_str(char *str, const char *seps, char *kwords[], size_t sz);

#endif
