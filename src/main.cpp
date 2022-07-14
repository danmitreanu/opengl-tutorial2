#include "Engine.h"

int main()
{
    Engine a;
    if (!a.initialize("demo", 800, 600))
        return 1;

    a.run();

    return 0;
}
