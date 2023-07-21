#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>

using namespace std;

class Observer
{
public:
    Observer();

    virtual void update(int hour, int min, int sec) = 0;

    void dis(){cout<<_hour<<":"<<_min<<":"<<_sec<<endl;}

protected:
    int _hour;
    int _min;
    int _sec;
};

#endif // OBSERVER_H
