#include "beijingtime.h"
#include <list>

using namespace std;

beijingTime::beijingTime() : Subject ()
{

}

void beijingTime::notify()
{
    list<Observer *>::iterator itr;
    for(itr = li.begin(); itr != li.end(); ++itr)
    {
        (*itr)->update(_hour, _min, _sec);
    }
}

void beijingTime::setTime(int hour, int min, int sec)
{
    _hour = hour;
    _min = min;
    _sec = sec;
    notify();
}

void beijingTime::getTime()
{
    cout<<_hour<<":"<<_min<<":"<<_sec<<endl;
}
