#include "memory.h"
#include "gamestate.h"
#include "gfx/camera.h"
#include "fade.h"
#include "unknown_struct.h"
#include "extendedStageSelect.h"

#define TEXT_COLOR_WHITE 0
#define TEXT_COLOR_RED   1
#define TEXT_COLOR_BEIGE 2
#define TEXT_COLOR_BROWN 3

void extendedStageSelect_entrypoint(extendedStageSelect* self) {
    ENTER(self, extendedStageSelect_functions);
}

void extendedStageSelect_init(extendedStageSelect* self) {
    void* (*ptr_heap_alloc)(s32, u32) = heap_alloc;
    mfds_state* (*ptr_textbox_create)(void*, void*, u32) = textbox_create;
    void (*ptr_textbox_setMessagePtr)(mfds_state*, u16*, u16*, s16) = textbox_setMessagePtr;
    void (*ptr_textbox_setPos)(mfds_state*, u16, u16, s32) = textbox_setPos;
    void (*ptr_textbox_setDimensions)(mfds_state*, s8, s16, u8, u8) = textbox_setDimensions;
    void (*ptr_textbox_printNumber)(mfds_state*, u8, u32) = textbox_printNumber;
    void (*ptr_textbox_setScaleAndSomethingElse)(mfds_state*, u8, u8, f32, f32, f32, u8, u8) = textbox_setScaleAndSomethingElse;
    void (*ptr_fade_setSettings)(u16, s16, u8, u8, u8) = fade_setSettings;
    void (*ptr_goToNextFunc)(u16[], s16*) = goToNextFunc;

    u32 i;
    u16* text_buffer_ptr = NULL;

    // Header textboxes
    self->headerNamesTextboxes = ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, sizeof(mfds_state* [5]));
    for (i = 0; i < 5; i++) {
        // Create textbox struct (mfds_state)
        self->headerNamesTextboxes[i] = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);

        // Convert the text to the CV64 format, then assign said text to the textbox struct
        text_buffer_ptr = ASCII2CV64(headerNames[i], (u16*) ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, TEXT_BUFFER_SIZE));
        ptr_textbox_setMessagePtr(self->headerNamesTextboxes[i], text_buffer_ptr, NULL, 0);

        // Assign position. X pos is always 20, Y pos varies (we add a "+ 20" so that the first string is put in Y pos 20 instead of 0)
        ptr_textbox_setDimensions(self->headerNamesTextboxes[i], 1, 500, 0, 0);
        ptr_textbox_setScaleAndSomethingElse(self->headerNamesTextboxes[i], 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);

        update_text_color(self->headerNamesTextboxes[i], TEXT_COLOR_BEIGE);
    }
    // Position each of the header strings
    // CV64 Extended Map Select
    ptr_textbox_setPos(self->headerNamesTextboxes[0], 40, 10, 1);
    // Character:
    ptr_textbox_setPos(self->headerNamesTextboxes[1], 5, 200, 1);
    // Alt. Costume:
    ptr_textbox_setPos(self->headerNamesTextboxes[2], 145, 200, 1);
    // Cutscene:
    ptr_textbox_setPos(self->headerNamesTextboxes[3], 5, 220, 1);
    // Spawn:
    ptr_textbox_setPos(self->headerNamesTextboxes[4], 145, 220, 1);

    // Map Selection textboxes
    self->mapNamesTextboxes = ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, sizeof(mfds_state* [8]));
    for (i = 0; i < 8; i++) {
        // Create textbox struct (mfds_state)
        self->mapNamesTextboxes[i] = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);

        // Convert the text to the CV64 format, then assign said text to the textbox struct
        text_buffer_ptr = ASCII2CV64(mapNames[i], (u16*) ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, TEXT_BUFFER_SIZE));
        ptr_textbox_setMessagePtr(self->mapNamesTextboxes[i], text_buffer_ptr, NULL, 0);

        // Assign position. X pos is always 20, Y pos varies (we add a "+ 30" so that the first string is put in Y pos 30 instead of 0)
        ptr_textbox_setPos(self->mapNamesTextboxes[i], 10, (20 * i) + 30, 1);
        ptr_textbox_setDimensions(self->mapNamesTextboxes[i], 1, 500, 0, 0);
        ptr_textbox_setScaleAndSomethingElse(self->mapNamesTextboxes[i], 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);
    }
    update_text_color(self->mapNamesTextboxes[0], TEXT_COLOR_RED);

    // Character names textbox
    self->characterNamesTextbox = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);

    text_buffer_ptr = ASCII2CV64(characterNames[0], (u16*) ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, TEXT_BUFFER_SIZE));
    ptr_textbox_setMessagePtr(self->characterNamesTextbox, text_buffer_ptr, NULL, 0);

    ptr_textbox_setPos(self->characterNamesTextbox, 90, 200, 1);
    ptr_textbox_setDimensions(self->characterNamesTextbox, 1, 500, 0, 0);
    ptr_textbox_setScaleAndSomethingElse(self->characterNamesTextbox, 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);

    // Yes / No textbox
    self->YesNoTextbox = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);

    text_buffer_ptr = ASCII2CV64(YesNoSelection[0], (u16*) ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, TEXT_BUFFER_SIZE));
    ptr_textbox_setMessagePtr(self->YesNoTextbox, text_buffer_ptr, NULL, 0);

    ptr_textbox_setPos(self->YesNoTextbox, 260, 200, 1);
    ptr_textbox_setDimensions(self->YesNoTextbox, 1, 500, 0, 0);
    ptr_textbox_setScaleAndSomethingElse(self->YesNoTextbox, 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);

    // Cutscene
    self->cutsceneTextbox = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);
    ptr_textbox_setMessagePtr(self->cutsceneTextbox, ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, NUMBER_BUFFER_SIZE), NULL, 0);
    ptr_textbox_setPos(self->cutsceneTextbox, 90, 220, 1);
    ptr_textbox_setDimensions(self->cutsceneTextbox, 1, 500, 0, 0);
    ptr_textbox_setScaleAndSomethingElse(self->cutsceneTextbox, 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);
    ptr_textbox_printNumber(self->cutsceneTextbox, 2, 0);

    ptr_goToNextFunc(self->header.current_function, &self->header.functionInfo_ID);

    // Spawn
    self->spawnTextbox = ptr_textbox_create(self, common_cameras_array.HUD, 0x08600000);
    ptr_textbox_setMessagePtr(self->spawnTextbox, ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, NUMBER_BUFFER_SIZE), NULL, 0);
    ptr_textbox_setPos(self->spawnTextbox, 260, 220, 1);
    ptr_textbox_setDimensions(self->spawnTextbox, 1, 500, 0, 0);
    ptr_textbox_setScaleAndSomethingElse(self->spawnTextbox, 2, 2, 0.0f, 0.9f, 0.9f, TRUE, TRUE);
    ptr_textbox_printNumber(self->spawnTextbox, 2, 0);

    D_80383AB8.background_color.color_u32 = 0x000000FF;
    ptr_fade_setSettings(FADE_IN, 1, 0, 0, 0);
}

