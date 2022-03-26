#include <raylib.h>
#include <raygui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "patch.h"
#include "config.h"

#define RAYGUI_STANDALONE

int main()
{
        int removeSys   = true;
        int enableSound = false;
        int patching    = false;
        
        unsigned progress = 0;

        const char *fileMsg   = FILE_MSG_INIT;
        const char *statusMsg = "";

        int nFiles = 0;
        char fileName[MAX_FILENAME_LEN] = {0};
        
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
        Image icon = LoadImage(ICON_PATH);
        SetWindowIcon(icon);
        GuiLoadStyle(STYLE_PATH);
        
        InitAudioDevice(); 
        
        SetTargetFPS(TARGET_FPS);               
        GuiSetStyle(DEFAULT, TEXT_SIZE, PATCHER_TEXT_SIZE);

        /* Init music */
        Music music = LoadMusicStream(MUSIC_PATH);
        music.looping = true;
        PlayMusicStream(music);
        SetMusicPitch(music, GANDALF_PITCH);
        SetMusicVolume(music, enableSound);

        /* Load Gandalf */
        Texture2D gandalf = LoadTexture(GANDALF_PATH);
        Rectangle srcFrame = {
                0.0f, 
                0.0f, 
                gandalf.width / GANDALF_FRAMES_COUNT, 
                gandalf.height,
        };

        Rectangle dstFrame = {
                0, 0, 
                SCREEN_WIDTH, 
                SCREEN_HEIGHT,
        };

        const float gandalfScale = SCREEN_HEIGHT / gandalf.height;

        /* Animation settings */
        unsigned tick     = 0;
        unsigned curFrame = 0;
      
        while (!WindowShouldClose())
        {
                UpdateMusicStream(music); 

                if (patching) {
                        /* Animate Gandalf */
                        tick = ++tick % TICKS_PER_FRAME;
                        if (!tick)
                                srcFrame.x = (++curFrame % GANDALF_FRAMES_COUNT) * 
                                                gandalf.width / GANDALF_FRAMES_COUNT;

                        if (IsKeyDown(KEY_SPACE))
                                progress += SPEEDUP;
                                
                        /* Move progress bar */
                        ++progress;
                        if (progress > PATCH_TIME) {
                                progress = 0;
                                if (patch(fileName))
                                        statusMsg = "Something went wrong during patching...";

                                patching = false;
                        }
                }

                if (IsFileDropped()) {
                        if (!progress) {
                                /* Get dropped file name */
                                strncpy(fileName, *GetDroppedFiles(&nFiles), sizeof(fileName));
                                fileMsg = fileName;
                                statusMsg = "";
                        }

                        ClearDroppedFiles();
                }

                /* Immediate GUI draw begin */
                BeginDrawing();
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

                /* Draw Gandalf frame */
                DrawTextureTiled(gandalf, srcFrame, dstFrame, (Vector2) {0, 0}, 0, gandalfScale, WHITE);

                GuiSetState(GUI_STATE_NORMAL);
                /*---------------------------------------------------------------------*/            
                GuiLabel(COPYRIGHT_RECT, COPYRIGHT_LBL);
                /*---------------------------------------------------------------------*/

                /* Disable control elements while patching */
                if (patching)
                        GuiSetState(GUI_STATE_DISABLED);
                
                /*---------------------------------------------------------------------*/
                GuiLabel(SETTIGS_RECT, SETTIGS_LBL);
                
                enableSound = GuiCheckBox(ENABLE_SOUND_RECT, ENABLE_SOUND_LBL, enableSound);
                removeSys   = GuiCheckBox(REMOVE_SYS_RECT, REMOVE_SYS_LBL, removeSys);
                SetMusicVolume(music, enableSound);
                /*---------------------------------------------------------------------*/
                
                /*---------------------------------------------------------------------*/
                /* Drag and drop file section */
                GuiLabel(FILE_RECT, FILE_LBL);
                if (fileMsg)
                        GuiLabel(FILE_MSG_RECT, fileMsg);
                /*---------------------------------------------------------------------*/

                /*---------------------------------------------------------------------*/
                /* Status bar messages */
                if (statusMsg)
                        GuiLabel(STATUS_MSG_RECT, statusMsg);
                /*---------------------------------------------------------------------*/

                GuiSetState(GUI_STATE_NORMAL);

                /* Draw patching progress */
                GuiProgressBar(PROGRESSBAR_RECT, "", "", progress, 0, PATCH_TIME);

                if (!nFiles)
                        GuiSetState(GUI_STATE_DISABLED);
                        
                if (patching) {
                        GuiLabel(PRESS_SPACE_RECT, PRESS_SPACE_LBL);
                } else {
                        patching = GuiButton(PATCH_RECT, GuiIconText(RAYGUI_ICON_EXPLOSION, PATCH_LBL));  

                        if (patching) {                        
                                patching = false;
                                
                                int info = get_patch_info(fileName);
                                switch (info) {
                                case INFO_UNPATCHED:        
                                        patching = true;
                                        break;
                                case INFO_PATCHED:
                                        statusMsg = "File is already patched.";
                                        break;
                                case INFO_UNSUPPORTED:
                                        statusMsg = "Unsupported file format. File has to be ELF.";
                                        break;
                                default:
                                        statusMsg = "Unknown error...";
                                        break;
                                }
                        }                            
                }
               
                EndDrawing();
        }

        UnloadTexture(gandalf);
        ClearDroppedFiles();
        CloseWindow();
        return 0;
}
