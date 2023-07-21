#ifndef CANADATIME_H
#define CANADATIME_H

#include "observer.h"

class canadaTime : public Observer
{
public:
    canadaTime();

    void update(int hour, int min, int sec);
};

#endif // CANADATIME_H
