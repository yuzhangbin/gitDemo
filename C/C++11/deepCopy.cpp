#include "deepCopy.h"

deepCopy::deepCopy(int len)
{
    std::cout<<"deepCopy len = "<<len<<std::endl;
    this->str = new char[len];
}

deepCopy::deepCopy(const deepCopy &another)
{
    int len = strlen(another.str);
    this->str = new char[len];
    strcpy(this->str, another.str);
}

deepCopy & deepCopy::operator=(const deepCopy &another)
{
    if(this == &another)
    {
        return *this;
    }
    int len = strlen(another.str);
    this->str = new char[len];
    strcpy(this->str, another.str);
    return *this;
}

void deepCopy::print()
{
    std::cout<<"this str length = "<<strlen(this->str)<<std::endl;
}

deepCopy::~deepCopy()
{
    delete []this->str;
}