void extendedStageSelect_loop(extendedStageSelect* self) {
    void (*ptr_textbox_printNumber)(mfds_state*, u8, u32) = textbox_printNumber;
    void (*ptr_goToNextFunc)(u16[], s16*) = goToNextFunc;
    u32 i = 0;
    
    // Map selection
    if ((D_80383AB8.controllers[0].buttons_pressed & BTN_DDOWN)) {
        if (self->option == 7 && self->map < NUM_MAPS - 1) {
            self->option = 7;
            self->first_string_ID++;
            for (i = 0; i < 8; i++) {
                if (self->first_string_ID >= NUM_MAPS) {
                    self->first_string_ID = NUM_MAPS - 1;
                }
                update_text_string(self->mapNamesTextboxes[i], mapNames[self->first_string_ID + i]);
            }
        }
        else if (self->option < 7) {
            self->option++;
            update_text_color(self->mapNamesTextboxes[self->option], TEXT_COLOR_RED);
            update_text_color(self->mapNamesTextboxes[self->option - 1], TEXT_COLOR_WHITE);
        }
        self->map++;
    }
    if ((D_80383AB8.controllers[0].buttons_pressed & BTN_DUP)) {
        if (self->option == 0 && self->map > 0) {
            self->option = 0;
            self->first_string_ID--;
            for (i = 0; i < 8; i++) {
                if (self->first_string_ID < 0) {
                    self->first_string_ID = 0;
                }
                update_text_string(self->mapNamesTextboxes[i], mapNames[self->first_string_ID + i]);
            }
        }
        else if (self->option > 0) {
            self->option--;
            update_text_color(self->mapNamesTextboxes[self->option], TEXT_COLOR_RED);
            update_text_color(self->mapNamesTextboxes[self->option + 1], TEXT_COLOR_WHITE);
        }
        self->map--;
    }

    if (self->map >= NUM_MAPS) {
        self->map = NUM_MAPS - 1;
    }
    else if (self->map < 0) {
        self->map = 0;
    }

    if (D_80383AB8.controllers[0].buttons_pressed & BTN_L) {
        if (self->character == CARRIE) {
            self->character = REINHARDT;
        }
        else {
            self->character++;
        }
        update_text_string(self->characterNamesTextbox, characterNames[self->character]);
    }

    if (D_80383AB8.controllers[0].buttons_pressed & BTN_R) {
        if (self->alternate_costume == TRUE) {
            self->alternate_costume = FALSE;
        }
        else {
            self->alternate_costume = TRUE;
        }
        update_text_string(self->YesNoTextbox, YesNoSelection[self->alternate_costume]);
    }

    if (D_80383AB8.controllers[0].buttons_pressed & BTN_CRIGHT) {
        if (self->spawn >= 120) {
            self->spawn = 120;
        }
        else {
            self->spawn++;
        }
        ptr_textbox_printNumber(self->spawnTextbox, 2, self->spawn);
    }
    else if (D_80383AB8.controllers[0].buttons_pressed & BTN_CLEFT) {
        if (self->spawn <= 0) {
            self->spawn = 0;
        }
        else {
            self->spawn--;
        }
        ptr_textbox_printNumber(self->spawnTextbox, 2, self->spawn);
    }

    if (D_80383AB8.controllers[0].buttons_pressed & BTN_DRIGHT) {
        if (self->cutscene >= 99) {
            self->cutscene = 99;
        }
        else {
            self->cutscene++;
        }
        ptr_textbox_printNumber(self->cutsceneTextbox, 2, self->cutscene);
    }
    else if (D_80383AB8.controllers[0].buttons_pressed & BTN_DLEFT) {
        if (self->cutscene <= 0) {
            self->cutscene = 0;
        }
        else {
            self->cutscene--;
        }
        ptr_textbox_printNumber(self->cutsceneTextbox, 2, self->cutscene);
    }

    if (D_80383AB8.controllers[0].buttons_pressed & BTN_A) {
        ptr_goToNextFunc(self->header.current_function, &self->header.functionInfo_ID);
    }
}

