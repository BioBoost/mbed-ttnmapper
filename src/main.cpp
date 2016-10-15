#include "mbed.h"
#include "lorawan.h"

int main(void)
{
    init();
    while (true) {
        os_runloop_once();
    }
    return 0;
}
