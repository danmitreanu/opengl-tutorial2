#include "Application.h"
#include "VertexLayout.h"

int main()
{
    Application a;
    if (!a.initialize("demo", 800, 600))
        return 1;

    a.run();

    return 0;
}
