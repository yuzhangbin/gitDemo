#ifndef DONGJINGTIME_H
#define DONGJINGTIME_H

#include "observer.h"

class dongjingTime : public Observer
{
public:
    dongjingTime();

    void update(int hour, int min, int sec);
};

#endif // DONGJINGTIME_H
