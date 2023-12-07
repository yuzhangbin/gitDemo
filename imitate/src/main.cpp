#include <iostream>
#include "sunnet.h"

int main()
{
    Sunnet *sunnetInstance = Sunnet::instance();
    sunnetInstance->start();
    
    sunnetInstance->wait();
    return 0;
}