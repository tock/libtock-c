#include<openthread/platform/settings.h>

void otPlatSettingsInit(otInstance *aInstance, const uint16_t *aSensitiveKeys, uint16_t aSensitiveKeysLength){
	// TODO
}


void otPlatSettingsDeinit(otInstance *aInstance){
	// TODO
}

otError otPlatSettingsGet(otInstance *aInstance, uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength){
	// TODO
	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsSet(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength){
	// TODO
	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsAdd(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength){
	// TODO
	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsDelete(otInstance *aInstance, uint16_t aKey, int aIndex){
	// TODO
	return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatSettingsWipe(otInstance *aInstance){
	// TODO
}