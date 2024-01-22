#include <assert.h>
#include <openthread/instance.h>

int main(int argc, char *argv[])
{
        otInstance *instance;
        instance = otInstanceInitSingle();
        assert(instance);

    for(;;)
    {
        otTaskletsProcess(instance);
    //    otSysProcessDrivers(instance); IMPLEMENT IN PLATFORM
    }

        return 0;
}


