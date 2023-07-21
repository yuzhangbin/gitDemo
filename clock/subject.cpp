#include "subject.h"

Subject::Subject()
{

}

void Subject::registerObserver(Observer *ob)
{
    li.push_back(ob);
}

void Subject::removeObserver(Observer *ob)
{
    li.remove(ob);
}
