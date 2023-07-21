#ifndef BEIJINGTIME_H
#define BEIJINGTIME_H

#include "subject.h"


class beijingTime : public Subject
{
public:
    beijingTime();

    void notify();

    void getTime();
    void setTime(int hour, int min, int sec);
};

#endif // BEIJINGTIME_H
