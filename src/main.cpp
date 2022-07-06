#include "Application.h"
#include "VertexLayout.h"

int main()
{
    Application a;
    if (!a.initialize("demo", 800, 600))
        return 1;

    VertexLayout vlayout;
    VertexBuffer vbuffer;

    vlayout.AddVertexAttribute("pos", 2);
    vlayout.AddVertexAttribute("color", 3);

    float data[] = { 0.2f, 0.2f, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f, 1.0f, 1.0f, 1.0f };

    vbuffer.create(data, vlayout, sizeof(data) / vlayout.size());

    a.run();

    return 0;
}
