#include "canadatime.h"

canadaTime::canadaTime() : Observer ()
{

}

void canadaTime::update(int hour, int min, int sec)
{
    _hour = hour + 200;
    _min = min;
    _sec = sec;
    dis();
}
