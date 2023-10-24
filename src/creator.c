#include "module.h"
#include "creator.h"

void extendedStageSelectCreator_entrypoint(extendedStageSelectCreator* self) {
    if (self->isCreated == FALSE) {
        module_createAndSetChild(self, EXTENDED_STAGE_SELECT_MODULE_ID);
        self->isCreated = TRUE;
    }
}
