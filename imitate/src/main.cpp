#include <iostream>
#include "sunnet.h"

void test()
{
    Sunnet *sunnetInstance = Sunnet::instance();
    shared_ptr<string> type = make_shared<string>("ping");
    uint32_t ping1 = sunnetInstance->newService(type);
    uint32_t ping2 = sunnetInstance->newService(type);
    uint32_t ping3 = sunnetInstance->newService(type);

    auto msg1 = sunnetInstance->makeMsg(ping1, new char[3] { 'h', 'i', '\0' }, 3);
    auto msg2 = sunnetInstance->makeMsg(ping2, new char[6] { 'h', 'e', 'l', 'l', 'o', '\0' }, 6);

    sunnetInstance->send(ping3, msg1);
    sunnetInstance->send(ping3, msg2);
}

int main()
{
    Sunnet *sunnetInstance = Sunnet::instance();
    sunnetInstance->start();
    test();
    sunnetInstance->wait();
    return 0;
}