/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/26/2013 11:28:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zilong.Zhu (), zilong.whu@gmail.com
 *   Organization:  edu.whu
 *
 * =====================================================================================
 */
#include "c/split.h"
#include "cpp/split.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<std::string> fields;
    split_str("a\tb\tc\t,::d:e,,,", "\t,:", fields);
    for (size_t i = 0; i < fields.size(); ++i)
    {
        std::cout << fields[i] << std::endl;
    }
    return 0;
}
