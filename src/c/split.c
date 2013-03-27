/*
 * =====================================================================================
 *
 *       Filename:  split.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/26/2013 11:14:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zilong.Zhu (), zilong.whu@gmail.com
 *   Organization:  edu.whu
 *
 * =====================================================================================
 */
#include <string.h>
#include "c/split.h"

int split_str(char *str, const char *seps, char *kwords[], size_t sz)
{
    if (!(str&&seps))
        return -1;
    if ('\0' == seps[0])
    {
        if (kwords && sz > 0)
            kwords[0] = str;
        return 1;
    }
    size_t num = 0;                             // 记录关键词数目;
    char *p = str, *q = NULL;
    while (1)
    {
        q = ::strpbrk(p, seps);                 // 找下一个分隔符的位置
        if (q)
        {
            if (kwords && num < sz)             // kwords数组未满
                kwords[num] = p;
            if (kwords)
                *q = '\0';
            ++num;
        }
        else break;
        p = q + 1;
    }
    return num;
}
