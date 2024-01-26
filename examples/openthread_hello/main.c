#include <assert.h>
#include <openthread/instance.h>

int main(int argc, char *argv[])
{
    size_t   otInstanceBufferLength = 0;
    uint8_t *otInstanceBuffer       = NULL;

    otInstance *instance;

    // obtain required buffer size
    (void)otInstanceInit(NULL, &otInstanceBufferLength);

    // Call to allocate the buffer
    otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
    assert(otInstanceBuffer);

    // Initialize OpenThread with the buffer
    /* Hangs within the below function call (see comment in loop)*/
    instance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength); 
    assert(instance);

    for(;;)
    {
        /* Until the platform library files are implemented, this will not
        be called. With the empty platform files, the Instance() constructor 
        that is called within the otInstanceInit function hangs. */
        printf("looping...\n");
	
        otTaskletsProcess(instance);
        otSysProcessDrivers(instance); 
    
    }

    return 0;
}


