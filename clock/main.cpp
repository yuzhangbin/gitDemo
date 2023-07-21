#include <iostream>
#include "beijingtime.h"
#include "canadatime.h"
#include "dongjingtime.h"
#include "observer.h"
#include "subject.h"

using namespace std;

int main()
{
    dongjingTime dt;
    canadaTime ct;
    beijingTime bt;

    bt.registerObserver(&dt);
    bt.registerObserver(&ct);
    bt.setTime(10, 11, 12);

    bt.getTime();

    cout<<"------------------"<<endl;

    bt.removeObserver(&dt);

    bt.setTime(100, 111, 122);

    bt.getTime();


    return 0;
}
