#ifndef CONFIG_H
#define CONFIG_H

#include <raylib.h>


/* General settings */
const unsigned TARGET_FPS               = 60;
const char TITLE[]                      = "Saxguy patcher";

const unsigned PATCH_TIME               = 100000;
const unsigned SPEEDUP                  = 1000;

const unsigned MAX_FILENAME_LEN         = 256;

const char    ICON_PATH[]               = "resources/icon.png";
const char   STYLE_PATH[]               = "resources/cyber.rgs";


/* Common sizes */
const float UNIT                        = 20;
const float SCREEN_WIDTH                = 40 * UNIT;
const float SCREEN_HEIGHT               = 24 * UNIT;

const float PATCHER_TEXT_SIZE           = 1.5 * UNIT;


/* Copyright label */
const Rectangle COPYRIGHT_RECT          = {UNIT, UNIT, SCREEN_WIDTH, UNIT};
const char      COPYRIGHT_LBL[]         = "Copyright (C) Saxguy technologies";


/* Gandalf settings */
const unsigned TICKS_PER_FRAME          = 2;
const unsigned GANDALF_FRAMES_COUNT     = 16
;
const float    GANDALF_PITCH            = 0.77f;
const char     GANDALF_PATH[]           = "resources/gandalf.png";
const char     MUSIC_PATH[]             = "resources/sax-guy.mp3";


/* Settings checkboxes */
const Rectangle SETTIGS_RECT            = {UNIT, 4 * UNIT, SCREEN_WIDTH, UNIT};
const char      SETTIGS_LBL[]           = "Patch settings:";

const Rectangle ENABLE_SOUND_RECT       = {UNIT, 6 * UNIT, UNIT, UNIT};
const char      ENABLE_SOUND_LBL[]      = "Enable sounds";

const Rectangle REMOVE_SYS_RECT         = {UNIT, 8 * UNIT, UNIT, UNIT};
const char      REMOVE_SYS_LBL[]        = "Delete system";


/* Drag and drop file section */
const Rectangle FILE_RECT               = {UNIT, SCREEN_HEIGHT - 11 * UNIT, SCREEN_WIDTH, 2 * UNIT};
const char      FILE_LBL[]              = "File:";

const char        FILE_MSG_INIT[]       = "Drop file to patch here...";
const Rectangle   FILE_MSG_RECT         = {UNIT, SCREEN_HEIGHT - 9 * UNIT, SCREEN_WIDTH, UNIT};
const Rectangle STATUS_MSG_RECT         = {UNIT, SCREEN_HEIGHT - 2 * UNIT, SCREEN_WIDTH, UNIT};


/* Patch button section */
const Rectangle PROGRESSBAR_RECT        = {UNIT, SCREEN_HEIGHT - 7 * UNIT, SCREEN_WIDTH - 2 * UNIT, UNIT};

const Rectangle  PRESS_SPACE_RECT       = {UNIT, SCREEN_HEIGHT - 5 * UNIT, SCREEN_WIDTH, UNIT};
const char       PRESS_SPACE_LBL[]      = "Press SPACE to speed up patching";

const Rectangle  PATCH_RECT             = {UNIT, SCREEN_HEIGHT - 5 * UNIT, SCREEN_WIDTH  - 2 * UNIT, 2 * UNIT};
const char       PATCH_LBL[]            = "patch";


#endif /* CONFIG_H */

