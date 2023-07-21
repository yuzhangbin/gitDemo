#include "dongjingtime.h"

dongjingTime::dongjingTime() : Observer()
{

}

void dongjingTime::update(int hour, int min, int sec)
{
    _hour = hour + 100;
    _min = min;
    _sec = sec;
    dis();
}
