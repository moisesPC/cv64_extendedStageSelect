#ifndef EXTENDED_STAGE_SELECT_CREATOR_H
#define EXTENDED_STAGE_SELECT_CREATOR_H

#include "module.h"

#define EXTENDED_STAGE_SELECT_MODULE_ID 0x2141

typedef struct {
    ModuleHeader header;
    u32 isCreated;
    u8 field_0x24[MODULE_SIZE - 0x24];
} extendedStageSelectCreator;

void extendedStageSelectCreator_entrypoint(extendedStageSelectCreator* self);

#endif