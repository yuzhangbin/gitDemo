#ifndef __DEEPCOPY_H__
#define __DEEPCOPY_H__

#include <iostream>
#include <string.h>

class deepCopy
{
    public:
        deepCopy(int len);
        //拷贝构造函数（深拷贝）
        deepCopy(const deepCopy &another);
        //赋值运算符重载（深拷贝）
        deepCopy &operator=(const deepCopy &another);
        void print();
        ~deepCopy();
    private:
        char *str;
};

#endif