void extendedStageSelect_warpToMap(extendedStageSelect* self) {
    void (*ptr_changeGameState)(u32) = changeGameState;

    D_80383AB8.map_ID = self->map;
    D_80383AB8.map_entrance_ID = self->spawn;
    D_80383AB8.entrance_cutscene_ID = self->cutscene;
    D_80383AB8.SaveStruct_gameplay.character = self->character;
    D_80383AB8.alternate_costume = self->alternate_costume;
    D_80383AB8.current_opened_menu = 0;
    ptr_changeGameState(GAMEPLAY);
}

void update_text_color(mfds_state* textbox, u32 color_palette) {
    textbox->color_palette = color_palette;
    textbox->flags |= UPDATE_STRING;
}

void update_text_string(mfds_state* textbox, const char* ascii_message) {
    void* (*ptr_heap_alloc)(s32, u32) = heap_alloc;
    void (*ptr_heapBlock_free)(void*) = heapBlock_free;
    void (*ptr_textbox_setMessagePtr)(mfds_state*, u16*, u16*, s16) = textbox_setMessagePtr;

    u16* text_buffer_ptr = NULL;
    
    ptr_heapBlock_free(textbox->text);
    text_buffer_ptr = ASCII2CV64(ascii_message, (u16*) ptr_heap_alloc(HEAP_KIND_MULTIPURPOSE, TEXT_BUFFER_SIZE));
    ptr_textbox_setMessagePtr(textbox, text_buffer_ptr, NULL, 0);
    textbox->flags |= UPDATE_STRING;
}

// Convert the ASCII message to the CV64 custom format
u16* ASCII2CV64(const char* ascii_message, u16* cv64_text_buffer) {
    u16* (*ptr_convertUTF16ToCustomTextFormat)(u16*) = convertUTF16ToCustomTextFormat;

    convertUTF8StringToUTF16(ascii_message, cv64_text_buffer);
    ptr_convertUTF16ToCustomTextFormat(cv64_text_buffer);

    return cv64_text_buffer;
}

// Helper function
// https://decomp.me/scratch/9H1Uy
u32 convertUTF8StringToUTF16(char* src, u16* buffer) {
    u32 string_length = 0;

    // If the source string starts with a null char (0), we assume the string empty.
    if (*src != 0) {
        // Copy the char from the source string into the bufferination.
        // Then advance to the next char until we find the null char (0).
        do {
            *buffer = *src;
            src++;
            buffer++;
            string_length++;
        } while (*src != 0);
    }
    // Make sure to add the null char at the end of the bufferination string,
    // and then return the length of the string.
    *buffer = 0;
    return string_length;
}
