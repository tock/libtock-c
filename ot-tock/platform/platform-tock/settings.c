#include<openthread/platform/settings.h>

// Our current approach is to use the flash.c API rather than the settings flash API (this 
// is configured in openthread-system.h). This file is left for now as a place holder.

void otPlatSettingsInit(otInstance *aInstance, const uint16_t *aSensitiveKeys, uint16_t aSensitiveKeysLength){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
	OT_UNUSED_VARIABLE(aSensitiveKeys);
	OT_UNUSED_VARIABLE(aSensitiveKeysLength);

}


void otPlatSettingsDeinit(otInstance *aInstance){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
}

otError otPlatSettingsGet(otInstance *aInstance, uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
	OT_UNUSED_VARIABLE(aKey);
	OT_UNUSED_VARIABLE(aIndex);
	OT_UNUSED_VARIABLE(aValue);
	OT_UNUSED_VARIABLE(aValueLength);

	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsSet(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
	OT_UNUSED_VARIABLE(aKey);
	OT_UNUSED_VARIABLE(aValue);
	OT_UNUSED_VARIABLE(aValueLength);

	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsAdd(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
	OT_UNUSED_VARIABLE(aKey);
	OT_UNUSED_VARIABLE(aValue);
	OT_UNUSED_VARIABLE(aValueLength);

	return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsDelete(otInstance *aInstance, uint16_t aKey, int aIndex){
	// TODO
	OT_UNUSED_VARIABLE(aInstance);
	OT_UNUSED_VARIABLE(aKey);
	OT_UNUSED_VARIABLE(aIndex);

	return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatSettingsWipe(otInstance *aInstance){
	OT_UNUSED_VARIABLE(aInstance);
	// TODO
}