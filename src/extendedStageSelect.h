#ifndef EXTENDED_STAGE_SELECT_H
#define EXTENDED_STAGE_SELECT_H

#include "module.h"
#include "textbox.h"
#include "unknown_struct.h"

#define NUM_MAPS 30
#define TEXT_BUFFER_SIZE 40 * sizeof(u16)
#define NUMBER_BUFFER_SIZE 4 * sizeof(u16)

typedef struct {
    ModuleHeader header;
    mfds_state** mapNamesTextboxes;
    mfds_state** headerNamesTextboxes;
    mfds_state* characterNamesTextbox;
    mfds_state* YesNoTextbox;
    mfds_state* spawnTextbox;
    mfds_state* cutsceneTextbox;
    s8 first_string_ID;
    s8 option;
    s8 map;
    s8 character;
    s8 alternate_costume;
    s8 cutscene;
    s8 spawn;
} extendedStageSelect;

void extendedStageSelect_entrypoint(extendedStageSelect* self);
void extendedStageSelect_init(extendedStageSelect* self);
void extendedStageSelect_loop(extendedStageSelect* self);
void extendedStageSelect_warpToMap(extendedStageSelect* self);
void update_text_color(mfds_state* textbox, u32 color_palette);
void update_text_string(mfds_state* textbox, const char* ascii_message);
u16* ASCII2CV64(const char* ascii_message, u16* cv64_text_buffer);
u32 convertUTF8StringToUTF16(char* src, u16* buffer);

void (*extendedStageSelect_functions[])(extendedStageSelect* self) = 
{
    extendedStageSelect_init,
    extendedStageSelect_loop,
    extendedStageSelect_warpToMap
};

const char* mapNames[NUM_MAPS] = {
    "Forest of Silence",
    "Castle Wall (Towers)",
    "Castle Wall (Main)",
    "Villa (Yard)",
    "Villa (Foyer)",
    "Villa (Hallway)",
    "Villa (Maze Garden)",
    "Tunnel",
    "Underground Waterway",
    "Castle Center (Main)",
    "Castle Center (Bottom Elevator)",
    "Castle Center (Gears)",
    "Castle Center (Friendly Lizard-man)",
    "Castle Center (Library)",
    "Castle Center (Nitro Room)",
    "Castle Center (Top Elevator)",
    "Tower of Execution",
    "Tower of Sorcery",
    "Tower of Science",
    "Duel Tower",
    "Castle Keep Stairs",
    "Castle Keep",
    "Intro Cutscene Map",
    "Clock Tower",
    "Dracula Desert",
    "Rose / Actrice Fan Room",
    "Villa (Vampire Crypt)",
    "Room of Clocks",
    "Ending Map",
    "Test Grid"
};

const char* characterNames[] = {
    "R",
    "C"
};

const char* YesNoSelection[] = {
    "NO",
    "YES"
};

const char* headerNames[] = {
    "CV64 Extended Map Select",
    "Character:",
    "Alt. Costume:",
    "Cutscene:",
    "Spawn:"
};

#endif
