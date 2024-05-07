#include "asserts.h"

int main()
{
    assert(1 + 1 == 7);
    int x = 10; int y = 15;
    for (int i = 0; i < 5; i++)
    {
        assert(x > y);
    }
    assert(1 + 1 == 4);
    assert(1 + 1 == 10);
    fatal_error(10 < 1 + 2, 2);
    return 0;
}