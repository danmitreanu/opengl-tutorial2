#include "Engine.h"

int main()
{
    Engine a;
    if (!a.initialize("demo", 1200, 900))
        return 1;

    a.run();

    return 0;
}
