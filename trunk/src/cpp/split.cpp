/*
 * =====================================================================================
 *
 *       Filename:  split.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/26/2013 11:23:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zilong.Zhu (), zilong.whu@gmail.com
 *   Organization:  edu.whu
 *
 * =====================================================================================
 */

#include <string.h>
#include "cpp/split.h"

void split_str(const char *str, const char *seps, std::vector<std::string> &fields)
{
    fields.clear();
    if (!(str&&seps))
        return ;
    if ('\0' == seps[0])
    {
        fields.push_back(str);
        return ;
    }
    const char *p = str, *q = NULL;
    while (1)
    {
        q = ::strpbrk(p, seps);                 // 找下一个分隔符的位置
        if (q) fields.push_back(std::string(p, q));
        else break;
        p = q + 1;
    }
}
