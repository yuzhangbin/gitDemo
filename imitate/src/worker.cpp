#include "worker.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void Worker::operator()()
{
    while (true)
    {
        cout << "working id:" << id << endl;
        usleep(100000);
    }
    
}