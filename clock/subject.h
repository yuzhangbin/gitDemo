#ifndef SUBJECT_H
#define SUBJECT_H

#include <list>
#include "observer.h"

using namespace std;

class Subject
{
public:
    Subject();

    void registerObserver(Observer *ob);
    void removeObserver(Observer *ob);
    virtual void notify() = 0;

protected:
    list<Observer *> li;

protected:
    int _hour;
    int _min;
    int _sec;
};

#endif // SUBJECT_H
