// TODO ADD COPYRIGHT ETC

#include<openthread/platform/alarm-micro.h>
#include<openthread/platform/alarm-milli.h>
void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    printf("alarm micro start\n");
    // TODO
}

void otPlatAlarmMicroStop(otInstance *aInstance){
    printf("alarm micro stop\n");
    // TODO
}

uint32_t otPlatAlarmMicroGetNow(void){
    printf("alarm micro get now\n");
    // TODO
    return 0;
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    printf("alarm milli start\n");
    // TODO
}

void otPlatAlarmMilliStop(otInstance *aInstance){
    printf("alarm milli stop\n");
    // TODO
}

uint32_t otPlatAlarmMilliGetNow(void){
    printf("alarm milli get now\n");
    // TODO
    return 0;
}

