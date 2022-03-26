#include <raylib.h>
#include <raygui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "patch.h"

#define RAYGUI_STANDALONE

/* Window settings */
const unsigned TARGET_FPS = 60;

const unsigned UNIT = 20;
const unsigned SCREEN_WIDTH  = 40 * UNIT;
const unsigned SCREEN_HEIGTH = 24 * UNIT;

int main()
{
        int removeSys   = true;
        int enableSound = false;
        int patching    = false;
        
        unsigned progress = 0;

        const char *fileMsg   = "Drop file to patch here...";
        const char *statusMsg = "";

        int nFiles = 0;
        char fileName[256] = {0};

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Saxguy patcher");
        InitAudioDevice(); 
        
        SetTargetFPS(TARGET_FPS);               
        GuiSetStyle(DEFAULT, TEXT_SIZE, UNIT);

        /* Init music */
        Music music = LoadMusicStream("resources/sax-guy.mp3");
        music.looping = true;
        PlayMusicStream(music);
        SetMusicPitch(music, 0.77f);
        SetMusicVolume(music, enableSound);

        /* Gandalf settings */
        Texture2D gandalf = LoadTexture("resources/gandalf.png");
        const unsigned nFrames = 16;
        
        Rectangle  srcFrame = {
                0.0f, 
                0.0f, 
                gandalf.width / nFrames, 
                gandalf.height,
        };
        
        Rectangle dstFrame = {
                SCREEN_WIDTH / 1.5, 7   * UNIT, 
                gandalf.width / nFrames * UNIT * 1.0/10, 
                gandalf.height          * UNIT * 1.0/10,
        };

        const Vector2 gandalfPosition = {
                gandalf.width / nFrames, 
                gandalf.height
        };

        const float gandalfScale = 1.0/10 * UNIT;

        /* Animation settings */
        unsigned tick          = 0;
        unsigned curFrame      = 0;
        unsigned ticksPerFrame = 2;
      
    
        while (!WindowShouldClose())
        {
                UpdateMusicStream(music); 

                if (patching) {
                        /* Animate Gandalf */
                        tick = ++tick % ticksPerFrame;
                        if (!tick)
                                srcFrame.x = (++curFrame % nFrames) * gandalf.width / nFrames;

                        /* Move progress bar */
                        progress = ++progress % 1000;
                        if (!progress)
                                patching = false;
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
                ClearBackground(RAYWHITE);
                
                /*---------------------------------------------------------------------*/            
                DrawText("Copyright © Saxguy technologies", UNIT, UNIT, UNIT, BLACK);
                /*---------------------------------------------------------------------*/


                /* Disable control elements while patching */
                if (patching)
                        GuiSetState(GUI_STATE_DISABLED);
                
                /*---------------------------------------------------------------------*/
                DrawText("Patch settings:", UNIT, 4 * UNIT, UNIT, DARKGRAY); 
                
                enableSound = GuiCheckBox((Rectangle){UNIT, 6 * UNIT, UNIT, UNIT}, "Enable sound", enableSound);
                removeSys   = GuiCheckBox((Rectangle){UNIT, 8 * UNIT, UNIT, UNIT}, "Delete system", removeSys);
                SetMusicVolume(music, enableSound);
                /*---------------------------------------------------------------------*/
                
                /*---------------------------------------------------------------------*/
                /* Drag and drop file section */
                DrawText("File:", UNIT, SCREEN_HEIGTH - 11 * UNIT, 2 * UNIT, DARKGRAY);
                if (fileMsg)
                        DrawText(fileMsg, UNIT, SCREEN_HEIGTH - 9 * UNIT, UNIT, GRAY);
                /*---------------------------------------------------------------------*/

                /*---------------------------------------------------------------------*/
                /* Status bar messages */
                if (statusMsg)
                        DrawText(statusMsg, UNIT, SCREEN_HEIGTH - 2 * UNIT, UNIT, RED);
                /*---------------------------------------------------------------------*/

                GuiSetState(GUI_STATE_NORMAL);

                /* Draw Gandalf frame */
                DrawTextureTiled(gandalf, srcFrame, dstFrame, gandalfPosition, 0, gandalfScale, WHITE);

                /* Draw patching progress */
                GuiProgressBar((Rectangle){UNIT, SCREEN_HEIGTH - 7 * UNIT, SCREEN_WIDTH - 2 * UNIT, UNIT}, "", "", progress, 0, 1000);


                if (!nFiles)
                        GuiSetState(GUI_STATE_DISABLED);
                        
                if (patching) {
                        DrawText("Ooops... You have to look at the screen because you can't undo it...\n"
                                 "The file has already become corrupted...", UNIT, SCREEN_HEIGTH - 5 * UNIT, UNIT, DARKGRAY);  
                } else {
                        patching = GuiButton((Rectangle) {
                                UNIT, 
                                SCREEN_HEIGTH - 5 * UNIT, 
                                SCREEN_WIDTH - 2 * UNIT, 
                                2 * UNIT
                        }, GuiIconText(RAYGUI_ICON_EXPLOSION, "patch"));  

                        if (patching) {                        
                                patching = false;
                                
                                int info = get_patch_info(fileName);
                                switch (info) {
                                case INFO_UNPATCHED:
                                        if (patch(fileName))
                                                statusMsg = "Something went wrong during patching...";
                                                
                                        patching = true;
                                        break;
                                case INFO_PATCHED:
                                        statusMsg = "The file is already patched";
                                        break;
                                case INFO_UNSUPPORT:
                                        statusMsg = "Unsupported file format. File have to be ELF.";
                                        break;
                                default:
                                        statusMsg = "Strange error...";
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
