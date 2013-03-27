/*
 * =====================================================================================
 *
 *       Filename:  split.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/26/2013 11:20:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zilong.Zhu (), zilong.whu@gmail.com
 *   Organization:  edu.whu
 *
 * =====================================================================================
 */
#ifndef __C_UTILITY_CPP_SPLIT_H__
#define __C_UTILITY_CPP_SPLIT_H__

#include <string>
#include <vector>

void split_str(const char *str, const char *seps, std::vector<std::string> &fields);

#endif
